// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerCharacter.h"
#include "EODPlayerController.h"
#include "Core/GameSingleton.h"
#include "PlayerAnimInstance.h"
#include "Weapons/PrimaryWeapon.h"
#include "Weapons/SecondaryWeapon.h"
#include "Statics/WeaponLibrary.h"
#include "UI/HUDWidget.h"
#include "UI/SkillBarWidget.h"
#include "Components/InventoryComponent.h"
#include "Components/PlayerStatsComponent.h"
#include "Components/SkillBarComponent.h"
#include "Components/SkillTreeComponent.h"
#include "Core/EODPreprocessors.h"
#include "Core/EODSaveGame.h"
#include "UI/SkillTreeWidget.h"
#include "Statics/EODBlueprintFunctionLibrary.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameUserSettings.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerStatsComponent>(FName("Character Stats Component")))
{
	PrimaryActorTick.bCanEverTick = true;

	// The player should be free to look around with mouse without actually rotating the posessed character
	bUseControllerRotationYaw = false;
	bReplicates = true;
	bReplicateMovement = true;
	GetCharacterMovement()->SetIsReplicated(true);

	// @note Defaul skeletal mesh component inherited from ACharacter class will contain face mesh
	if (GetMesh())
	{
		GetMesh()->AddLocalOffset(FVector(0.f, 0.f, -90.f));
		GetMesh()->bUseAttachParentBound = true;
	}

	// @note : SetMasterPoseComponent() from constructor doesn't work in packaged game (for some weird reason?!)

	Hair			= CreateNewArmorComponent(FName("Hair"), ObjectInitializer);
	HatItem			= CreateNewArmorComponent(FName("Hat Item"), ObjectInitializer);
	FaceItem		= CreateNewArmorComponent(FName("Face Item"), ObjectInitializer);
	Chest			= CreateNewArmorComponent(FName("Chest"), ObjectInitializer);
	Hands			= CreateNewArmorComponent(FName("Hands"), ObjectInitializer);
	Legs			= CreateNewArmorComponent(FName("Legs"), ObjectInitializer);
	Feet			= CreateNewArmorComponent(FName("Feet"), ObjectInitializer);

	//~ Begin Camera Components Initialization
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, FName("Camera Boom"));
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->AddLocalOffset(FVector(0.f, 0.f, 60.f));

	PlayerCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, FName("Camera"));
	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//~ End Camera Components Initialization

	InventoryComponent = ObjectInitializer.CreateDefaultSubobject<UInventoryComponent>(this, FName("Player Inventory"));
	SkillBarComponent = ObjectInitializer.CreateDefaultSubobject<USkillBarComponent>(this, FName("Skill Bar"));
	SkillTreeComponent = ObjectInitializer.CreateDefaultSubobject<USkillTreeComponent>(this, FName("Skill Tree"));

	AudioComponent = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, FName("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);

	SetWalkSpeed(BaseNormalMovementSpeed * GetStatsComponent()->GetMovementSpeedModifier());

	// By default the weapon should be sheathed
	bWeaponSheathed = true;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//~ Begin Axis Input Bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	//~ End Axis Input Bindings
	
	//~ Begin Action Input Bindings
	PlayerInputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &APlayerCharacter::ZoomInCamera);
	PlayerInputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &APlayerCharacter::ZoomOutCamera);

	PlayerInputComponent->BindAction("Forward", IE_Pressed, this, &APlayerCharacter::OnPressedForward);
	PlayerInputComponent->BindAction("Forward", IE_Released, this, &APlayerCharacter::OnReleasedForward);
	PlayerInputComponent->BindAction("Backward", IE_Pressed, this, &APlayerCharacter::OnPressedBackward);
	PlayerInputComponent->BindAction("Backward", IE_Released, this, &APlayerCharacter::OnReleasedBackward);

	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &APlayerCharacter::OnPressedBlock);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &APlayerCharacter::OnReleasedBlock);

	PlayerInputComponent->BindAction("NormalAttack", IE_Pressed, this, &APlayerCharacter::OnPressedNormalAttack);
	PlayerInputComponent->BindAction("NormalAttack", IE_Released, this, &APlayerCharacter::OnReleasedNormalAttack);
	
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &APlayerCharacter::OnDodge);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::OnJump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::OnInteract);
	PlayerInputComponent->BindAction("ToggleSheathe", IE_Pressed, this, &APlayerCharacter::OnToggleSheathe);
	PlayerInputComponent->BindAction("ToggleStats", IE_Pressed, this, &APlayerCharacter::OnToggleCharacterStatsUI);
	PlayerInputComponent->BindAction("ToggleMouseCursor", IE_Pressed, this, &APlayerCharacter::OnToggleMouseCursor);
	PlayerInputComponent->BindAction("ToggleSkillTree", IE_Pressed, SkillTreeComponent, &USkillTreeComponent::ToggleSkillTreeUI);
	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, InventoryComponent, &UInventoryComponent::ToggleInventoryUI);
	PlayerInputComponent->BindAction("ToggleAutoRun", IE_Pressed, this, &APlayerCharacter::OnToggleAutoRun);
	PlayerInputComponent->BindAction("Escape", IE_Pressed, this, &APlayerCharacter::OnPressedEscape);

	PlayerInputComponent->BindAction("Skill_1", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<1>);
	PlayerInputComponent->BindAction("Skill_2", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<2>);
	PlayerInputComponent->BindAction("Skill_3", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<3>);
	PlayerInputComponent->BindAction("Skill_4", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<4>);
	PlayerInputComponent->BindAction("Skill_5", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<5>);
	PlayerInputComponent->BindAction("Skill_6", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<6>);
	PlayerInputComponent->BindAction("Skill_7", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<7>);
	PlayerInputComponent->BindAction("Skill_8", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<8>);
	PlayerInputComponent->BindAction("Skill_9", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<9>);
	PlayerInputComponent->BindAction("Skill_10", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<10>);
	PlayerInputComponent->BindAction("Skill_11", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<11>);
	PlayerInputComponent->BindAction("Skill_12", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<12>);
	PlayerInputComponent->BindAction("Skill_13", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<13>);
	PlayerInputComponent->BindAction("Skill_14", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<14>);
	PlayerInputComponent->BindAction("Skill_15", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<15>);
	PlayerInputComponent->BindAction("Skill_16", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<16>);
	PlayerInputComponent->BindAction("Skill_17", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<17>);
	PlayerInputComponent->BindAction("Skill_18", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<18>);
	PlayerInputComponent->BindAction("Skill_19", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<19>);
	PlayerInputComponent->BindAction("Skill_20", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<20>);

	PlayerInputComponent->BindAction("Skill_1", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<1>);
	PlayerInputComponent->BindAction("Skill_2", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<2>);
	PlayerInputComponent->BindAction("Skill_3", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<3>);
	PlayerInputComponent->BindAction("Skill_4", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<4>);
	PlayerInputComponent->BindAction("Skill_5", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<5>);
	PlayerInputComponent->BindAction("Skill_6", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<6>);
	PlayerInputComponent->BindAction("Skill_7", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<7>);
	PlayerInputComponent->BindAction("Skill_8", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<8>);
	PlayerInputComponent->BindAction("Skill_9", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<9>);
	PlayerInputComponent->BindAction("Skill_10", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<10>);
	PlayerInputComponent->BindAction("Skill_11", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<11>);
	PlayerInputComponent->BindAction("Skill_12", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<12>);
	PlayerInputComponent->BindAction("Skill_13", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<13>);
	PlayerInputComponent->BindAction("Skill_14", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<14>);
	PlayerInputComponent->BindAction("Skill_15", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<15>);
	PlayerInputComponent->BindAction("Skill_16", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<16>);
	PlayerInputComponent->BindAction("Skill_17", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<17>);
	PlayerInputComponent->BindAction("Skill_18", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<18>);
	PlayerInputComponent->BindAction("Skill_19", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<19>);
	PlayerInputComponent->BindAction("Skill_20", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<20>);

	//~ End Action Input Bindings

}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APlayerCharacter, IWR_CharacterMovementDirection, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(APlayerCharacter, CurrentWeaponAnimationToUse, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(APlayerCharacter, BlockMovementDirectionYaw, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(APlayerCharacter, bWeaponSheathed, COND_SkipOwner);

}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = this;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PrimaryWeapon = GetWorld()->SpawnActor<APrimaryWeapon>(APrimaryWeapon::StaticClass(), SpawnInfo);
	SecondaryWeapon = GetWorld()->SpawnActor<ASecondaryWeapon>(ASecondaryWeapon::StaticClass(), SpawnInfo);

	LoadUnequippedWeaponAnimationReferences();

	// @note Set secondary weapon first and primary weapon later during initialization
	SetCurrentSecondaryWeapon(SecondaryWeaponID);
	SetCurrentPrimaryWeapon(PrimaryWeaponID);

	if (HUDWidgetClass.Get())
	{
		HUDWidget = CreateWidget<UHUDWidget>(GetGameInstance(), HUDWidgetClass);
	}
}

void APlayerCharacter::PostInitProperties()
{
	Super::PostInitProperties();
}

#if WITH_EDITOR
void APlayerCharacter::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetActiveAnimationReferences())
	{
		return;
	}
	
	if (IsDead())
	{
		UAnimMontage* DeathMontage = GetActiveAnimationReferences()->Die.Get();

		if (GetCharacterState() != ECharacterState::Dead)
		{
			PlayAnimationMontage(DeathMontage, UCharacterLibrary::SectionName_Default, ECharacterState::Dead);
		}
		else
		{
			// @todo Revival options when the death animation finishes playing
		}

		return;
	}

	/*
	if (GetCharacterMovement()->IsFalling())
	{
		UAnimMontage* JumpMontage = GetActiveAnimationReferences()->Jump.Get();

		if (GetCharacterState() != ECharacterState::Jumping)
		{
			PlayAnimationMontage(JumpMontage, UCharacterLibrary::SectionName_JumpStart, ECharacterState::Jumping);
		}
		else if (GetCharacterState() == ECharacterState::Jumping && GetMesh()->GetAnimInstance()->Montage_IsPlaying(JumpMontage))
		{

		}
	}


	if (GetCharacterMovement()->IsFalling() && !IsJumping())
	{
		// SetCharacterState(ECharacterState::Jumping);

		// This case is an exception to CharacterState replication
		CharacterState = ECharacterState::Jumping;
		GetMesh()->GetAnimInstance()->Montage_Play(GetActiveAnimationReferences()->AnimationMontage_Jump);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(UCharacterLibrary::SectionName_JumpLoop,
			GetActiveAnimationReferences()->AnimationMontage_Jump);
	}
	// It is necessary to test if jump montage is playing, or else the "JumpEnd" section ends up playing twice because of montage blending out
	else if (!GetCharacterMovement()->IsFalling() && IsJumping() && GetMesh()->GetAnimInstance()->Montage_IsPlaying(GetActiveAnimationReferences()->AnimationMontage_Jump))
	{
		FName CurrentSection = GetMesh()->GetAnimInstance()->Montage_GetCurrentSection(GetActiveAnimationReferences()->AnimationMontage_Jump);
		if (CurrentSection != UCharacterLibrary::SectionName_JumpEnd)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(GetActiveAnimationReferences()->AnimationMontage_Jump);
			GetMesh()->GetAnimInstance()->Montage_JumpToSection(UCharacterLibrary::SectionName_JumpEnd,
				GetActiveAnimationReferences()->AnimationMontage_Jump);
		}
	}
	*/

	if (IsLocallyControlled())
	{
		// If block key is pressed but the character is not blocking
		if (bBlockPressed && !IsBlocking() && CanBlock())
		{
			EnableBlock();
		}
		// If block is not pressed but character is blocking
		else if (!bBlockPressed && IsBlocking())
		{
			DisableBlock();
		}

		if (IsIdle())
		{
			UpdateIdleState(DeltaTime);
		}
		else if (IsAutoRunning())
		{
			UpdateAutoRun(DeltaTime);
		}
		else if (IsMoving())
		{
			UpdateMovement(DeltaTime);
		}
		else if (IsBlocking())
		{
			UpdateBlockState(DeltaTime);
		}

		if (bRotateSmoothly)
		{
			bRotateSmoothly = !DeltaRotateCharacterToDesiredYaw(DesiredSmoothRotationYaw, DeltaTime);
		}
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	if (IsLocallyControlled() && HUDWidget)
	{
		HUDWidget->AddToViewport();
	}
}

USkeletalMeshComponent * APlayerCharacter::CreateNewArmorComponent(const FName Name, const FObjectInitializer & ObjectInitializer)
{
	USkeletalMeshComponent* Sk = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, Name);
	Sk->SetupAttachment(GetMesh());
	Sk->SetMasterPoseComponent(GetMesh());
	Sk->bUseAttachParentBound = true;
	return Sk;
}

bool APlayerCharacter::CanMove() const
{
	return IsIdleOrMoving() || IsBlocking() || IsAutoRunning();
}

bool APlayerCharacter::CanJump() const
{
	return IsIdleOrMoving() || IsBlocking() || IsAutoRunning();
}

bool APlayerCharacter::CanDodge() const
{
	int32 DodgeCost = DodgeStaminaCost / GetStatsComponent()->GetStaminaConsumptionModifier();

	if (GetStatsComponent()->GetCurrentStamina() >= DodgeCost &&
		(IsIdleOrMoving() || IsBlocking() || IsCastingSpell() || IsNormalAttacking()))
	{
		return true;
	}

	return false;
	// @todo add UsingSkill, Looting, Interacting, etc. to this too
}

bool APlayerCharacter::CanBlock() const
{
	return (Super::CanBlock() || IsAutoRunning() || IsNormalAttacking()) &&
		!(CurrentWeaponAnimationToUse == EWeaponAnimationType::NoWeapon || CurrentWeaponAnimationToUse == EWeaponAnimationType::SheathedWeapon);
}

bool APlayerCharacter::CanNormalAttack() const
{
	return IsIdleOrMoving() || IsNormalAttacking();

	// return CharacterState == ECharacterState::IdleWalkRun || CharacterState == ECharacterState::Attacking;
}

bool APlayerCharacter::CanUseAnySkill() const
{
	return GetEquippedWeaponType() != EWeaponType::None && !bWeaponSheathed && IsIdleOrMoving() && IsBlocking();
}

bool APlayerCharacter::IsAutoRunning() const
{
	return GetCharacterState() == ECharacterState::AutoRun;
}

FORCEINLINE APrimaryWeapon* APlayerCharacter::GetPrimaryWeapon() const
{
	return PrimaryWeapon;
}

FORCEINLINE ASecondaryWeapon* APlayerCharacter::GetSecondaryWeapon() const
{
	return SecondaryWeapon;
}

FORCEINLINE EWeaponType APlayerCharacter::GetEquippedWeaponType() const
{
	return PrimaryWeapon->WeaponType;
}

FORCEINLINE UHUDWidget* APlayerCharacter::GetHUDWidget() const
{
	return HUDWidget;
}

bool APlayerCharacter::IsWeaponSheathed() const
{
	return bWeaponSheathed;
}

UHUDWidget* APlayerCharacter::BP_GetHUDWidget() const
{
	return GetHUDWidget();
}

bool APlayerCharacter::Interrupt(const float BCAngle)
{
	// if (CanInterrupt() && )

	/*
	if (CanInterrupt() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->AnimationMontage_HitEffects)
	{
		if (BCAngle <= 90)
		{
			PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_HitEffects,
				UCharacterLibrary::SectionName_ForwardInterrupt,
				ECharacterState::GotHit);
		}
		else
		{
			PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_HitEffects,
				UCharacterLibrary::SectionName_BackwardInterrupt,
				ECharacterState::GotHit);
		}

		return true;
	}
	*/
	return false;
}

bool APlayerCharacter::Flinch(const float BCAngle)
{
	/*
	if (CanFlinch() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->AnimationMontage_Flinch)
	{
		if (BCAngle <= 90)
		{
			PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Flinch,
				UCharacterLibrary::SectionName_ForwardFlinch);
		}
		else
		{
			PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Flinch,
				UCharacterLibrary::SectionName_BackwardFlinch);
		}

		return true;
	}
	*/

	return false;
}

bool APlayerCharacter::Stun(const float Duration)
{
	if (CanStun())
	{
		PlayStunAnimation();
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &APlayerCharacter::EndStun, Duration, false);

		return true;
	}

	return false;
}

void APlayerCharacter::EndStun()
{
	StopStunAnimation();
	
	// @todo Restore character state to IdleWalkRun if necessary (if OnMontageBlendingOut event doesn't restore character state to IdleWalkRun)
}

bool APlayerCharacter::Freeze(const float Duration)
{
	// @todo maybe just freeze animation instead of freezing entire character since it might freeze additional effects like glow
	
	if (CanFreeze())
	{
		CustomTimeDilation = 0;
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &APlayerCharacter::EndFreeze, Duration, false);

		return true;
	}

	return false;
}

void APlayerCharacter::EndFreeze()
{
	CustomTimeDilation = GetStatsComponent()->GetActiveTimeDilation();
}

bool APlayerCharacter::Knockdown(const float Duration)
{
	/*
	if (CanKnockdown() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->AnimationMontage_HitEffects)
	{
		PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_HitEffects,
			UCharacterLibrary::SectionName_KnockdownStart,
			ECharacterState::GotHit);
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &APlayerCharacter::EndKnockdown, Duration, false);

		return true;
	}
	*/

	return false;
}

void APlayerCharacter::EndKnockdown()
{
	/*
	PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_HitEffects,
		UCharacterLibrary::SectionName_KnockdownEnd,
		ECharacterState::GotHit);
		*/
}

bool APlayerCharacter::Knockback(const float Duration, const FVector& ImpulseDirection)
{
	/*
	if (CanKnockdown() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->AnimationMontage_HitEffects)
	{
		PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_HitEffects,
			UCharacterLibrary::SectionName_KnockdownStart,
			ECharacterState::GotHit);
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &APlayerCharacter::EndKnockdown, Duration, false);
		PushPlayer(ImpulseDirection);

		return true;
	}
	*/

	return false;
}

void APlayerCharacter::BlockAttack()
{
	/*
	UKismetSystemLibrary::PrintString(this, FString("Blocked Attack"));
	PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_HitEffects,
						 UCharacterLibrary::SectionName_BlockAttack);
						 */
}

bool APlayerCharacter::CanAutoRun() const
{
	// The character can auto run only if character is in idle state
	// return CharacterState == ECharacterState::IdleWalkRun && GetVelocity().Size() == 0;
	return IsIdleOrMoving();
}

void APlayerCharacter::MoveForward(const float Value)
{
	if (Value != 0 && CanMove())
	{
		if (IsAutoRunning())
		{
			DisableAutoRun();
		}

		FRotator rot = FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f);
		FVector Direction = FRotationMatrix(rot).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(const float Value)
{
	if (Value != 0 && CanMove())
	{
		if (IsAutoRunning())
		{
			DisableAutoRun();
		}

		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::ZoomInCamera()
{
	if (CameraBoom->TargetArmLength >= CameraArmMinimumLength)
		CameraBoom->TargetArmLength -= CameraZoomRate;
}

void APlayerCharacter::ZoomOutCamera()
{
	if (CameraBoom->TargetArmLength <= CameraArmMaximumLength)
		CameraBoom->TargetArmLength += CameraZoomRate;
}

void APlayerCharacter::OnDodge()
{
	/*
	if (CanDodge() && PlayerAnimInstance && GetActiveAnimationReferences() && GetActiveAnimationReferences()->AnimationMontage_Dodge)
	{
		int32 DodgeCost = DodgeStaminaCost / StatsComp->GetStaminaConsumptionModifier();
		StatsComp->ModifyCurrentStamina(-DodgeCost);

		float ForwardAxisValue = InputComponent->GetAxisValue(FName("MoveForward"));
		float RightAxisValue = InputComponent->GetAxisValue(FName("MoveRight"));
		float DesiredPlayerRotationYaw = GetPlayerControlRotationYaw();

		if (ForwardAxisValue != 0)
		{
			DesiredPlayerRotationYaw = GetRotationYawFromAxisInput();
		}

		SetCharacterRotation(FRotator(0.f, DesiredPlayerRotationYaw, 0.f));

		if (ForwardAxisValue == 0)
		{
			if (RightAxisValue > 0)
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Dodge,
					UCharacterLibrary::SectionName_RightDodge,
					ECharacterState::Dodging);
			}
			else if (RightAxisValue < 0)
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Dodge,
					UCharacterLibrary::SectionName_LeftDodge,
					ECharacterState::Dodging);
			}
			else
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Dodge,
					UCharacterLibrary::SectionName_BackwardDodge,
					ECharacterState::Dodging);
			}
		}
		else
		{
			if (ForwardAxisValue > 0)
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Dodge,
					UCharacterLibrary::SectionName_ForwardDodge,
					ECharacterState::Dodging);
			}
			else if (ForwardAxisValue < 0)
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Dodge,
					UCharacterLibrary::SectionName_BackwardDodge,
					ECharacterState::Dodging);
			}
		}

		if (GetWorld()->GetTimerManager().IsTimerActive(DodgeTimerHandle))
		{
			DisableiFrames();
		}
		
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("EnableiFrames"), DodgeImmunityDuration);
		GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, TimerDelegate, DodgeImmunityTriggerDelay, false);
	}
	*/
}

void APlayerCharacter::OnPressedForward()
{
	bBackwardPressed = false;
	bForwardPressed = true;
	GetWorld()->GetTimerManager().SetTimer(SPAttackTimerHandle, this, &APlayerCharacter::DisableForwardPressed, 0.1f, false);
}

void APlayerCharacter::OnPressedBackward()
{
	bForwardPressed = false;
	bBackwardPressed = true;
	GetWorld()->GetTimerManager().SetTimer(SPAttackTimerHandle, this, &APlayerCharacter::DisableBackwardPressed, 0.1f, false);
}

void APlayerCharacter::OnReleasedForward()
{
	// GetWorld()->GetTimerManager().SetTimer()
}

void APlayerCharacter::OnReleasedBackward()
{
}

void APlayerCharacter::OnPressedBlock()
{
	bBlockPressed = true;
}

void APlayerCharacter::OnReleasedBlock()
{
	bBlockPressed = false;
}

void APlayerCharacter::OnPressedEscape()
{
	// @todo pause game, show cursor and display pause menu
}

void APlayerCharacter::EnableBlock()
{
	/*
	if (IsNormalAttacking())
	{
		PlayerAnimInstance->Montage_Stop(0.2, GetActiveAnimationReferences()->AnimationMontage_NormalAttacks);
	}

	SetCharacterState(ECharacterState::Blocking);
	SetUseControllerRotationYaw(true);
	SetWalkSpeed(BaseBlockMovementSpeed * StatsComp->GetMovementSpeedModifier());

	FTimerHandle TimerDelegate;
	GetWorld()->GetTimerManager().SetTimer(BlockTimerHandle, this, &APlayerCharacter::EnableDamageBlocking, DamageBlockTriggerDelay, false);
	*/
}

void APlayerCharacter::DisableBlock()
{
	SetUseControllerRotationYaw(false);
	SetCharacterState(ECharacterState::IdleWalkRun);
	DisableDamageBlocking();
}

void APlayerCharacter::OnJump()
{
	/*
	if (CanJump() && PlayerAnimInstance && GetActiveAnimationReferences())
	{
		if (IsBlocking())
		{
			DisableBlock();
		}

		Jump();
		PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Jump,
			UCharacterLibrary::SectionName_JumpStart,
			ECharacterState::Jumping);
	}
	*/
}

void APlayerCharacter::OnInteract()
{
	// @todo definition
}

void APlayerCharacter::OnToggleSheathe()
{
	bool bNewValue = !bWeaponSheathed;
	SetWeaponSheathed(bNewValue);
}

void APlayerCharacter::OnToggleCharacterStatsUI()
{

	// @todo definition
}

void APlayerCharacter::OnToggleMouseCursor()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC->bShowMouseCursor)
	{
		PC->bShowMouseCursor = false;
		FInputModeGameOnly GameOnlyInputMode;
		GameOnlyInputMode.SetConsumeCaptureMouseDown(true);
		PC->SetInputMode(GameOnlyInputMode);
	}
	else
	{
		PC->bShowMouseCursor = true;
		FInputModeGameAndUI GameAndUIInputMode;
		GameAndUIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		PC->SetInputMode(GameAndUIInputMode);
	}
}

void APlayerCharacter::OnToggleSkillTree()
{
	if (HUDWidget)
	{
		if (HUDWidget->SkillTreeWidget->IsVisible())
		{
			HUDWidget->SkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			HUDWidget->SkillTreeWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void APlayerCharacter::OnPressedNormalAttack()
{
	/*
	if (!CanNormalAttack() || !GetActiveAnimationReferences()->AnimationMontage_NormalAttacks)
	{
		return;
	}

	// @todo maybe change character state to using skill when using SP attacks?s
	if (!IsNormalAttacking() && bForwardPressed)
	{
		PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_NormalAttacks,
			UCharacterLibrary::SectionName_ForwardSPSwing,
			ECharacterState::Attacking);
		SetCharacterRotation(FRotator(0.f, GetPlayerControlRotationYaw(), 0.f));
	}
	else if (!IsNormalAttacking() && bBackwardPressed)
	{
		PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_NormalAttacks,
			UCharacterLibrary::SectionName_BackwardSPSwing,
			ECharacterState::Attacking);
		SetCharacterRotation(FRotator(0.f, GetPlayerControlRotationYaw(), 0.f));
	}
	else if (!IsNormalAttacking())
	{
		PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_NormalAttacks,
			UCharacterLibrary::SectionName_FirstSwing,
			ECharacterState::Attacking);
	}
	else if (IsNormalAttacking())
	{
		FName CurrentSection = PlayerAnimInstance->Montage_GetCurrentSection(GetActiveAnimationReferences()->AnimationMontage_NormalAttacks);
		FName NextSection = GetNextNormalAttackSectionName(CurrentSection);

		if (NextSection != NAME_None)
		{
			FString CurrentSectionString = CurrentSection.ToString();
			if (CurrentSectionString.EndsWith("End"))
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_NormalAttacks, NextSection, ECharacterState::Attacking);
			}
			else
			{
				SetNextMontageSection(CurrentSection, NextSection);
			}
		}
	}
	*/
}

void APlayerCharacter::OnReleasedNormalAttack()
{
	// empty 
}

void APlayerCharacter::OnToggleAutoRun()
{
	/*
	if (CharacterState == ECharacterState::AutoRun)
	{
		DisableAutoRun();
	}
	else if (CanAutoRun())
	{
		EnableAutoRun();
	}
	*/
}

FORCEINLINE void APlayerCharacter::EnableAutoRun()
{
	SetCharacterState(ECharacterState::AutoRun);
	SetUseControllerRotationYaw(true);
}

FORCEINLINE void APlayerCharacter::DisableAutoRun()
{
	// Make sure that auto run is active before you attempt to disable it
	// check(CharacterState == ECharacterState::AutoRun);

	SetCharacterState(ECharacterState::IdleWalkRun);
	SetUseControllerRotationYaw(false);
}

void APlayerCharacter::DisableForwardPressed()
{
	bForwardPressed = false;
}

void APlayerCharacter::DisableBackwardPressed()
{
	bBackwardPressed = false;
}

void APlayerCharacter::UpdateIdleState(float DeltaTime)
{
	if (IWR_CharacterMovementDirection != ECharMovementDirection::None)
	{
		SetIWRCharMovementDir(ECharMovementDirection::None);
	}
}

void APlayerCharacter::UpdateMovement(float DeltaTime)
{
	float ActorRotationYaw = GetActorRotation().Yaw;
	float DesiredPlayerRotationYaw = GetRotationYawFromAxisInput();
			
	bool bRotatePlayer = DesiredPlayerRotationYaw == ActorRotationYaw ? false : true;

	float ForwardAxisValue = InputComponent->GetAxisValue(FName("MoveForward"));
	float RightAxisValue = InputComponent->GetAxisValue(FName("MoveRight"));
	if (ForwardAxisValue == 0 && RightAxisValue == 0)
	{
		bRotatePlayer = false;
	}

	/*
	if (bRotatePlayer)
	{
		// DeltaRotatePlayerToDesiredYaw(DesiredPlayerRotationYaw, DeltaTime);
		DeltaRotateCharacterToDesiredYaw(DesiredPlayerRotationYaw, DeltaTime);
	}
	
	if (ForwardAxisValue < 0)
	{
		float Speed = (BaseNormalMovementSpeed * StatsComp->GetMovementSpeedModifier() * 5) / 16;
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			// SetWalkSpeed calls an RPC every frame and we want to avoid the overhead of RPC
			SetWalkSpeed(Speed);
		}
	}
	else
	{
		float Speed = BaseNormalMovementSpeed * StatsComp->GetMovementSpeedModifier();
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			// SetWalkSpeed calls an RPC every frame and we want to avoid the overhead of RPC
			SetWalkSpeed(Speed);
		}
	}
	*/
	if (ForwardAxisValue == 0)
	{
		if (RightAxisValue > 0 && IWR_CharacterMovementDirection != ECharMovementDirection::R)
		{
			SetIWRCharMovementDir(ECharMovementDirection::R);
		}
		else if (RightAxisValue < 0 && IWR_CharacterMovementDirection != ECharMovementDirection::L)
		{
			SetIWRCharMovementDir(ECharMovementDirection::L);
		}
	}
	else
	{
		if (ForwardAxisValue > 0 && IWR_CharacterMovementDirection != ECharMovementDirection::F)
		{
			SetIWRCharMovementDir(ECharMovementDirection::F);
		}
		else if (ForwardAxisValue < 0 && IWR_CharacterMovementDirection != ECharMovementDirection::B)
		{
			SetIWRCharMovementDir(ECharMovementDirection::B);
		}
	}
}

void APlayerCharacter::UpdateBlockState(float DeltaTime)
{
	float ForwardAxisValue = InputComponent->GetAxisValue(FName("MoveForward"));
	float RightAxisValue = InputComponent->GetAxisValue(FName("MoveRight"));
	
	if (ForwardAxisValue == 0)
	{
		if (RightAxisValue > 0)
		{
			if (BlockMovementDirectionYaw != 90.f)
				SetBlockMovementDirectionYaw(90.f);
		}
		else if (RightAxisValue < 0)
		{
			if (BlockMovementDirectionYaw != -90.f)
				SetBlockMovementDirectionYaw(-90.f);
		}
		else
		{
			if (BlockMovementDirectionYaw != 0.f)
				SetBlockMovementDirectionYaw(0.f);
		}
	}
	else
	{
		float NewYaw = FMath::RadiansToDegrees(FMath::Atan2(RightAxisValue, ForwardAxisValue));
		
		if (BlockMovementDirectionYaw != NewYaw)
		{
			SetBlockMovementDirectionYaw(NewYaw);
		}
	}
}

void APlayerCharacter::UpdateAutoRun(float DeltaTime)
{
	if (Controller && Controller->IsLocalPlayerController())
	{
		FRotator rot = FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f);
		FVector Direction = FRotationMatrix(rot).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, 1.f);
	}

	if (IWR_CharacterMovementDirection != ECharMovementDirection::F)
	{
		SetIWRCharMovementDir(ECharMovementDirection::F);
	}
	
	/*
	float Speed = BaseNormalMovementSpeed * StatsComp->GetMovementSpeedModifier();
	if (GetCharacterMovement()->MaxWalkSpeed != Speed)
	{
		SetWalkSpeed(Speed);
	}
	*/
}

/*
void APlayerCharacter::OnMeleeCollision(UAnimSequenceBase * Animation, TArray<FHitResult>& HitResults, bool bHit)
{
	Super::OnMeleeCollision(Animation, HitResults, bHit);
	//////
	// @note intentionally commented out. If player animation references are null when a collision event is triggered, we want it to crash.
	if (!PlayerAnimationReferences || !(Animation == PlayerAnimationReferences->AnimationMontage_NormalAttacks || 
										Animation == PlayerAnimationReferences->AnimationMontage_Skills || 
										Animation == PlayerAnimationReferences->AnimationMontage_Spells))
	{
		return;
	}
	///////

	bool bEnemiesHit = false;
	FSkill* ActiveSkill = GetCurrentActiveSkill();

#if DEVSTAGE_CODE_ENABLED
	if (!ActiveSkill)
	{
		return;
	}
#endif // DEVSTAGE_CODE_ENABLED

	check(ActiveSkill);

	for (FHitResult& HitResult : HitResults)
	{
		if (!HitResult.Actor.Get())
		{
			continue;
		}

		AEODCharacterBase* HitCharacter = Cast<AEODCharacterBase>(HitResult.Actor.Get());
		
		if (!HitCharacter || (HitCharacter->IsDodgingDamage() && !ActiveSkill->SkillLevelUpInfo.bUndodgable))
		{
			// @todo handle damage for non AEODCharacterBase actors
			continue;
		}

		bEnemiesHit = true;

		TArray<FHitResult> LineHitResults;
		FVector LineStart = GetActorLocation();
		FVector LineEnd = FVector(HitCharacter->GetActorLocation().X, HitCharacter->GetActorLocation().Y, LineStart.Z);

		FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(this);
		GetWorld()->LineTraceMultiByChannel(LineHitResults, LineStart, LineEnd, COLLISION_COMBAT, Params);

		FHitResult LineHitResultToHitCharacter;
		bool bLineHitResultFound = false;

		for (FHitResult& LineHitResult : LineHitResults)
		{
			if (LineHitResult.Actor.Get() && LineHitResult.Actor.Get() == HitCharacter)
			{
				LineHitResultToHitCharacter = LineHitResult;
				bLineHitResultFound = true;
				break;
			}
		}

		if (bLineHitResultFound)
		{
			FVector Start = LineHitResultToHitCharacter.ImpactPoint;
			FVector End = LineHitResultToHitCharacter.ImpactPoint + LineHitResultToHitCharacter.ImpactNormal * 50;
			UKismetSystemLibrary::DrawDebugArrow(this, Start, End, 200, FLinearColor::White, 5.f, 2.f);
		}

		/////// 
		FEODDamage EODDamage(ActiveSkill);
		EODDamage.CapsuleHitResult = HitResult;
		EODDamage.LineHitResult = LineHitResultToHitCharacter;
		ApplyEODDamage(HitCharacter, EODDamage);
		//////
	}

	if (!bEnemiesHit)
	{
		OnUnsuccessfulHit.Broadcast(TArray<TWeakObjectPtr<AEODCharacterBase>>());
	}
}
*/

/*
int32 APlayerCharacter::ApplyEODDamage(FEODDamage& EODDamage)
{
	return Super::ApplyEODDamage(EODDamage);
}
*/

void APlayerCharacter::Destroyed()
{
	Super::Destroyed();

	if (PrimaryWeapon)
	{
		// PrimaryWeapon->SetOwningCharacter(nullptr);
		PrimaryWeapon->OnUnEquip();
		PrimaryWeapon->Destroy();
	}
	if (SecondaryWeapon)
	{
		// SecondaryWeapon->SetOwningCharacter(nullptr);
		SecondaryWeapon->OnUnEquip();
		SecondaryWeapon->Destroy();
	}


}

FORCEINLINE void APlayerCharacter::SavePlayerState()
{
	if (HUDWidget)
	{
		HUDWidget->SaveHUDLayout();
	}
}

void APlayerCharacter::BP_SavePlayerState()
{
	SavePlayerState();
}

void APlayerCharacter::UpdateEquippedWeaponAnimationReferences(const EWeaponType EquippedWeaponType)
{
	if (EquippedWeaponAnimationReferences)
	{
		// UCharacterLibrary::UnloadPlayerAnimationReferences(EquippedWeaponAnimationReferences, Gender);

		// delete older animation references, prevent memory leak
		delete EquippedWeaponAnimationReferences;
		EquippedWeaponAnimationReferences = nullptr;
	}

	EWeaponAnimationType WeaponAnimationType;
	switch (EquippedWeaponType)
	{
	case EWeaponType::GreatSword:
		WeaponAnimationType = EWeaponAnimationType::GreatSword;
		break;
	case EWeaponType::WarHammer:
		WeaponAnimationType = EWeaponAnimationType::WarHammer;
		break;
	case EWeaponType::LongSword:
		WeaponAnimationType = EWeaponAnimationType::ShieldAndSword;
		break;
	case EWeaponType::Mace:
		WeaponAnimationType = EWeaponAnimationType::ShieldAndMace;
		break;
	case EWeaponType::Dagger:
		WeaponAnimationType = EWeaponAnimationType::Daggers;
		break;
	case EWeaponType::Staff:
		WeaponAnimationType = EWeaponAnimationType::Staff;
		break;
	case EWeaponType::Shield:
	case EWeaponType::None:
	default:
		WeaponAnimationType = EWeaponAnimationType::NoWeapon;
		break;
	}

	SetCurrentWeaponAnimationToUse(WeaponAnimationType);
	// EquippedWeaponAnimationReferences = UCharacterLibrary::GetPlayerAnimationReferences(WeaponAnimationType, Gender);
}

/*
FPlayerAnimationReferences * APlayerCharacter::GetActiveAnimationReferences() const
{
	return bWeaponSheathed ? SheathedWeaponAnimationReferences : EquippedWeaponAnimationReferences;
}
*/

FName APlayerCharacter::GetNextNormalAttackSectionName(const FName & CurrentSection) const
{
	if (CurrentSection == UCharacterLibrary::SectionName_FirstSwing ||
		CurrentSection == UCharacterLibrary::SectionName_FirstSwingEnd)
	{
		return UCharacterLibrary::SectionName_SecondSwing;
	}
	else if (CurrentSection == UCharacterLibrary::SectionName_SecondSwing ||
			 CurrentSection == UCharacterLibrary::SectionName_SecondSwingEnd)
	{
		return UCharacterLibrary::SectionName_ThirdSwing;
	}
	else if (CurrentSection == UCharacterLibrary::SectionName_ThirdSwing ||
	 		 CurrentSection == UCharacterLibrary::SectionName_ThirdSwingEnd)
	{
		if (CurrentWeaponAnimationToUse == EWeaponAnimationType::GreatSword ||
			CurrentWeaponAnimationToUse == EWeaponAnimationType::WarHammer)
		{
			return NAME_None;
		}
		else
		{
			return UCharacterLibrary::SectionName_FourthSwing;
		}
	}
	else if (CurrentSection == UCharacterLibrary::SectionName_FourthSwing ||
			 CurrentSection == UCharacterLibrary::SectionName_FourthSwingEnd)
	{
		if (CurrentWeaponAnimationToUse == EWeaponAnimationType::Staff)
		{
			return NAME_None;
		}
		else
		{
			return UCharacterLibrary::SectionName_FifthSwing;
		}
	}

	return NAME_None;
}

FPlayerAnimationReferencesTableRow* APlayerCharacter::GetActiveAnimationReferences() const
{
	return nullptr;
}

FName APlayerCharacter::GetAnimationReferencesRowID(EWeaponType WeaponType, ECharacterGender Gender)
{
	FString Prefix;
	if (Gender == ECharacterGender::Female)
	{
		Prefix = FString("Female_");
	}
	else
	{
		Prefix = FString("Male_");
	}

	FString Postfix;
	switch (WeaponType)
	{
	case EWeaponType::GreatSword:
		Postfix = FString("GreatSword");
		break;
	case EWeaponType::WarHammer:
		Postfix = FString("WarHammer");
		break;
	case EWeaponType::LongSword:
		Postfix = FString("LongSword");
		break;
	case EWeaponType::Mace:
		Postfix = FString("Mace");
		break;
	case EWeaponType::Dagger:
		Postfix = FString("Dagger");
		break;
	case EWeaponType::Staff:
		Postfix = FString("Staff");
		break;
	case EWeaponType::Shield:
		Postfix = FString("Shield");
		break;
	case EWeaponType::None:
		Postfix = FString("NoWeapon");
		break;
	default:
		Postfix = FString("NoWeapon");
		break;
	}

	FString RowIDString = Prefix + Postfix;
	FName RowID = FName(*RowIDString);

	return RowID;
}

TSharedPtr<FStreamableHandle> APlayerCharacter::LoadAnimationReferences(FPlayerAnimationReferencesTableRow* AnimationReferences)
{
	TSharedPtr<FStreamableHandle> StreamableHandle;

	UGameSingleton* GameSingleton = nullptr;
	if (GEngine)
	{
		GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
	}

	if (!GameSingleton)
	{
		return StreamableHandle;
	}

	TArray<FSoftObjectPath> AssetsToLoad;
	if (AnimationReferences)
	{
		AssetsToLoad.Add(AnimationReferences->BlockAttack.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->Die.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->Dodge.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->Flinch.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->HitEffects.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->NormalAttacks.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->Skills.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->Jump.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->SpecialActions.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->Spells.ToSoftObjectPath());
	}

	StreamableHandle = GameSingleton->StreamableManager.RequestSyncLoad(AssetsToLoad);
	return StreamableHandle;
}

void APlayerCharacter::UnloadUnequippedWeaponAnimationReferences()
{
	if (UnequippedWeaponAnimationsStreamableHandle.IsValid())
	{
		UnequippedWeaponAnimationsStreamableHandle.Get()->ReleaseHandle();
		UnequippedWeaponAnimationsStreamableHandle.Reset();
	}
}

void APlayerCharacter::LoadUnequippedWeaponAnimationReferences()
{
	UnloadUnequippedWeaponAnimationReferences();

	FName RowID = GetAnimationReferencesRowID(EWeaponType::None, Gender);
	FPlayerAnimationReferencesTableRow* PlayerAnimationReferences = PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(RowID,
		FString("APlayerCharacter::LoadUnequippedWeaponAnimationReferences(), loading unequipped weapon animation references"));

	if (!PlayerAnimationReferencesDataTable)
	{
		return;
	}

	UnequippedWeaponAnimationsStreamableHandle = LoadAnimationReferences(PlayerAnimationReferences);
}

void APlayerCharacter::UnloadEquippedWeaponAnimationReferences()
{
	if (EquippedWeaponAnimationsStreamableHandle.IsValid())
	{
		EquippedWeaponAnimationsStreamableHandle.Get()->ReleaseHandle();
		EquippedWeaponAnimationsStreamableHandle.Reset();
	}
}

void APlayerCharacter::LoadEquippedWeaponAnimationReferences()
{
	UnloadEquippedWeaponAnimationReferences();

	FName RowID = GetAnimationReferencesRowID(GetEquippedWeaponType(), Gender);
	FPlayerAnimationReferencesTableRow* PlayerAnimationReferences = PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(RowID,
		FString("APlayerCharacter::LoadUnequippedWeaponAnimationReferences(), loading unequipped weapon animation references"));

	if (!PlayerAnimationReferencesDataTable)
	{
		return;
	}

	UnequippedWeaponAnimationsStreamableHandle = LoadAnimationReferences(PlayerAnimationReferences);
}

void APlayerCharacter::OnPressingSkillKey(const uint32 SkillButtonIndex)
{
	/*
	if (!CanUseAnySkill())
	{
		return;
	}

	FName SkillID = SkillBarComponent->GetSkillIDFromSkillSlot(SkillButtonIndex);
	if (SkillID == NAME_None)
	{
		return;
	}

	FPlayerSkillTableRow* SkillToUse = GetSkill(SkillID);
	if (!SkillToUse)
	{
		return;
	}

	*/

	/*
	if (!CanUseSkill(SkillToUse))
	{
		return;
	}
	*/

	/*
	StatsComp->ModifyCurrentMana(-SkillToUse->ManaRequired);
	StatsComp->ModifyCurrentStamina(-SkillToUse->StaminaRequired);

	if (SkillToUse->AnimMontage.IsNull())
	{

	}
	else
	{

	}

	SetCurrentActiveSkillID(SkillID);
	SkillBarComponent->OnSkillUsed(SkillID, SkillToUse);

	*/

	/*
	// Skill is in cooldown, can't use skill. @note Empty skill slot will return false
	if (HUDWidget->IsSkillInCooldown(SkillButtonIndex))
	{
		return;
	}

	FName SkillID = HUDWidget->GetSkillAtIndex(SkillButtonIndex);
	// If no skill is equipped in the given slot
	if (SkillID == NAME_None)
	{
		return;
	}

	// @note no need to check for active or passive skill since only active skills can be placed in skill bar slot.
	// In fact, only active skills can be dragged out of skill tree widget.

	FPlayerSkillTableRow* Skill = GetSkill(SkillID, FString("APlayerCharacter::OnPressingSkillKey(), looking for player skill"));

	if (!Skill)
	{
		UKismetSystemLibrary::PrintString(this, FString("Couldn't find skill"));
		return;
	}

	if (Skill->AnimMontage.IsNull())
	{
		UKismetSystemLibrary::PrintString(this, FString("Anim montage of skill is null"));
		return;
	}
	else if (Skill->AnimMontage.IsPending())
	{
		double Start = FPlatformTime::Seconds();
		Skill->AnimMontage.LoadSynchronous();
		double Stop = FPlatformTime::Seconds();

		double Time = (Stop - Start) * 1000000;
		FString Message = FString("Time to load animation: ") + FString::SanitizeFloat(Time);

		UKismetSystemLibrary::PrintString(this, Message);
	}
	else if (Skill->AnimMontage.IsValid())
	{

	}

	SetCharacterRotation(FRotator(0.f, GetPlayerControlRotationYaw(), 0.f));
	PlayAnimationMontage(Skill->AnimMontage.Get(), Skill->SkillStartMontageSectionName, ECharacterState::UsingActiveSkill);
	SetCurrentActiveSkillID(SkillID);
	*/

	/*

	// @attention
	// CurrentActiveSkill = Skill;

	HUDWidget->PutSkillOnCooldownTimer(SkillButtonIndex, Skill->SkillLevelUpInfo.Cooldown, 1.f);
	PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Skills, Skill->SkillStartMontageSectionName, ECharacterState::UsingActiveSkill);
	// @todo set current active skill

	// Check if skill can be used with currently equipped weapon

	// Trigger status effects on using this skill? 


	// HUDWidget->PutSkillOnCooldownTimer()
	*/
}

void APlayerCharacter::OnReleasingSkillKey(const uint32 SkillButtonIndex)
{
	/*
	if (!SkillBarWidget)
	{
		return;
	}

	SkillBarWidget->ReleaseSkillButton(SkillButtonIndex);
	*/
}

float APlayerCharacter::GetPlayerControlRotationYaw()
{
	float ControlRotationYaw = Controller->GetControlRotation().Yaw;

	if (0 <= ControlRotationYaw && ControlRotationYaw <= 180)
		return ControlRotationYaw;
	else if (180 < ControlRotationYaw && ControlRotationYaw < 360)
	{
		return (ControlRotationYaw - 360.f);
	}
	else if (ControlRotationYaw == 360)
		return 0.f;
	else
		return ControlRotationYaw;
}

float APlayerCharacter::GetRotationYawFromAxisInput()
{
	float ForwardAxisValue = InputComponent->GetAxisValue(FName("MoveForward"));
	float RightAxisValue = InputComponent->GetAxisValue(FName("MoveRight"));
	
	float ControlRotationYaw = GetPlayerControlRotationYaw();
	float ResultingRotation = 0.f;

	if (ForwardAxisValue == 0)
	{
		if (RightAxisValue > 0)
		{
			ResultingRotation = ControlRotationYaw + 90.f;
		}
		else if (RightAxisValue < 0)
		{
			ResultingRotation = ControlRotationYaw - 90.f;
		}
	}
	else
	{
		if (ForwardAxisValue > 0)
		{
			float DeltaAngle = FMath::RadiansToDegrees(FMath::Atan2(RightAxisValue, ForwardAxisValue));
			ResultingRotation = ControlRotationYaw + DeltaAngle;
		}
		else if (ForwardAxisValue < 0)
		{
			float DeltaAngle = FMath::RadiansToDegrees(FMath::Atan2(-RightAxisValue, -ForwardAxisValue));
			ResultingRotation = ControlRotationYaw + DeltaAngle;
		}
	}

	return ResultingRotation;
}

/*
FEODDamageResult APlayerCharacter::ApplyEODDamage(AEODCharacterBase * InstigatingChar, const FEODDamage & EODDamage, const FHitResult & CollisionHitResult)
{
	FEODDamageResult EODDamageResult;
	TArray<TWeakObjectPtr<AEODCharacterBase>> WeakPtrsCharArray;
	TWeakObjectPtr<AEODCharacterBase> WeakPtrToInstigatingChar(InstigatingChar);
	WeakPtrsCharArray.Add(WeakPtrToInstigatingChar);

	// If character is dodging and incoming attack is dodgable
	if (IsDodgingDamage() && !EODDamage.bUndodgable)
	{
		// Trigger OnSuccessfulDodge event
		OnSuccessfulDodge.Broadcast(WeakPtrsCharArray);

		DisplayStatusMessage(FString("Dodge"));

		EODDamageResult.CharacterResponseToDamage = ECharacterResponseToDamage::Dodged;
		EODDamageResult.ActualDamage = 0;
		return EODDamageResult;
	}


	TArray<FHitResult> LineHitResults;
	FVector LineStart = InstigatingChar->GetActorLocation();
	float LineEnd_Z;
	if (GetActorLocation().Z < LineStart.Z)
	{
		LineEnd_Z = GetActorLocation().Z;
	}
	else
	{
		LineEnd_Z = LineStart.Z;
	}
	FVector LineEnd = FVector(GetActorLocation().X, GetActorLocation().Y, LineEnd_Z);

	FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(InstigatingChar);
	GetWorld()->LineTraceMultiByChannel(LineHitResults, LineStart, LineEnd, COLLISION_COMBAT, Params);

	FHitResult InstigatorToThisCharLineHitResult;

	for (FHitResult& LineHitResult : LineHitResults)
	{
		if (LineHitResult.Actor.Get() && LineHitResult.Actor.Get() == this)
		{
			InstigatorToThisCharLineHitResult = LineHitResult;
			break;
		}
	}

#if DEVSTAGE_CODE_ENABLED
	FVector Start = InstigatorToThisCharLineHitResult.ImpactPoint;
	FVector End = InstigatorToThisCharLineHitResult.ImpactPoint + InstigatorToThisCharLineHitResult.ImpactNormal * 50;
	UKismetSystemLibrary::DrawDebugArrow(this, Start, End, 200, FLinearColor::Blue, 5.f, 2.f);
#endif

	//~ @todo any incoming crit rate reduction logic
	bool bCriticalHit = EODDamage.CritRate >= FMath::RandRange(0.f, 100.f) ? true : false;
	EODDamageResult.bCritHit = bCriticalHit;
	if (bCriticalHit)
	{
		EODDamageResult.ActualDamage = EODDamage.CritDamage;
	}
	else
	{
		EODDamageResult.ActualDamage = EODDamage.NormalDamage;
	}
	int32 Resistance;
	if (EODDamage.DamageType == EDamageType::Magickal)
	{
		Resistance = StatsComp->GetMagickResistance();
	}
	else
	{
		Resistance = StatsComp->GetPhysicalResistance();
	}

	// If character is blocking and incoming damage is blockable
	if (IsBlockingDamage() && !EODDamage.bUnblockable)
	{
		FVector MyDirection = GetActorForwardVector();
		FVector HitNormal = InstigatorToThisCharLineHitResult.ImpactNormal;

		float Angle = UEODBlueprintFunctionLibrary::CalculateAngleBetweenVectors(MyDirection, HitNormal);

		if (Angle < 60)
		{
			// Trigger OnSuccessfulBlock event
			OnSuccessfulBlock.Broadcast(WeakPtrsCharArray);

			float DamageReductionOnBlock;
			if (EODDamage.DamageType == EDamageType::Magickal)
			{
				DamageReductionOnBlock = StatsComp->GetMagickDamageReductionOnBlock();
			}
			else
			{
				DamageReductionOnBlock = StatsComp->GetPhysicalDamageReductionOnBlock();
			}

			EODDamageResult.ActualDamage = EODDamageResult.ActualDamage * (1 - DamageReductionOnBlock);
			EODDamageResult.ActualDamage = UCombatLibrary::CalculateDamage(EODDamageResult.ActualDamage, Resistance);
			EODDamageResult.CharacterResponseToDamage = ECharacterResponseToDamage::Blocked;

			// Apply real damage
			StatsComp->ModifyCurrentHealth(-EODDamageResult.ActualDamage);

			return EODDamageResult;
		}
	}

	switch (EODDamage.CrowdControlEffect)
	{
	case ECrowdControlEffect::Flinch:
		if (CanFlinch())
		{
			FVector MyDirection = GetActorForwardVector();
			FVector HitNormal = InstigatorToThisCharLineHitResult.ImpactNormal;

			float Angle = UEODBlueprintFunctionLibrary::CalculateAngleBetweenVectors(MyDirection, HitNormal);

			if (Angle <= 90)
			{
				Flinch(EHitDirection::Forward);
			}
			else
			{
				Flinch(EHitDirection::Backward);
			}
		}
		break;
	case ECrowdControlEffect::Interrupt:
		if (CanInterrupt())
		{
			FVector MyDirection = GetActorForwardVector();
			FVector HitNormal = InstigatorToThisCharLineHitResult.ImpactNormal;

			float Angle = UEODBlueprintFunctionLibrary::CalculateAngleBetweenVectors(MyDirection, HitNormal);

			if (Angle <= 90)
			{
				Interrupt(EHitDirection::Forward);
			}
			else
			{
				Interrupt(EHitDirection::Backward);
			}
		}
		break;
	case ECrowdControlEffect::KnockedDown:
		if (CanKnockdown())
		{
			Knockdown(EODDamage.CrowdControlEffectDuration);
		}
		break;
	case ECrowdControlEffect::KnockedBack:
		if (CanKnockback())
		{
			FVector ImpulseDirection = -(InstigatorToThisCharLineHitResult.ImpactNormal);
			Knockback(EODDamage.CrowdControlEffectDuration, ImpulseDirection);
		}
		break;
	case ECrowdControlEffect::Stunned:
		if (CanStun())
		{
			Stun(EODDamage.CrowdControlEffectDuration);
		}
		break;
	case ECrowdControlEffect::Crystalized:
		if (CanFreeze())
		{
			Freeze(EODDamage.CrowdControlEffectDuration);
		}
		break;
	default:
		break;
	}

	// Trigger OnReceivingHit event
	OnReceivingHit.Broadcast(WeakPtrsCharArray);
	EODDamageResult.ActualDamage = UCombatLibrary::CalculateDamage(EODDamageResult.ActualDamage, Resistance);
	StatsComp->ModifyCurrentHealth(-EODDamageResult.ActualDamage);

	DisplayReceivedDamage(EODDamageResult.ActualDamage);

	return EODDamageResult;
}
*/

void APlayerCharacter::OnMontageBlendingOut(UAnimMontage * AnimMontage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		// CharacterState = ECharacterState::IdleWalkRun;
	}
}

void APlayerCharacter::OnMontageEnded(UAnimMontage * AnimMontage, bool bInterrupted)
{
}

void APlayerCharacter::SetCurrentPrimaryWeapon(const FName WeaponID)
{
	if (WeaponID == NAME_None)
	{
		RemovePrimaryWeapon();
		return;
	}

	FWeaponTableRow* WeaponData = UWeaponLibrary::GetWeaponData(WeaponID);
	// If it's an invalid weapon
	if (!WeaponData || WeaponData->WeaponMesh.IsNull())
	{
		return;
	}

	RemovePrimaryWeapon();
	if (UWeaponLibrary::IsWeaponDualHanded(WeaponData->WeaponType))
	{
		RemoveSecondaryWeapon();
	}
	PrimaryWeaponID = WeaponID;
	PrimaryWeapon->OnEquip(WeaponID, WeaponData);
	UpdateCurrentWeaponAnimationType();
}

void APlayerCharacter::SetCurrentSecondaryWeapon(const FName WeaponID)
{
	if (WeaponID == NAME_None)
	{
		RemoveSecondaryWeapon();
		return;
	}

	FWeaponTableRow* WeaponData = UWeaponLibrary::GetWeaponData(WeaponID);
	// If it's an invalid weapon
	if (!WeaponData || WeaponData->WeaponMesh.IsNull())
	{
		return;
	}

	// Since secondary weapon is guaranteed to be single handed
	RemoveSecondaryWeapon();
	if (UWeaponLibrary::IsWeaponDualHanded(PrimaryWeapon->WeaponType))
	{
		RemovePrimaryWeapon();
	}
	SecondaryWeaponID = WeaponID;
	SecondaryWeapon->OnEquip(WeaponID, WeaponData);
}

void APlayerCharacter::RemovePrimaryWeapon()
{
	PrimaryWeaponID = NAME_None;
	PrimaryWeapon->OnUnEquip();
}

void APlayerCharacter::RemoveSecondaryWeapon()
{
	SecondaryWeaponID = NAME_None;
	SecondaryWeapon->OnUnEquip();
}

void APlayerCharacter::UpdateCurrentWeaponAnimationType()
{
	if (bWeaponSheathed)
	{
		if (CurrentWeaponAnimationToUse != EWeaponAnimationType::SheathedWeapon)
		{
			SetCurrentWeaponAnimationToUse(EWeaponAnimationType::SheathedWeapon);
		}

		return;
	}

	if (IsPrimaryWeaponEquippped())
	{
		switch (PrimaryWeapon->WeaponType)
		{
		case EWeaponType::GreatSword:
			SetCurrentWeaponAnimationToUse(EWeaponAnimationType::GreatSword);
			break;
		case EWeaponType::WarHammer:
			SetCurrentWeaponAnimationToUse(EWeaponAnimationType::WarHammer);
			break;
		case EWeaponType::LongSword:
			SetCurrentWeaponAnimationToUse(EWeaponAnimationType::ShieldAndSword);
			break;
		case EWeaponType::Mace:
			SetCurrentWeaponAnimationToUse(EWeaponAnimationType::ShieldAndMace);
			break;
		case EWeaponType::Dagger:
			SetCurrentWeaponAnimationToUse(EWeaponAnimationType::Daggers);
			break;
		case EWeaponType::Staff:
			SetCurrentWeaponAnimationToUse(EWeaponAnimationType::Staff);
			break;
		default:
			// SetCurrentWeaponAnimationToUse(EWeaponAnimationType::NoWeapon);
			break;
		}
	}
	else if (IsSecondaryWeaponEquipped())
	{
		if (SecondaryWeapon->WeaponType == EWeaponType::Dagger)
		{
			SetCurrentWeaponAnimationToUse(EWeaponAnimationType::Daggers);
		}
		else
		{
			SetCurrentWeaponAnimationToUse(EWeaponAnimationType::NoWeapon);
		}
	}
	else
	{
		SetCurrentWeaponAnimationToUse(EWeaponAnimationType::NoWeapon);
	}
}

void APlayerCharacter::TurnOnTargetSwitch()
{
	/*
	Super::TurnOnTargetSwitch();

	Hair->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 1.f);
	HatItem->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 1.f);
	FaceItem->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 1.f);
	Chest->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 1.f);
	Hands->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 1.f);
	Legs->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 1.f);
	Feet->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 1.f);
	*/
}

void APlayerCharacter::TurnOffTargetSwitch()
{
	/*
	Super::TurnOffTargetSwitch();

	Hair->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 0.f);
	HatItem->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 0.f);
	FaceItem->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 0.f);
	Chest->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 0.f);
	Hands->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 0.f);
	Legs->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 0.f);
	Feet->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 0.f);
	*/
}

bool APlayerCharacter::IsPrimaryWeaponEquippped() const
{
	return PrimaryWeaponID != NAME_None && PrimaryWeapon->bEquipped;
}

bool APlayerCharacter::IsSecondaryWeaponEquipped() const
{
	return SecondaryWeaponID != NAME_None && SecondaryWeapon->bEquipped;
}

bool APlayerCharacter::IsFastRunning() const
{
	// return CharacterState == ECharacterState::SpecialMovement;
	return false;
}

/*
FORCEINLINE bool APlayerCharacter::CanUseSkill(const FPlayerSkillTableRow* Skill)
{
	if (Skill)
	{
		if ((Skill->SupportedWeapons & (1 << (uint8)GetEquippedWeaponType())) &&
			StatsComp->GetCurrentMana() >= Skill->ManaRequired &&
			StatsComp->GetCurrentStamina() >= Skill->StaminaRequired)
		{
			return true;
		}
	}

	return false;
}
*/

FORCEINLINE void APlayerCharacter::SetIWRCharMovementDir(ECharMovementDirection NewDirection)
{
	IWR_CharacterMovementDirection = NewDirection;

	if (Role < ROLE_Authority)
	{
		Server_SetIWRCharMovementDir(NewDirection);
	}
}

void APlayerCharacter::Server_SetIWRCharMovementDir_Implementation(ECharMovementDirection NewDirection)
{
	SetIWRCharMovementDir(NewDirection);
}

bool APlayerCharacter::Server_SetIWRCharMovementDir_Validate(ECharMovementDirection NewDirection)
{
	return true;
}

void APlayerCharacter::SetCurrentWeaponAnimationToUse(EWeaponAnimationType NewWeaponAnimationType)
{
	// UpdateNormalAttackSectionToSkillMap(NewWeaponAnimationType, CurrentWeaponAnimationToUse);
	CurrentWeaponAnimationToUse = NewWeaponAnimationType;
	
	
	/*
	if (Role < ROLE_Authority)
	{
		Server_SetCurrentWeaponAnimationToUse(NewWeaponAnimationType);
	}
	*/
}

void APlayerCharacter::Server_SetCurrentWeaponAnimationToUse_Implementation(EWeaponAnimationType NewWeaponAnimationType)
{
	SetCurrentWeaponAnimationToUse(NewWeaponAnimationType);
}

bool APlayerCharacter::Server_SetCurrentWeaponAnimationToUse_Validate(EWeaponAnimationType NewWeaponAnimationType)
{
	return true;
}

void APlayerCharacter::SetBlockMovementDirectionYaw(float NewYaw)
{
	BlockMovementDirectionYaw = NewYaw;

	if (Role < ROLE_Authority)
	{
		Server_SetBlockMovementDirectionYaw(NewYaw);
	}
}

void APlayerCharacter::Server_SetBlockMovementDirectionYaw_Implementation(float NewYaw)
{
	SetBlockMovementDirectionYaw(NewYaw);
}

bool APlayerCharacter::Server_SetBlockMovementDirectionYaw_Validate(float NewYaw)
{
	return true;
}

void APlayerCharacter::SetWeaponSheathed(bool bNewValue)
{
	bWeaponSheathed = bNewValue;
	UpdateCurrentWeaponAnimationType();

	if (Role < ROLE_Authority)
	{
		Server_SetWeaponSheathed(bNewValue);
	}
}

void APlayerCharacter::AddSkill(FName SkillID, uint8 SkillLevel)
{
	/*
	FSkill* Skill = UCharacterLibrary::GetPlayerSkill(SkillID, SkillLevel);
	if (Skill)
	{
		IDToSkillMap.Add(SkillID, Skill);
		UKismetSystemLibrary::PrintString(this, FString("Added skill"), true, false);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, FString("Failed to add"), true, false);
	}
	*/
}

/*
FORCEINLINE FPlayerSkillTableRow * APlayerCharacter::GetSkill(FName SkillID,  const FString& ContextString) const
{
	return UCharacterLibrary::GetPlayerSkill(SkillID, ContextString);
}

FORCEINLINE void APlayerCharacter::SetCurrentActivePlayerSkill(FPlayerSkillTableRow* Skill)
{
	// CurrentActivePlayerSkill = Skill;
}

FORCEINLINE FPlayerSkillTableRow * APlayerCharacter::GetCurrentActivePlayerSkill() const
{
	// return CurrentActivePlayerSkill;
	return nullptr;
}
*/

/*
FSkillDamageInfo APlayerCharacter::GetCurrentActiveSkillDamageInfo() const
{
	// @todo check for null

	FSkillDamageInfo SkillDamageInfo;
	const FPlayerSkillTableRow* Skill = GetCurrentActivePlayerSkill();
	SkillDamageInfo.bUnblockable = Skill->bUnblockable;
	SkillDamageInfo.bUndodgable = Skill->bUndodgable;
	SkillDamageInfo.bIgnoresBlock = Skill->bIgnoresBlock;
	SkillDamageInfo.CrowdControlEffect = Skill->CrowdControlEffect;
	SkillDamageInfo.CrowdControlEffectDuration = Skill->CrowdControlEffectDuration;
	SkillDamageInfo.DamagePercent = Skill->DamagePercent;
	SkillDamageInfo.DamageType = Skill->DamageType;

	return SkillDamageInfo;
}

*/

void APlayerCharacter::OnNormalAttackSectionStart(FName SectionName)
{
	// @attention
	// CurrentActiveSkill = NormalAttackSectionToSkillMap[SectionName];
}

void APlayerCharacter::CleanupNormalAttackSectionToSkillMap()
{
	/*
	TArray<FName> Keys;
	NormalAttackSectionToSkillMap.GetKeys(Keys);
	for (FName& Key : Keys)
	{
		FSkill* Skill = NormalAttackSectionToSkillMap[Key];
		delete Skill;
	}
	NormalAttackSectionToSkillMap.Empty();
	*/
}

void APlayerCharacter::UpdateNormalAttackSectionToSkillMap(EWeaponType NewWeaponType)
{
	/*
	CleanupNormalAttackSectionToSkillMap();

	if (NewWeaponType == EWeaponType::GreatSword ||
		NewWeaponType == EWeaponType::WarHammer)
	{
		FSkill* Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 100;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_FirstSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 100;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_SecondSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 200;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_ThirdSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 150;
		Skill->SkillLevelUpInfo.StaminaRequired = 10;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_ForwardSPSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 150;
		Skill->SkillLevelUpInfo.StaminaRequired = 10;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_BackwardSPSwing, Skill);
	}
	else if (NewWeaponType == EWeaponType::Staff)
	{
		FSkill* Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 100;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_FirstSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 100;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_SecondSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 120;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_ThirdSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 150;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_FourthSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 125;
		Skill->SkillLevelUpInfo.StaminaRequired = 10;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_ForwardSPSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 125;
		Skill->SkillLevelUpInfo.StaminaRequired = 10;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_BackwardSPSwing, Skill);
	}
	else if (NewWeaponType == EWeaponType::Dagger ||
		NewWeaponType == EWeaponType::LongSword ||
		NewWeaponType == EWeaponType::Mace)
	{
		FSkill* Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 100;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_FirstSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 100;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_SecondSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 120;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_ThirdSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 120;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_FourthSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 150;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_FifthSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 125;
		Skill->SkillLevelUpInfo.StaminaRequired = 10;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_ForwardSPSwing, Skill);

		Skill = new FSkill();
		Skill->SkillLevelUpInfo.DamagePercent = 125;
		Skill->SkillLevelUpInfo.StaminaRequired = 10;
		NormalAttackSectionToSkillMap.Add(UCharacterLibrary::SectionName_BackwardSPSwing, Skill);
	}
	*/
}

void APlayerCharacter::OnRep_WeaponSheathed()
{
	UpdateCurrentWeaponAnimationType();	
}

void APlayerCharacter::OnRep_CurrentWeaponAnimationToUse(EWeaponAnimationType OldAnimationType)
{
}

void APlayerCharacter::Server_SetWeaponSheathed_Implementation(bool bNewValue)
{
	SetWeaponSheathed(bNewValue);
}

bool APlayerCharacter::Server_SetWeaponSheathed_Validate(bool bNewValue)
{
	return true;
}
