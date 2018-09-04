// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerCharacter.h"
#include "EODPlayerController.h"
#include "Core/GameSingleton.h"
#include "PlayerAnimInstance.h"
#include "Weapons/PrimaryWeapon.h"
#include "Weapons/SecondaryWeapon.h"
#include "Statics/WeaponLibrary.h"
#include "UI/HUDWidget.h"
#include "Components/InventoryComponent.h"
#include "Components/PlayerStatsComponent.h"
#include "Core/EODPreprocessors.h"

#include "Engine/World.h"
#include "UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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

	Hair			= CreateNewArmorComponent(TEXT("Hair"), ObjectInitializer);
	HatItem			= CreateNewArmorComponent(TEXT("Hat Item"), ObjectInitializer);
	FaceItem		= CreateNewArmorComponent(TEXT("Face Item"), ObjectInitializer);
	Chest			= CreateNewArmorComponent(TEXT("Chest"), ObjectInitializer);
	Hands			= CreateNewArmorComponent(TEXT("Hands"), ObjectInitializer);
	Legs			= CreateNewArmorComponent(TEXT("Legs"), ObjectInitializer);
	Feet			= CreateNewArmorComponent(TEXT("Feet"), ObjectInitializer);

	//~ Begin Camera Components Initialization
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("Camera Boom"));
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->AddLocalOffset(FVector(0.f, 0.f, 60.f));

	PlayerCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//~ End Camera Components Initialization

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Player Inventory"));

	SetWalkSpeed(BaseNormalMovementSpeed * StatsComp->GetMovementSpeedModifier());

	// be default the weapon should be sheathed
	bWeaponSheathed = true;

	// bHasActiveiframes = false;
	// bIsBlockingDamage = false;

	MaxNumberOfSkills = 30;
	
	for (int i = 0; i < MaxNumberOfSkills; i++)
	{
		EventsOnSuccessfulSkillAttack.Add(FCombatEvent());
		EventsOnUsingSkill.Add(FCombatEvent());
	}

	Faction = EFaction::Player;
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

	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &APlayerCharacter::EnableBlock);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &APlayerCharacter::DisableBlock);

	PlayerInputComponent->BindAction("NormalAttack", IE_Pressed, this, &APlayerCharacter::OnNormalAttackKeyPressed);
	PlayerInputComponent->BindAction("NormalAttack", IE_Released, this, &APlayerCharacter::OnNormalAttackKeyReleased);
	
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &APlayerCharacter::OnDodge);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::OnJump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::OnInteract);
	PlayerInputComponent->BindAction("ToggleSheathe", IE_Pressed, this, &APlayerCharacter::OnToggleSheathe);
	PlayerInputComponent->BindAction("ToggleStats", IE_Pressed, this, &APlayerCharacter::OnToggleCharacterStatsUI);
	PlayerInputComponent->BindAction("ToggleMouseCursor", IE_Pressed, this, &APlayerCharacter::OnToggleMouseCursor);
	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, InventoryComponent, &UInventoryComponent::ToggleInventoryUI);
	PlayerInputComponent->BindAction("ToggleAutoRun", IE_Pressed, this, &APlayerCharacter::OnToggleAutoRun);


	PlayerInputComponent->BindAction("Skill_1", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<0>);
	PlayerInputComponent->BindAction("Skill_2", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<1>);
	PlayerInputComponent->BindAction("Skill_3", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<2>);
	PlayerInputComponent->BindAction("Skill_4", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<3>);
	PlayerInputComponent->BindAction("Skill_5", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<4>);
	PlayerInputComponent->BindAction("Skill_6", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<5>);
	PlayerInputComponent->BindAction("Skill_7", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<6>);
	PlayerInputComponent->BindAction("Skill_8", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<7>);
	PlayerInputComponent->BindAction("Skill_9", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<8>);
	PlayerInputComponent->BindAction("Skill_10", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<9>);
	PlayerInputComponent->BindAction("Skill_11", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<10>);
	PlayerInputComponent->BindAction("Skill_12", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<11>);
	PlayerInputComponent->BindAction("Skill_13", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<12>);
	PlayerInputComponent->BindAction("Skill_14", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<13>);
	PlayerInputComponent->BindAction("Skill_15", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<14>);
	PlayerInputComponent->BindAction("Skill_16", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<15>);
	PlayerInputComponent->BindAction("Skill_17", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<16>);
	PlayerInputComponent->BindAction("Skill_18", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<17>);
	PlayerInputComponent->BindAction("Skill_19", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<18>);
	PlayerInputComponent->BindAction("Skill_20", IE_Pressed, this, &APlayerCharacter::PressedSkillKey<19>);

	PlayerInputComponent->BindAction("Skill_1", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<0>);
	PlayerInputComponent->BindAction("Skill_2", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<1>);
	PlayerInputComponent->BindAction("Skill_3", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<2>);
	PlayerInputComponent->BindAction("Skill_4", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<3>);
	PlayerInputComponent->BindAction("Skill_5", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<4>);
	PlayerInputComponent->BindAction("Skill_6", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<5>);
	PlayerInputComponent->BindAction("Skill_7", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<6>);
	PlayerInputComponent->BindAction("Skill_8", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<7>);
	PlayerInputComponent->BindAction("Skill_9", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<8>);
	PlayerInputComponent->BindAction("Skill_10", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<9>);
	PlayerInputComponent->BindAction("Skill_11", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<10>);
	PlayerInputComponent->BindAction("Skill_12", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<11>);
	PlayerInputComponent->BindAction("Skill_13", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<12>);
	PlayerInputComponent->BindAction("Skill_14", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<13>);
	PlayerInputComponent->BindAction("Skill_15", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<14>);
	PlayerInputComponent->BindAction("Skill_16", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<15>);
	PlayerInputComponent->BindAction("Skill_17", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<16>);
	PlayerInputComponent->BindAction("Skill_18", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<17>);
	PlayerInputComponent->BindAction("Skill_19", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<18>);
	PlayerInputComponent->BindAction("Skill_20", IE_Released, this, &APlayerCharacter::ReleasedSkillKey<19>);

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
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SheathedWeaponAnimationReferences = UCharacterLibrary::GetPlayerAnimationReferences(EWeaponAnimationType::SheathedWeapon, Gender);

	PrimaryWeapon = GetWorld()->SpawnActor<APrimaryWeapon>(APrimaryWeapon::StaticClass(), SpawnInfo);
	SecondaryWeapon = GetWorld()->SpawnActor<ASecondaryWeapon>(ASecondaryWeapon::StaticClass(), SpawnInfo);

	PrimaryWeapon->SetOwningCharacter(this);
	SecondaryWeapon->SetOwningCharacter(this);

	// @note please set secondary weapon first and primary weapon later.
	if (SecondaryWeaponID != NAME_None)
	{
		SetCurrentSecondaryWeapon(SecondaryWeaponID);
	}
	if (PrimaryWeaponID != NAME_None)
	{
		SetCurrentPrimaryWeapon(PrimaryWeaponID);
	}
}

#if WITH_EDITOR
void APlayerCharacter::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// @todo definition
}
#endif

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling() && !IsJumping())
	{
		// SetCharacterState(ECharacterState::Jumping);

		// This case is an exception to CharacterState replication
		CharacterState = ECharacterState::Jumping;
		GetMesh()->GetAnimInstance()->Montage_Play(GetActiveAnimationReferences()->AnimationMontage_Jump);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("JumpLoop"), GetActiveAnimationReferences()->AnimationMontage_Jump);
	}
	// It is necessary to test if jump montage is playing, or else the "JumpEnd" sections ends up playing twice because of montage blending out
	else if (!GetCharacterMovement()->IsFalling() && IsJumping() && GetMesh()->GetAnimInstance()->Montage_IsPlaying(GetActiveAnimationReferences()->AnimationMontage_Jump))
	{
		FName CurrentSection = GetMesh()->GetAnimInstance()->Montage_GetCurrentSection(GetActiveAnimationReferences()->AnimationMontage_Jump);
		if (CurrentSection != FName("JumpEnd"))
		{
			GetMesh()->GetAnimInstance()->Montage_Play(GetActiveAnimationReferences()->AnimationMontage_Jump);
			GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("JumpEnd"), GetActiveAnimationReferences()->AnimationMontage_Jump);
		}
	}

	if (IsLocallyControlled())
	{
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
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	//~ Player HUD
	if (Controller && Controller->IsLocalPlayerController() && BP_HUDWidget.Get())
	{
		HUDWidget = CreateWidget<UHUDWidget>(GetGameInstance(), BP_HUDWidget);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}

USkeletalMeshComponent * APlayerCharacter::CreateNewArmorComponent(const FName Name, const FObjectInitializer & ObjectInitializer)
{
	USkeletalMeshComponent* Sk = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, Name);
	Sk->SetupAttachment(GetMesh());
	// Sk->AddLocalRotation(FRotator(0.f, -90.f, 0.f));
	// Sk->AddLocalOffset(FVector(0.f, 0.f, -90.f));
	Sk->SetMasterPoseComponent(GetMesh());
	Sk->bUseAttachParentBound = true;
	return Sk;
}

bool APlayerCharacter::CanMove() const
{
	return CharacterState == ECharacterState::IdleWalkRun || IsBlocking();
}

bool APlayerCharacter::CanJump() const
{
	return CharacterState == ECharacterState::IdleWalkRun || IsBlocking();
}

bool APlayerCharacter::CanDodge() const
{
	return CharacterState == ECharacterState::IdleWalkRun || IsBlocking() || IsCastingSpell();
	// @todo add UsingSkill, Looting, Interacting, etc. to this too
}

bool APlayerCharacter::CanNormalAttack() const
{
	return CharacterState == ECharacterState::IdleWalkRun || CharacterState == ECharacterState::Attacking;
}

bool APlayerCharacter::IsAutoRunning() const
{
	return CharacterState == ECharacterState::AutoRun;
}

APrimaryWeapon * APlayerCharacter::GetPrimaryWeapon() const
{
	return PrimaryWeapon;
}

ASecondaryWeapon * APlayerCharacter::GetSecondaryWeapon() const
{
	return SecondaryWeapon;
}

bool APlayerCharacter::CanAutoRun() const
{
	// The character can auto run only if character is in idle state
	return CharacterState == ECharacterState::IdleWalkRun && GetVelocity().Size() == 0;
}

void APlayerCharacter::MoveForward(const float Value)
{
	if (Value != 0 && CanMove())
	{
		FRotator rot = FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f);
		FVector Direction = FRotationMatrix(rot).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(const float Value)
{
	if (Value != 0 && CanMove())
	{
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
	if (CanDodge() && PlayerAnimInstance && GetActiveAnimationReferences())
	{
		float ForwardAxisValue = InputComponent->GetAxisValue(TEXT("MoveForward"));
		float RightAxisValue = InputComponent->GetAxisValue(TEXT("MoveRight"));
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
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Dodge, FName("RightDodge"), ECharacterState::Dodging);
			}
			else if (RightAxisValue < 0)
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Dodge, FName("LeftDodge"), ECharacterState::Dodging);
			}
			else
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Dodge, FName("BackwardDodge"), ECharacterState::Dodging);
			}
		}
		else
		{
			if (ForwardAxisValue > 0)
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Dodge, FName("ForwardDodge"), ECharacterState::Dodging);
			}
			else if (ForwardAxisValue < 0)
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Dodge, FName("BackwardDodge"), ECharacterState::Dodging);
			}
		}
	}
}

void APlayerCharacter::EnableBlock()
{
	if (CanBlock())
	{
		SetCharacterState(ECharacterState::Blocking);
		SetUseControllerRotationYaw(true);
		SetWalkSpeed(BaseBlockMovementSpeed * StatsComp->GetMovementSpeedModifier());
	}
}

void APlayerCharacter::DisableBlock()
{
	SetUseControllerRotationYaw(false);

	if (IsBlocking())
	{
		SetCharacterState(ECharacterState::IdleWalkRun);
	}
}

void APlayerCharacter::OnJump()
{
	if (CanJump() && PlayerAnimInstance && GetActiveAnimationReferences())
	{
		if (IsBlocking())
		{
			DisableBlock();
		}

		Jump();
		PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_Jump, FName("JumpStart"), ECharacterState::Jumping);
	}
}

void APlayerCharacter::OnInteract()
{
}

void APlayerCharacter::OnToggleSheathe()
{
	bool bNewValue = !bWeaponSheathed;
	SetWeaponSheathed(bNewValue);
}

void APlayerCharacter::OnToggleCharacterStatsUI()
{
}

void APlayerCharacter::OnToggleMouseCursor()
{
}

/*
void APlayerCharacter::OnNormalAttack()
{
	if (CanNormalAttack() && PlayerAnimInstance && GetActiveAnimationReferences())
	{
		if (IsNormalAttacking())
		{
			FName CurrentSection = PlayerAnimInstance->Montage_GetCurrentSection(GetActiveAnimationReferences()->AnimationMontage_NormalAttacks);
			if (CurrentSection == FName("FirstSwingEnd"))
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_NormalAttacks, FName("SecondSwing"), ECharacterState::Attacking);
				SetCharacterRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
			}
			else if (CurrentSection == FName("SecondSwingEnd"))
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_NormalAttacks, FName("ThirdSwing"), ECharacterState::Attacking);
				SetCharacterRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
			}
			else if (CurrentSection == FName("ThirdSwingEnd"))
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_NormalAttacks, FName("FourthSwing"), ECharacterState::Attacking);
				SetCharacterRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
			}
			else if (CurrentSection == FName("FourthSwingEnd"))
			{
				PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_NormalAttacks, FName("FifthSwing"), ECharacterState::Attacking);
				SetCharacterRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
			}
			else if (CurrentSection == FName("FirstSwing"))
			{
				SetNextMontageSection(FName("FirstSwing"), FName("SecondSwing"));
			}
			else if (CurrentSection == FName("SecondSwing"))
			{
				SetNextMontageSection(FName("SecondSwing"), FName("ThirdSwing"));
			}
			else if (CurrentSection == FName("ThirdSwing"))
			{
				SetNextMontageSection(FName("ThirdSwing"), FName("FourthSwing"));
			}
			else if (CurrentSection == FName("FourthSwing"))
			{
				SetNextMontageSection(FName("FourthSwing"), FName("FifthSwing"));
			}
		}
		else
		{
			PlayAnimationMontage(GetActiveAnimationReferences()->AnimationMontage_NormalAttacks, FName("FirstSwing"), ECharacterState::Attacking);
			SetCharacterRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
		}
	}
}
*/

void APlayerCharacter::OnNormalAttackKeyPressed()
{
}

void APlayerCharacter::OnNormalAttackKeyReleased()
{
}

void APlayerCharacter::OnToggleAutoRun()
{
	if (CharacterState == ECharacterState::AutoRun)
	{
		DisableAutoRun();
	}
	else if (CanAutoRun())
	{
		EnableAutoRun();
	}
}

void APlayerCharacter::EnableAutoRun()
{
	SetCharacterState(ECharacterState::AutoRun);
	SetUseControllerRotationYaw(true);
}

void APlayerCharacter::DisableAutoRun()
{
	// Make sure that auto run is active before you attempt to disable it
	check(CharacterState == ECharacterState::AutoRun);

	SetCharacterState(ECharacterState::IdleWalkRun);
	SetUseControllerRotationYaw(false);
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

	float ForwardAxisValue = InputComponent->GetAxisValue(TEXT("MoveForward"));
	float RightAxisValue = InputComponent->GetAxisValue(TEXT("MoveRight"));
	if (ForwardAxisValue == 0 && RightAxisValue == 0)
	{
		bRotatePlayer = false;
	}

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
	float ForwardAxisValue = InputComponent->GetAxisValue(TEXT("MoveForward"));
	float RightAxisValue = InputComponent->GetAxisValue(TEXT("MoveRight"));
	
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
	
	float Speed = BaseNormalMovementSpeed * StatsComp->GetMovementSpeedModifier();
	if (GetCharacterMovement()->MaxWalkSpeed != Speed)
	{
		SetWalkSpeed(Speed);
	}
}

void APlayerCharacter::OnMeleeCollision(UAnimSequenceBase * Animation, TArray<FHitResult>& HitResults, bool bHit)
{
	/*
	// @note intentionally commented out. If player animation references are null when a collision event is triggered, we want it to crash.
	if (!PlayerAnimationReferences || !(Animation == PlayerAnimationReferences->AnimationMontage_NormalAttacks || 
										Animation == PlayerAnimationReferences->AnimationMontage_Skills || 
										Animation == PlayerAnimationReferences->AnimationMontage_Spells))
	{
		return;
	}
	*/

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

		FEODDamage EODDamage(ActiveSkill);
		EODDamage.CapsuleHitResult = HitResult;
		EODDamage.LineHitResult = LineHitResultToHitCharacter;
		ApplyEODDamage(HitCharacter, EODDamage);
	}

	if (!bEnemiesHit)
	{
		OnUnsuccessfulHit.Broadcast(TArray<TWeakObjectPtr<AEODCharacterBase>>());
	}
}

void APlayerCharacter::ApplyEODDamage(AEODCharacterBase* HitCharacter, FEODDamage& EODDamage)
{
}

void APlayerCharacter::TakeEODDamage(AEODCharacterBase* HitInstigator, FEODDamage& EODDamage)
{
}

void APlayerCharacter::Destroyed()
{
	Super::Destroyed();

	if (PrimaryWeapon)
	{
		PrimaryWeapon->SetOwningCharacter(nullptr);
		PrimaryWeapon->OnUnEquip();
		PrimaryWeapon->Destroy();
	}
	if (SecondaryWeapon)
	{
		SecondaryWeapon->SetOwningCharacter(nullptr);
		SecondaryWeapon->OnUnEquip();
		SecondaryWeapon->Destroy();
	}
}

/*
void APlayerCharacter::UpdatePlayerAnimationReferences()
{
	if (PlayerAnimationReferences)
	{
		UCharacterLibrary::UnloadPlayerAnimationReferences(PlayerAnimationReferences, Gender);

		// delete older animation references, prevent memory leak
		delete PlayerAnimationReferences;
		PlayerAnimationReferences = nullptr;
	}

	PlayerAnimationReferences = UCharacterLibrary::GetPlayerAnimationReferences(CurrentWeaponAnimationToUse, Gender);

}
*/

void APlayerCharacter::UpdateEquippedWeaponAnimationReferences(const EWeaponType EquippedWeaponType)
{
	if (EquippedWeaponAnimationReferences)
	{
		UCharacterLibrary::UnloadPlayerAnimationReferences(EquippedWeaponAnimationReferences, Gender);

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
	EquippedWeaponAnimationReferences = UCharacterLibrary::GetPlayerAnimationReferences(WeaponAnimationType, Gender);
}

FPlayerAnimationReferences * APlayerCharacter::GetActiveAnimationReferences() const
{
	return bWeaponSheathed ? SheathedWeaponAnimationReferences : EquippedWeaponAnimationReferences;
}

/*
float APlayerCharacter::GetMaxPlayerWalkSpeed() const
{
	return BaseWalkSpeed * StatsComp->GetMovementSpeedModifier();
}
*/

void APlayerCharacter::OnPressingSkillKey(const uint32 SkillButtonIndex)
{
#if DEVSTAGE_CODE_ENABLED
	if (SkillButtonIndex == 0)
	{
		SetCurrentWeaponAnimationToUse(EWeaponAnimationType::NoWeapon);
	}
	else if (SkillButtonIndex == 1)
	{
		SetCurrentWeaponAnimationToUse(EWeaponAnimationType::ShieldAndSword);
	}
	else if (SkillButtonIndex == 2)
	{
		SetCurrentWeaponAnimationToUse(EWeaponAnimationType::GreatSword);
	}
	else if (SkillButtonIndex == 3)
	{
		SetCurrentWeaponAnimationToUse(EWeaponAnimationType::WarHammer);
	}
	else if (SkillButtonIndex == 4)
	{
		SetCurrentWeaponAnimationToUse(EWeaponAnimationType::Staff);
	}
	else if (SkillButtonIndex == 5)
	{
		SetCurrentWeaponAnimationToUse(EWeaponAnimationType::Daggers);
	}
#endif
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
	float ForwardAxisValue = InputComponent->GetAxisValue(TEXT("MoveForward"));
	float RightAxisValue = InputComponent->GetAxisValue(TEXT("MoveRight"));
	
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

void APlayerCharacter::OnMontageBlendingOut(UAnimMontage * AnimMontage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		CharacterState = ECharacterState::IdleWalkRun;
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
	// If WeaponID is invalid
	if (!WeaponData)
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
	// If WeaponID is invalid
	if (!WeaponData)
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

	// UpdateCurrentWeaponAnimationType();
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

bool APlayerCharacter::IsPrimaryWeaponEquippped() const
{
	return PrimaryWeaponID != NAME_None && PrimaryWeapon->bEquipped;
}

bool APlayerCharacter::IsSecondaryWeaponEquipped() const
{
	return SecondaryWeaponID != NAME_None && SecondaryWeapon->bEquipped;
}

void APlayerCharacter::SetIWRCharMovementDir(ECharMovementDirection NewDirection)
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

void APlayerCharacter::OnRep_WeaponSheathed()
{
	UpdateCurrentWeaponAnimationType();	
}

void APlayerCharacter::OnRep_CurrentWeaponAnimationToUse()
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
