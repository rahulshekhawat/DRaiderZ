// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerCharacter.h"
#include "PlayerAnimInstance.h"
#include "Core/GameSingleton.h"
#include "Statics/WeaponLibrary.h"
#include "Components/InventoryComponent.h"
#include "Components/PlayerStatsComponent.h"
#include "Core/EODPreprocessors.h"
#include "Core/EODSaveGame.h"
#include "UI/HUDWidget.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer & ObjectInitializer) :Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerStatsComponent>(FName("Character Stats Component")))
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
	SkillsComponent = ObjectInitializer.CreateDefaultSubobject<USkillsComponent>(this, FName("Skills Component"));

	AudioComponent = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, FName("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);

	// By default the weapon should be sheathed
	bWeaponSheathed = true;

	BaseNormalMovementSpeed = 400.f;
	BaseSpecialMovementSpeed = 600.f;
	BaseBlockMovementSpeed = 150.f;

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
	PlayerInputComponent->BindAction("ToggleSkillTree", IE_Pressed, SkillsComponent, &USkillsComponent::ToggleSkillTreeUI);
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
	// DOREPLIFETIME_CONDITION(APlayerCharacter, CurrentWeaponAnimationToUse, COND_SkipOwner);
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

	SetWalkSpeed(BaseNormalMovementSpeed * GetStatsComponent()->GetMovementSpeedModifier());
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
#if MESSAGE_LOGGING_ENABLED
		UKismetSystemLibrary::PrintString(this, FString("Animation references are NULL"));
#endif // MESSAGE_LOGGING_ENABLED
		return;
	}

	/*
	if (Controller && Controller->IsLocalPlayerController())
	{
		// Manage controller rotation yaw
		if (IsBlocking() || IsAutoRunning())
		{
			if (!bUseControllerRotationYaw)
			{
				SetUseControllerRotationYaw(true);
			}
		}
		else
		{
			if (bUseControllerRotationYaw)
			{
				SetUseControllerRotationYaw(false);
			}
		}



		// Smooth rotation
		if (bRotateSmoothly)
		{
			bRotateSmoothly = !DeltaRotateCharacterToDesiredYaw(DesiredSmoothRotationYaw, DeltaTime);
		}
	}
	*/

	if (Controller && Controller->IsLocalPlayerController())
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
		else if (IsMoving() || (IsUsingAnySkill() && bSkillAllowsMovement))
		{
			UpdateMovement(DeltaTime);
		}
		else if (IsBlocking())
		{
			UpdateBlockState(DeltaTime);
		}
		else if (IsFastRunning())
		{
			UpdateFastMovementState(DeltaTime);
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

	if (Controller && Controller->IsLocalPlayerController() && HUDWidget)
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
	return IsIdleOrMoving() || IsBlocking() || IsAutoRunning() || (IsUsingAnySkill() && bSkillAllowsMovement) || IsFastRunning();
}

bool APlayerCharacter::CanJump() const
{
	return IsIdleOrMoving() || IsBlocking() || IsAutoRunning();
}

bool APlayerCharacter::CanDodge() const
{
	int32 DodgeCost = DodgeStaminaCost * GetStatsComponent()->GetStaminaConsumptionModifier();

	// @todo add UsingSkill, Looting, Interacting, etc. to this too
	if (GetStatsComponent()->GetCurrentStamina() >= DodgeCost &&
		(IsIdleOrMoving() || IsBlocking() || IsCastingSpell() || IsNormalAttacking()))
	{
		return true;
	}

	return false;
}

bool APlayerCharacter::CanBlock() const
{
	return (IsIdleOrMoving() || IsAutoRunning() || IsNormalAttacking()) &&
		!(GetEquippedWeaponType() == EWeaponType::None || IsWeaponSheathed());
}

bool APlayerCharacter::CanNormalAttack() const
{
	return IsIdleOrMoving() || IsNormalAttacking();
}

bool APlayerCharacter::CanUseAnySkill() const
{
	return (GetEquippedWeaponType() != EWeaponType::None) && !IsWeaponSheathed() && (IsIdleOrMoving() || IsBlocking() || IsFastRunning());
}

bool APlayerCharacter::CanUseSkill(FSkillTableRow * Skill)
{
	if (Skill)
	{
		if ((Skill->SupportedWeapons & (1 << (uint8)GetEquippedWeaponType())) &&
			GetStatsComponent()->GetCurrentMana() > Skill->ManaRequired &&
			GetStatsComponent()->GetCurrentStamina() > Skill->StaminaRequired)
		{
			return true;
		}
	}

	return false;
}

bool APlayerCharacter::IsAutoRunning() const
{
	return GetCharacterState() == ECharacterState::AutoRun;
}

UHUDWidget* APlayerCharacter::BP_GetHUDWidget() const
{
	return GetHUDWidget();
}

bool APlayerCharacter::Interrupt(const float BCAngle)
{
	if (CanInterrupt() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->HitEffects.Get())
	{
		UAnimMontage* HitMontage = GetActiveAnimationReferences()->HitEffects.Get();

		if (BCAngle <= 90)
		{
			PlayAnimationMontage(HitMontage,
				UCharacterLibrary::SectionName_ForwardInterrupt,
				ECharacterState::GotHit);
		}
		else
		{
			PlayAnimationMontage(HitMontage,
				UCharacterLibrary::SectionName_BackwardInterrupt,
				ECharacterState::GotHit);
		}

		return true;
	}

	return false;
}

bool APlayerCharacter::Flinch(const float BCAngle)
{
	if (CanInterrupt() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->Flinch.Get())
	{
		UAnimMontage* FlinchMontage = GetActiveAnimationReferences()->Flinch.Get();

		if (BCAngle <= 90)
		{
			PlayAnimationMontage(FlinchMontage,
				UCharacterLibrary::SectionName_ForwardFlinch);
		}
		else
		{
			PlayAnimationMontage(FlinchMontage,
				UCharacterLibrary::SectionName_BackwardFlinch);
		}

		return true;
	}

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
	if (CanKnockdown() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->HitEffects.Get())
	{
		PlayAnimationMontage(GetActiveAnimationReferences()->HitEffects.Get(),
			UCharacterLibrary::SectionName_KnockdownStart,
			ECharacterState::GotHit);
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &APlayerCharacter::EndKnockdown, Duration, false);

		return true;
	}

	return false;
}

void APlayerCharacter::EndKnockdown()
{
	PlayAnimationMontage(GetActiveAnimationReferences()->HitEffects.Get(),
		UCharacterLibrary::SectionName_KnockdownEnd,
		ECharacterState::GotHit);
}

bool APlayerCharacter::Knockback(const float Duration, const FVector& ImpulseDirection)
{
	if (CanKnockdown() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->HitEffects.Get())
	{
		PlayAnimationMontage(GetActiveAnimationReferences()->HitEffects.Get(),
			UCharacterLibrary::SectionName_KnockdownStart,
			ECharacterState::GotHit);
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &APlayerCharacter::EndKnockdown, Duration, false);
		PushPlayer(ImpulseDirection);

		return true;
	}

	return false;
}

void APlayerCharacter::BlockAttack()
{
	if (GetActiveAnimationReferences() && GetActiveAnimationReferences()->BlockAttack.Get())
	{
		PlayAnimationMontage(GetActiveAnimationReferences()->BlockAttack.Get(),
			UCharacterLibrary::SectionName_BlockAttack);
	}
}

bool APlayerCharacter::CanAutoRun() const
{
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

		FRotator Rotation = FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f);
		FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		// Internal_AddMovementInput()
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
	// Alternative to disable auto run
	if (bUseControllerRotationYaw)
	{
		SetUseControllerRotationYaw(false);
	}

	/*
	if (IsAutoRunning())
	{
		DisableAutoRun();
	}
	*/

	/*
	if (CanDodge())
	{
		int32 DodgeCost = DodgeStaminaCost * GetStatsComponent()->GetStaminaConsumptionModifier();
		GetStatsComponent()->ModifyCurrentStamina(-DodgeCost);

		float ForwardAxisValue = InputComponent->GetAxisValue(FName("MoveForward"));
		float RightAxisValue = InputComponent->GetAxisValue(FName("MoveRight"));
		float DesiredYaw;

		if (ForwardAxisValue != 0)
		{
			DesiredYaw = GetRotationYawFromAxisInput();
		}
		else
		{
			DesiredYaw = GetPlayerControlRotationYaw();
		}

		SetCharacterRotation(FRotator(0.f, DesiredYaw, 0.f));

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("EnableiFrames"), DodgeImmunityDuration);
		GetWorld()->GetTimerManager().SetTimer(DodgeImmunityTimerHandle, TimerDelegate, DodgeImmunityTriggerDelay, false);
	}
	*/

	if (CanDodge() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->Dodge.Get())
	{
		int32 DodgeCost = DodgeStaminaCost * GetStatsComponent()->GetStaminaConsumptionModifier();
		GetStatsComponent()->ModifyCurrentStamina(-DodgeCost);

		float ForwardAxisValue = InputComponent->GetAxisValue(FName("MoveForward"));
		float RightAxisValue = InputComponent->GetAxisValue(FName("MoveRight"));
		float DesiredYaw = GetPlayerControlRotationYaw();

		if (ForwardAxisValue != 0)
		{
			DesiredYaw = GetRotationYawFromAxisInput();
		}

		SetCharacterRotation(FRotator(0.f, DesiredYaw, 0.f));
		UAnimMontage* DodgeMontage = GetActiveAnimationReferences()->Dodge.Get();
		if (ForwardAxisValue == 0)
		{
			if (RightAxisValue > 0)
			{
				PlayAnimationMontage(DodgeMontage,
					UCharacterLibrary::SectionName_RightDodge,
					ECharacterState::Dodging);
			}
			else if (RightAxisValue < 0)
			{
				PlayAnimationMontage(DodgeMontage,
					UCharacterLibrary::SectionName_LeftDodge,
					ECharacterState::Dodging);
			}
			else
			{
				PlayAnimationMontage(DodgeMontage,
					UCharacterLibrary::SectionName_BackwardDodge,
					ECharacterState::Dodging);
			}
		}
		else
		{
			if (ForwardAxisValue > 0)
			{
				PlayAnimationMontage(DodgeMontage,
					UCharacterLibrary::SectionName_ForwardDodge,
					ECharacterState::Dodging);
			}
			else if (ForwardAxisValue < 0)
			{
				PlayAnimationMontage(DodgeMontage,
					UCharacterLibrary::SectionName_BackwardDodge,
					ECharacterState::Dodging);
			}
		}

		// @note Why would DodgeImmunityTimerHandle be active here?. Uncommented for now
		/*
		if (GetWorld()->GetTimerManager().IsTimerActive(DodgeImmunityTimerHandle))
		{
			DisableiFrames();
		}
		*/

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("EnableiFrames"), DodgeImmunityDuration);
		GetWorld()->GetTimerManager().SetTimer(DodgeImmunityTimerHandle, TimerDelegate, DodgeImmunityTriggerDelay, false);
	}
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

void APlayerCharacter::OnJump()
{
	if (CanJump() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->Jump.Get())
	{
		if (IsBlocking())
		{
			DisableBlock();
		}

		if (IsAutoRunning())
		{
			DisableAutoRun();
		}

		Jump();
	}
}

void APlayerCharacter::OnInteract()
{
	// @todo definition
}

void APlayerCharacter::OnToggleSheathe()
{
	bool bNewValue = !bWeaponSheathed;
	SetWeaponSheathed(bNewValue);
	
	// @todo play sheathe animation
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

void APlayerCharacter::OnPressedNormalAttack()
{
	if (!CanNormalAttack() || !GetActiveAnimationReferences() || !GetActiveAnimationReferences()->NormalAttacks.Get())
	{
		return;
	}

	UAnimMontage* NormalAttackMontage = GetActiveAnimationReferences()->NormalAttacks.Get();

	// @todo maybe change character state to using skill when using SP attacks?s
	if (!IsNormalAttacking() && bForwardPressed)
	{
		PlayAnimationMontage(NormalAttackMontage,
			UCharacterLibrary::SectionName_ForwardSPSwing,
			ECharacterState::Attacking);
		SetCharacterRotation(FRotator(0.f, GetPlayerControlRotationYaw(), 0.f));
	}
	else if (!IsNormalAttacking() && bBackwardPressed)
	{
		PlayAnimationMontage(NormalAttackMontage,
			UCharacterLibrary::SectionName_BackwardSPSwing,
			ECharacterState::Attacking);
		SetCharacterRotation(FRotator(0.f, GetPlayerControlRotationYaw(), 0.f));
	}
	else if (!IsNormalAttacking())
	{
		PlayAnimationMontage(NormalAttackMontage,
			UCharacterLibrary::SectionName_FirstSwing,
			ECharacterState::Attacking);
	}
	else if (IsNormalAttacking())
	{
		FName CurrentSection = GetMesh()->GetAnimInstance()->Montage_GetCurrentSection(NormalAttackMontage);
		FName NextSection = GetNextNormalAttackSectionName(CurrentSection);

		if (NextSection != NAME_None)
		{
			FString CurrentSectionString = CurrentSection.ToString();
			if (CurrentSectionString.EndsWith("End"))
			{
				PlayAnimationMontage(NormalAttackMontage, NextSection, ECharacterState::Attacking);
			}
			else
			{
				SetNextMontageSection(CurrentSection, NextSection);
			}
		}
	}
}

void APlayerCharacter::OnReleasedNormalAttack()
{
	// empty 
}

void APlayerCharacter::OnToggleAutoRun()
{
	if (GetCharacterState() == ECharacterState::AutoRun)
	{
		DisableAutoRun();
	}
	else if (CanAutoRun())
	{
		EnableAutoRun();
	}
}

void APlayerCharacter::StopNormalAttacking()
{
	GetMesh()->GetAnimInstance()->Montage_Stop(0.2, GetActiveAnimationReferences()->NormalAttacks.Get());
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

	if (bRotatePlayer)
	{
		DeltaRotateCharacterToDesiredYaw(DesiredPlayerRotationYaw, DeltaTime);
	}
	
	if (ForwardAxisValue < 0)
	{
		float Speed = (BaseNormalMovementSpeed * GetStatsComponent()->GetMovementSpeedModifier() * 5) / 16;
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			SetWalkSpeed(Speed);
		}
	}
	else
	{
		float Speed = BaseNormalMovementSpeed * GetStatsComponent()->GetMovementSpeedModifier();
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			SetWalkSpeed(Speed);
		}
	}

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

void APlayerCharacter::UpdateFastMovementState(float DeltaTime)
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

	if (bRotatePlayer)
	{
		DeltaRotateCharacterToDesiredYaw(DesiredPlayerRotationYaw, DeltaTime);
	}

	if (ForwardAxisValue < 0)
	{
		float Speed = (BaseNormalMovementSpeed * GetStatsComponent()->GetMovementSpeedModifier() * 5) / 16;
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			SetWalkSpeed(Speed);
		}
	}
	else
	{
		float Speed = BaseSpecialMovementSpeed * GetStatsComponent()->GetMovementSpeedModifier();
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			SetWalkSpeed(Speed);
		}
	}

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

void APlayerCharacter::UpdateAutoRun(float DeltaTime)
{
	if (Controller && Controller->IsLocalPlayerController())
	{
		FRotator rot = FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f);
		FVector Direction = FRotationMatrix(rot).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, 1.f);
	}

	// @todo Why isn't following code inside Enable Auto Run? Un-necessarily repetitive condition checks
	if (IWR_CharacterMovementDirection != ECharMovementDirection::F)
	{
		SetIWRCharMovementDir(ECharMovementDirection::F);
	}
	
	float Speed = BaseNormalMovementSpeed * GetStatsComponent()->GetMovementSpeedModifier();
	if (GetCharacterMovement()->MaxWalkSpeed != Speed)
	{
		SetWalkSpeed(Speed);
	}
}

void APlayerCharacter::Destroyed()
{
	Super::Destroyed();

	if (PrimaryWeapon)
	{
		PrimaryWeapon->OnUnEquip();
		PrimaryWeapon->Destroy();
	}
	if (SecondaryWeapon)
	{
		SecondaryWeapon->OnUnEquip();
		SecondaryWeapon->Destroy();
	}

	UnloadUnequippedWeaponAnimationReferences();
	UnloadEquippedWeaponAnimationReferences();
}

void APlayerCharacter::SavePlayerState()
{
	if (HUDWidget)
	{
		HUDWidget->SaveHUDLayout();
	}
}

FName APlayerCharacter::GetNextNormalAttackSectionName(const FName& CurrentSection) const
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
		if (GetEquippedWeaponType() == EWeaponType::GreatSword ||
			GetEquippedWeaponType() == EWeaponType::WarHammer)
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
		if (GetEquippedWeaponType() == EWeaponType::Staff)
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

FName APlayerCharacter::GetAnimationReferencesRowID(EWeaponType WeaponType, ECharacterGender CharGender)
{
	FString Prefix;
	if (CharGender == ECharacterGender::Female)
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
	case EWeaponType::None:
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

	if (!PlayerAnimationReferencesDataTable)
	{
		return;
	}

	FName RowID = GetAnimationReferencesRowID(EWeaponType::None, Gender);
	FPlayerAnimationReferencesTableRow* PlayerAnimationReferences = PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(RowID,
		FString("APlayerCharacter::LoadUnequippedWeaponAnimationReferences(), loading unequipped weapon animation references"));

	if (!PlayerAnimationReferences)
	{
		return;
	}

	UnequippedWeaponAnimationReferences = PlayerAnimationReferences;
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

	if (!PlayerAnimationReferencesDataTable)
	{
		return;
	}

	FName RowID = GetAnimationReferencesRowID(GetEquippedWeaponType(), Gender);
	FPlayerAnimationReferencesTableRow* PlayerAnimationReferences = PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(RowID,
		FString("APlayerCharacter::LoadEquippedWeaponAnimationReferences(), loading equipped weapon animation references"));

	if (!PlayerAnimationReferences)
	{
		return;
	}
	
	EquippedWeaponAnimationReferences = PlayerAnimationReferences;
	UnequippedWeaponAnimationsStreamableHandle = LoadAnimationReferences(PlayerAnimationReferences);
}

void APlayerCharacter::OnPressingSkillKey(const uint32 SkillButtonIndex)
{
	if (IsBlocking())
	{
		DisableBlock();
	}

	if (!CanUseAnySkill())
	{
#if MESSAGE_LOGGING_ENABLED
		UKismetSystemLibrary::PrintString(this, FString("Can't use any skill right now"));
#endif // MESSAGE_LOGGING_ENABLED
		return;
	}
	
	TPair<FName, FSkillTableRow*> SkillPair = SkillsComponent->GetSkillFromSkillSlot(SkillButtonIndex);
	// There isn't any skill to use
	if (SkillPair.Key == NAME_None || SkillPair.Value == nullptr)
	{
		return;
	}

	GetStatsComponent()->ModifyCurrentMana(-SkillPair.Value->ManaRequired);
	GetStatsComponent()->ModifyCurrentStamina(-SkillPair.Value->StaminaRequired);

	SetCurrentActiveSkillID(SkillPair.Key);
	SetCurrentActiveSkill(SkillPair.Value);
	SkillsComponent->OnSkillUsed(SkillButtonIndex, SkillPair.Key, SkillPair.Value);

	if (SkillPair.Value->AnimMontage.Get())
	{
		bSkillAllowsMovement = SkillPair.Value->bAllowsMovement;
		if (!bSkillAllowsMovement)
		{
			SetOffSmoothRotation(GetPlayerControlRotationYaw());
		}
		PlayAnimationMontage(SkillPair.Value->AnimMontage.Get(), SkillPair.Value ->SkillStartMontageSectionName, ECharacterState::UsingActiveSkill);
	}
}

void APlayerCharacter::OnReleasingSkillKey(const uint32 SkillButtonIndex)
{
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

void APlayerCharacter::OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		SetCharacterState(ECharacterState::IdleWalkRun);
	}

	if (GetCurrentActiveSkill() && GetCurrentActiveSkill()->AnimMontage.Get() == AnimMontage)
	{
		SkillsComponent->SetOffChainSkillReset();
	}
}

void APlayerCharacter::OnMontageEnded(UAnimMontage * AnimMontage, bool bInterrupted)
{
}

void APlayerCharacter::SetCurrentPrimaryWeapon(const FName WeaponID)
{
	//  You would call SetCurrentPrimaryWeapon(NAME_None) when you want to remove equipped primary weapon
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
	PrimaryWeapon->OnEquip(WeaponID, WeaponData);
	PrimaryWeaponID = WeaponID;

	LoadEquippedWeaponAnimationReferences();
	// UpdateCurrentWeaponAnimationType();

	// @todo add weapon stats
}

void APlayerCharacter::SetCurrentSecondaryWeapon(const FName WeaponID)
{
	//  You would call SetCurrentSecondaryWeapon(NAME_None) when you want to remove equipped secondary weapon
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

	
	// @todo add weapon stats
}

void APlayerCharacter::RemovePrimaryWeapon()
{
	PrimaryWeapon->OnUnEquip();
	PrimaryWeaponID = NAME_None;
	UnloadEquippedWeaponAnimationReferences();

	// @todo remove weapon stats
}

void APlayerCharacter::RemoveSecondaryWeapon()
{
	SecondaryWeaponID = NAME_None;
	SecondaryWeapon->OnUnEquip();

	// @todo remove weapon stats
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
	return GetCharacterState() == ECharacterState::SpecialMovement;
}

void APlayerCharacter::Server_SetIWRCharMovementDir_Implementation(ECharMovementDirection NewDirection)
{
	SetIWRCharMovementDir(NewDirection);
}

bool APlayerCharacter::Server_SetIWRCharMovementDir_Validate(ECharMovementDirection NewDirection)
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

	if (Role < ROLE_Authority)
	{
		Server_SetWeaponSheathed(bNewValue);
	}
}

void APlayerCharacter::OnNormalAttackSectionStart(FName SectionName)
{
	FString SkillIDString = FString("");
	if (Gender == ECharacterGender::Male)
	{
		SkillIDString += FString("M_");
	}
	else
	{
		SkillIDString += FString("F_");
	}

	switch (GetEquippedWeaponType())
	{
	case EWeaponType::GreatSword:
		SkillIDString += FString("GS_");
		break;
	case EWeaponType::WarHammer:
		SkillIDString += FString("WH_");
		break;
	case EWeaponType::LongSword:
		SkillIDString += FString("LS_");
		break;
	case EWeaponType::Mace:
		SkillIDString += FString("MC_");
		break;
	case EWeaponType::Dagger:
		SkillIDString += FString("DG_");
		break;
	case EWeaponType::Staff:
		SkillIDString += FString("ST_");
		break;
	case EWeaponType::Shield:
		break;
	case EWeaponType::None:
		break;
	default:
		break;
	}

	if (SectionName == UCharacterLibrary::SectionName_FirstSwing)
	{
		SkillIDString += FString("1");
	}
	else if (SectionName == UCharacterLibrary::SectionName_SecondSwing)
	{
		SkillIDString += FString("2");
	}
	else if (SectionName == UCharacterLibrary::SectionName_ThirdSwing)
	{
		SkillIDString += FString("3");
	}
	else if (SectionName == UCharacterLibrary::SectionName_FourthSwing)
	{
		SkillIDString += FString("4");
	}
	else if (SectionName == UCharacterLibrary::SectionName_FifthSwing)
	{
		SkillIDString += FString("5");
	}
	else if (SectionName == UCharacterLibrary::SectionName_BackwardSPSwing)
	{
		SkillIDString += FString("BSP");
	}
	else if (SectionName == UCharacterLibrary::SectionName_ForwardSPSwing)
	{
		SkillIDString += FString("FSP");
	}
	
	FName SkillID = FName(*SkillIDString);
	SetCurrentActiveSkillID(SkillID);

	// @todo set current active skill
}

void APlayerCharacter::OnRep_WeaponSheathed()
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
