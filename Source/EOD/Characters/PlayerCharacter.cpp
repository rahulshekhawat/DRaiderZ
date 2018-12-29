// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerCharacter.h"
#include "EOD/AnimInstances/PlayerAnimInstance.h"
#include "EOD/Core/GameSingleton.h"
#include "EOD/Statics/WeaponLibrary.h"
#include "EOD/Characters/Components/SkillsComponent.h"
#include "EOD/Player/Components/InventoryComponent.h"
#include "EOD/Player/Components/PlayerStatsComponent.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/Core/EODSaveGame.h"
#include "EOD/UI/HUDWidget.h"
#include "EOD/Characters/NPCBase.h"
#include "EOD/Player/EODPlayerController.h"
#include "EOD/UI/DialogueWindowWidget.h"
#include "EOD/Statics/DialogueLibrary.h"
#include "EOD/Weapons/WeaponDataAsset.h"
// #include "EOD/Weapons/WeaponSlot.h"
#include "EOD/Characters/Components/GameplaySkillsComponent.h"
#include "EOD/Characters/Components/EODCharacterMovementComponent.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/GameUserSettings.h"
// #include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerStatsComponent>(FName("Character Stats Component")))
{
	PrimaryActorTick.bCanEverTick = true;

	// The player should be free to look around with mouse without actually rotating the posessed character
	bUseControllerRotationYaw = false;

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

	// InventoryComponent = ObjectInitializer.CreateDefaultSubobject<UInventoryComponent>(this, FName("Player Inventory"));
	// SkillsComponent = ObjectInitializer.CreateDefaultSubobject<USkillsComponent>(this, FName("Skills Component"));

	AudioComponent = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, FName("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);

	MaxWeaponSlots = 2;
	
	OnPrimaryWeaponEquipped.AddDynamic(this, &APlayerCharacter::ActivateStatusEffectFromWeapon);
	OnPrimaryWeaponEquipped.AddDynamic(this, &APlayerCharacter::LoadWeaponAnimationReferences);
	OnPrimaryWeaponUnequipped.AddDynamic(this, &APlayerCharacter::DeactivateStatusEffectFromWeapon);
	// Perhaps unloading weapon animation references is not necessary until the equipped weapon type actually changes
	// OnPrimaryWeaponUnequipped.AddDynamic(this, &APlayerCharacter::UnloadWeaponAnimationReferences);

	OnSecondaryWeaponEquipped.AddDynamic(this, &APlayerCharacter::ActivateStatusEffectFromWeapon);
	OnSecondaryWeaponUnequipped.AddDynamic(this, &APlayerCharacter::DeactivateStatusEffectFromWeapon);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	/*
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	
	PlayerInputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &APlayerCharacter::ZoomInCamera);
	PlayerInputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &APlayerCharacter::ZoomOutCamera);
	*/

	PlayerInputComponent->BindAction("Forward", IE_Pressed, this, &APlayerCharacter::OnPressedForward);
	PlayerInputComponent->BindAction("Forward", IE_Released, this, &APlayerCharacter::OnReleasedForward);
	PlayerInputComponent->BindAction("Backward", IE_Pressed, this, &APlayerCharacter::OnPressedBackward);
	PlayerInputComponent->BindAction("Backward", IE_Released, this, &APlayerCharacter::OnReleasedBackward);

	/*
	PlayerInputComponent->BindAction("Guard", IE_Pressed, this, &APlayerCharacter::OnPressedBlock);
	PlayerInputComponent->BindAction("Guard", IE_Released, this, &APlayerCharacter::OnReleasedBlock);

	PlayerInputComponent->BindAction("NormalAttack", IE_Pressed, this, &APlayerCharacter::OnPressedNormalAttack);
	PlayerInputComponent->BindAction("NormalAttack", IE_Released, this, &APlayerCharacter::OnReleasedNormalAttack);
	
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &APlayerCharacter::OnDodge);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::OnJump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::OnInteract);
	PlayerInputComponent->BindAction("ToggleSheathe", IE_Pressed, this, &APlayerCharacter::ToggleSheathe);
	PlayerInputComponent->BindAction("ToggleStats", IE_Pressed, this, &APlayerCharacter::ToggleCharacterStatsUI);
	PlayerInputComponent->BindAction("ToggleMouseCursor", IE_Pressed, this, &APlayerCharacter::ToggleMouseCursor);
	PlayerInputComponent->BindAction("ToggleSkillTree", IE_Pressed, SkillsComponent, &USkillsComponent::ToggleSkillTreeUI);
	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, InventoryComponent, &UInventoryComponent::ToggleInventoryUI);
	PlayerInputComponent->BindAction("ToggleAutoRun", IE_Pressed, this, &APlayerCharacter::OnToggleAutoRun);
	PlayerInputComponent->BindAction("Escape", IE_Pressed, this, &APlayerCharacter::OnPressedEscape);
	*/

	/*
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
	*/

}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, PrimaryWeaponID);

}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	LoadUnequippedWeaponAnimationReferences();
	SetWalkSpeed(DefaultWalkSpeed * GetStatsComponent()->GetMovementSpeedModifier());

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = this;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PrimaryWeapon = GetWorld()->SpawnActor<APrimaryWeapon>(APrimaryWeapon::StaticClass(), SpawnInfo);
	SecondaryWeapon = GetWorld()->SpawnActor<ASecondaryWeapon>(ASecondaryWeapon::StaticClass(), SpawnInfo);

	// @note Set secondary weapon first and primary weapon later during initialization
	SetCurrentSecondaryWeapon(SecondaryWeaponID);
	SetCurrentPrimaryWeapon(PrimaryWeaponID);

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
		PrintToScreen(this, FString("Animation references are NULL"));
		return;
	}

	if (Controller && Controller->IsLocalPlayerController())
	{
		/*
		if (IsSwitchingWeapon())
		{
			UpdatePCTryingToMove();
		}
		*/

		/*
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

		if (bNormalAttackPressed && CanNormalAttack())
		{
			DoNormalAttack();
		}

		if (IsIdle())
		{
			UpdateIdleState(DeltaTime);
		}
		else if (IsAutoRunning())
		{
			UpdateAutoRun(DeltaTime);
		}
		else if (IsFastRunning())
		{
			UpdateFastMovementState(DeltaTime);
		}
		else if (bNormalAttackPressed && IsNormalAttacking())
		{
			UpdateNormalAttack(DeltaTime);
		}
		else if (IsMoving() || (IsUsingAnySkill() && bSkillAllowsMovement) || (IsSwitchingWeapon()))
		{
			// UpdateMovement(DeltaTime);
		}
		else if (IsBlocking())
		{
			UpdateBlockState(DeltaTime);
		}
		*/

		/*
		if (bRotateSmoothly)
		{
			bRotateSmoothly = !DeltaRotateCharacterToDesiredYaw(DesiredSmoothRotationYaw, DeltaTime);
		}
		*/
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Since setting master pose component from constructor doesn't work in packaged game
	SetMasterPoseComponentForMeshes();

	if (Controller && Controller->IsLocalPlayerController() && HUDWidgetClass.Get())
	{
		if (HUDWidgetClass.Get())
		{
			HUDWidget = CreateWidget<UHUDWidget>(GetGameInstance(), HUDWidgetClass);
			if (HUDWidget)
			{
				HUDWidget->AddToViewport();
			}
		}
	}
}

USkeletalMeshComponent* APlayerCharacter::CreateNewArmorComponent(const FName Name, const FObjectInitializer& ObjectInitializer)
{
	USkeletalMeshComponent* Sk = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, Name);
	Sk->SetupAttachment(GetMesh());
	Sk->SetMasterPoseComponent(GetMesh());
	Sk->bUseAttachParentBound = true;
	return Sk;
}

bool APlayerCharacter::CanMove() const
{
	// @todo, uncomment and replace return code with following comment:
	// return IsIdleOrMoving() || IsRunning() || bCharacterStateAllowsMovement;
	return IsIdleOrMoving() || IsBlocking() || IsAutoRunning() || (IsUsingAnySkill() && bSkillAllowsMovement) || IsFastRunning() || IsSwitchingWeapon() || bCharacterStateAllowsMovement;
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

bool APlayerCharacter::CanGuardAgainstAttacks() const
{
	return (IsIdleOrMoving() || IsNormalAttacking()) && !(IsWeaponSheathed() || GetEquippedWeaponType() == EWeaponType::None);
}

bool APlayerCharacter::CanBlock() const
{
	return (IsIdleOrMoving() || IsAutoRunning() || IsNormalAttacking()) &&
		!(GetEquippedWeaponType() == EWeaponType::None || IsWeaponSheathed());
}

bool APlayerCharacter::CanNormalAttack() const
{
	// return IsIdleOrMoving() || IsNormalAttacking();
	return IsIdleOrMoving();
}

bool APlayerCharacter::CanUseAnySkill() const
{
	return (GetEquippedWeaponType() != EWeaponType::None) && !IsWeaponSheathed() && (IsIdleOrMoving() || IsBlocking() || IsFastRunning() || IsNormalAttacking());
}

bool APlayerCharacter::CanUseSkill(FSkillTableRow* Skill)
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

UHUDWidget* APlayerCharacter::BP_GetHUDWidget() const
{
	return GetHUDWidget();
}

bool APlayerCharacter::CCEFlinch_Implementation(const float BCAngle)
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

bool APlayerCharacter::CCEInterrupt_Implementation(const float BCAngle)
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

bool APlayerCharacter::CCEStun_Implementation(const float Duration)
{
	if (CanStun())
	{
		PlayStunAnimation();
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AEODCharacterBase::CCERemoveStun, Duration, false);

		return true;
	}

	return false;
}

void APlayerCharacter::CCERemoveStun_Implementation()
{
	StopStunAnimation();
	// @todo Restore character state to IdleWalkRun if necessary (if OnMontageBlendingOut event doesn't restore character state to IdleWalkRun)
}

bool APlayerCharacter::CCEFreeze_Implementation(const float Duration)
{
	// @todo maybe just freeze animation instead of freezing entire character since it might freeze additional effects like glow
	if (CanFreeze())
	{
		CustomTimeDilation = 0;
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AEODCharacterBase::CCEUnfreeze, Duration, false);

		return true;
	}

	return false;
}

void APlayerCharacter::CCEUnfreeze_Implementation()
{
	CustomTimeDilation = GetStatsComponent()->GetActiveTimeDilation();
}

bool APlayerCharacter::CCEKnockdown_Implementation(const float Duration)
{
	if (CanKnockdown() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->HitEffects.Get())
	{
		PlayAnimationMontage(GetActiveAnimationReferences()->HitEffects.Get(),
			UCharacterLibrary::SectionName_KnockdownStart,
			ECharacterState::GotHit);
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AEODCharacterBase::CCEEndKnockdown, Duration, false);

		return true;
	}

	return false;
}

void APlayerCharacter::CCEEndKnockdown_Implementation()
{
	PlayAnimationMontage(GetActiveAnimationReferences()->HitEffects.Get(),
		UCharacterLibrary::SectionName_KnockdownEnd,
		ECharacterState::GotHit);
}

bool APlayerCharacter::CCEKnockback_Implementation(const float Duration, const FVector & ImpulseDirection)
{
	if (CanKnockdown() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->HitEffects.Get())
	{
		PlayAnimationMontage(GetActiveAnimationReferences()->HitEffects.Get(),
			UCharacterLibrary::SectionName_KnockdownStart,
			ECharacterState::GotHit);
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AEODCharacterBase::CCEEndKnockdown, Duration, false);
		PushBack(ImpulseDirection);

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
		PushBack(ImpulseDirection);

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

void APlayerCharacter::ActivateStatusEffectFromWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset)
{
}

void APlayerCharacter::DeactivateStatusEffectFromWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset)
{
}

void APlayerCharacter::LoadWeaponAnimationReferences(FName WeaponID, UWeaponDataAsset* WeaponDataAsset)
{
	EWeaponType WeaponType = EWeaponType::None;
	if (WeaponDataAsset)
	{
		WeaponType = WeaponDataAsset->WeaponType;
	}

	// No need to load references if they are already loaded
	if (AnimationReferencesMap.Contains(WeaponDataAsset->WeaponType))
	{
		return;
	}




	/*
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
	*/
}

void APlayerCharacter::UnloadWeaponAnimationReferences(FName WeaponID, UWeaponDataAsset* WeaponDataAsset)
{
}

void APlayerCharacter::AddPrimaryWeapon(FName WeaponID)
{


	//  You would call SetCurrentPrimaryWeapon(NAME_None) when you want to remove equipped primary weapon
	if (WeaponID == NAME_None)
	{
		RemovePrimaryWeapon();
		// RemovePrimaryWeaponFromDataAsset();
		return;
	}

	UWeaponDataAsset* WeaponDataAsset = UWeaponLibrary::GetWeaponDataAsset(WeaponID);
	if (!WeaponDataAsset)
	{
		return;
	}

	RemovePrimaryWeapon();
	// RemovePrimaryWeaponFromDataAsset();
	if (UWeaponLibrary::IsWeaponDualHanded(WeaponDataAsset->WeaponType))
	{
		RemoveSecondaryWeapon();
		// RemoveSecondaryWeaponFromDataAsset();
	}

	PrimaryWeaponID = WeaponID;
	PrimaryWeaponDataAsset = WeaponDataAsset;
	
	OnPrimaryWeaponEquipped.Broadcast(PrimaryWeaponID, PrimaryWeaponDataAsset);

	/*
	LoadEquippedWeaponAnimationReferences();
	// @todo add weapon stats
	*/
}

void APlayerCharacter::AddSecondaryWeapon(FName WeaponID)
{
	//  You would call SetCurrentSecondaryWeapon(NAME_None) when you want to remove equipped secondary weapon
	if (WeaponID == NAME_None)
	{
		RemoveSecondaryWeapon();
		// RemoveSecondaryWeaponFromDataAsset();
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
	// RemoveSecondaryWeaponFromDataAsset();
	if (UWeaponLibrary::IsWeaponDualHanded(PrimaryWeapon->WeaponType))
	{
		RemovePrimaryWeapon();
		// RemovePrimaryWeaponFromDataAsset();
	}
	SecondaryWeaponID = WeaponID;
	SecondaryWeapon->OnEquip(WeaponID, WeaponData);

	
	// @todo add weapon stats

}

void APlayerCharacter::AddPrimaryWeaponToCurrentSlot(FName WeaponID)
{
	// @todo check whether a weapon is already equipped

	UWeaponDataAsset* WeaponDataAsset = UWeaponLibrary::GetWeaponDataAsset(WeaponID);
	if (WeaponDataAsset)
	{
		Server_AddPrimaryWeaponToCurrentSlot(WeaponID, WeaponDataAsset);
	}

	/*
	//  You would call AddPrimaryWeaponToCurrentSlot(NAME_None) when you want to remove equipped primary weapon
	if (WeaponID == NAME_None)
	{
		RemovePrimaryWeaponFromCurrentSlot();
		return;
	}

	UWeaponDataAsset* WeaponDataAsset = UWeaponLibrary::GetWeaponDataAsset(WeaponID);
	if (!WeaponDataAsset)
	{
		return;
	}

	RemovePrimaryWeaponFromCurrentSlot();
	if (UWeaponLibrary::IsWeaponDualHanded(WeaponDataAsset->WeaponType))
	{
		RemoveSecondaryWeaponFromCurrentSlot();
	}

	Server_AddPrimaryWeaponToCurrentSlot(WeaponID, WeaponDataAsset);
	*/

	/*
	UWeaponSlot* CurrentWeaponSlot = GetCurrentWeaponSlot() ? GetCurrentWeaponSlot() : CreateNewWeaponSlot();

	UWeaponSlot* WeaponSlot = GetCurrentWeaponSlot();
	if (WeaponSlot)
	{
		WeaponSlot->AttachPrimaryWeapon(WeaponID, WeaponDataAsset);
	}
	else
	{

	}



	PrimaryWeaponID = WeaponID;
	PrimaryWeaponDataAsset = WeaponDataAsset;

	OnPrimaryWeaponEquipped.Broadcast(PrimaryWeaponID, PrimaryWeaponDataAsset);
	*/
	/*
	LoadEquippedWeaponAnimationReferences();
	// @todo add weapon stats
	*/
}

void APlayerCharacter::AddPrimaryWeaponToCurrentSlot(FName WeaponID, UWeaponDataAsset * WeaponDataAsset)
{
}

void APlayerCharacter::AddSecondaryWeaponToCurrentSlot(FName WeaponID)
{
}

void APlayerCharacter::AddSecondaryWeaponToCurrentSlot(FName WeaponID, UWeaponDataAsset * WeaponDataAsset)
{
}

void APlayerCharacter::AddPrimaryWeaponToSlot(FName WeaponID, int32 SlotIndex)
{
}

void APlayerCharacter::AddPrimaryWeaponToSlot(FName WeaponID, UWeaponDataAsset* WeaponDataAsset, int32 SlotIndex)
{
}

void APlayerCharacter::AddSecondaryWeaponToSlot(FName WeaponID, int32 SlotIndex)
{

}

void APlayerCharacter::AddSecondaryWeaponToSlot(FName WeaponID, UWeaponDataAsset * WeaponDataAsset, int32 SlotIndex)
{
}

void APlayerCharacter::RemovePrimaryWeaponFromCurrentSlot()
{
}

void APlayerCharacter::RemoveSecondaryWeaponFromCurrentSlot()
{
}

void APlayerCharacter::RemovePrimaryWeaponFromSlot(int32 SlotIndex)
{
	/*
	if (SlotWeapons.Num() > SlotIndex)
	{
		FWeaponSlot WeaponSlot = SlotWeapons[SlotIndex];
		if (WeaponSlot.IsPrimaryWeaponAttached())
		{
			
		}
	}
	*/
}

void APlayerCharacter::RemoveSecondaryWeaponFromSlot(int32 SlotIndex)
{
}

void APlayerCharacter::ToggleWeaponSlot()
{
}

void APlayerCharacter::RemovePrimaryWeapon()
{
	OnPrimaryWeaponUnequipped.Broadcast(PrimaryWeaponID, PrimaryWeaponDataAsset);
	PrimaryWeaponID = NAME_None;
	PrimaryWeaponDataAsset = nullptr;	

	/*
	PrimaryWeapon->OnUnEquip();
	PrimaryWeaponID = NAME_None;
	UnloadEquippedWeaponAnimationReferences();
	*/

	// @todo remove weapon stats
}

void APlayerCharacter::RemoveSecondaryWeapon()
{
	OnSecondaryWeaponUnequipped.Broadcast(SecondaryWeaponID, SecondaryWeaponDataAsset);
	SecondaryWeaponID = NAME_None;
	SecondaryWeaponDataAsset = nullptr;

	/*
	SecondaryWeaponID = NAME_None;
	SecondaryWeapon->OnUnEquip();
	*/
	// @todo remove weapon stats
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
	if (GetCameraBoom()->TargetArmLength >= CameraArmMinimumLength)
		GetCameraBoom()->TargetArmLength -= CameraZoomRate;
}

void APlayerCharacter::ZoomOutCamera()
{
	if (GetCameraBoom()->TargetArmLength <= CameraArmMaximumLength)
		GetCameraBoom()->TargetArmLength += CameraZoomRate;
}

/*
UWeaponSlot* APlayerCharacter::CreateNewWeaponSlot()
{
	UWeaponSlot* NewSlot = NewObject<UWeaponSlot>((UObject*)GetTransientPackage(), UWeaponSlot::StaticClass());
	int32 NewSlotIndex = WeaponSlots.Add(NewSlot);
	if (WeaponSlots.Num() == 1)
	{
		SetActiveWeaponSlotIndex(NewSlotIndex);
	}
	return NewSlot;
}
*/

void APlayerCharacter::SetActiveWeaponSlotIndex(int32 NewSlotIndex)
{
	ActiveWeaponSlotIndex = NewSlotIndex;
}

bool APlayerCharacter::StartDodging()
{
	if (!GetActiveAnimationReferences() || !GetActiveAnimationReferences()->Dodge.Get())
	{
		return false;
	}

	// If the animations for dodge are missing
	UAnimMontage* DodgeMontage = GetActiveAnimationReferences()->Dodge.Get();
	if (IsIdleOrMoving() || IsBlocking() || IsCastingSpell() || IsNormalAttacking())
	{
		// Disable movement during dodge
		if (bCharacterStateAllowsMovement)
		{
			SetCharacterStateAllowsMovement(false);
		}

		// Rotate character
		float DesiredYaw = GetControllerRotationYaw();
		if (ForwardAxisValue != 0)
		{
			DesiredYaw = DesiredRotationYawFromAxisInput;
		}

		// Instantly rotate character
		SetCharacterRotation(FRotator(0.f, DesiredYaw, 0.f));
		// Update desired rotation yaw in movement component so it doesn't try to rotate back to original rotation yaw
		UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
		if (MoveComp)
		{
			MoveComp->SetDesiredCustomRotationYaw(DesiredYaw);
		}

		FName SectionToPlay;
		if (ForwardAxisValue == 0)
		{
			if (RightAxisValue > 0)
			{
				SectionToPlay = UCharacterLibrary::SectionName_RightDodge;
			}
			else if (RightAxisValue < 0)
			{
				SectionToPlay = UCharacterLibrary::SectionName_LeftDodge;
			}
			else
			{
				SectionToPlay = UCharacterLibrary::SectionName_BackwardDodge;
			}
		}
		else
		{
			if (ForwardAxisValue > 0)
			{
				SectionToPlay = UCharacterLibrary::SectionName_ForwardDodge;
			}
			else if (ForwardAxisValue < 0)
			{
				SectionToPlay = UCharacterLibrary::SectionName_BackwardDodge;
			}
		}
		PlayAnimationMontage(DodgeMontage, SectionToPlay, ECharacterState::Dodging);
		TriggeriFrames(DodgeImmunityDuration, DodgeImmunityTriggerDelay);
		return true;
	}

	return false;
}

void APlayerCharacter::OnDodge()
{
	// Alternative to disable auto run
	if (bUseControllerRotationYaw)
	{
		SetUseControllerRotationYaw(false);
	}

	if (CanDodge() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->Dodge.Get())
	{
		int32 DodgeCost = DodgeStaminaCost * GetStatsComponent()->GetStaminaConsumptionModifier();
		GetStatsComponent()->ModifyCurrentStamina(-DodgeCost);

		// float ForwardAxisValue = InputComponent->GetAxisValue(FName("MoveForward"));
		// float RightAxisValue = InputComponent->GetAxisValue(FName("MoveRight"));
		float DesiredYaw = GetControllerRotationYaw();

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
	if (CanJump())
	{
		if (bUseControllerRotationYaw)
		{
			SetUseControllerRotationYaw(false);
		}

		if (IsAutoRunning())
		{
			DisableAutoRun();
		}

		Jump();
		SetCharacterState(ECharacterState::Jumping);
	}
}

void APlayerCharacter::OnInteract()
{
	// If player is already interacting
	if (GetCharacterState() == ECharacterState::Interacting)
	{
		UpdateInteraction();
		// EndInteraction();
	}
	else
	{
		StartInteraction();
	}
}

void APlayerCharacter::ToggleSheathe()
{
	if (CanToggleSheathe())
	{
		bool bNewValue = !IsWeaponSheathed();
		SetWeaponSheathed(bNewValue);
		SetCharacterStateAllowsMovement(true);
	}
}

void APlayerCharacter::PlayToggleSheatheAnimation()
{
	if (!EquippedWeaponAnimationReferences)
	{
		return;
	}
	
	UAnimMontage* MontageToPlay = IsPCTryingToMove() ? EquippedWeaponAnimationReferences->WeaponSwitchUpperBody.Get() :
		EquippedWeaponAnimationReferences->WeaponSwitchFullBody.Get();
	FName SectionToPlay = IsWeaponSheathed() ? UCharacterLibrary::SectionName_SheatheWeapon :
		UCharacterLibrary::SectionName_UnsheatheWeapon;

	UPlayerAnimInstance* PlayerAnimInstance = GetMesh()->GetAnimInstance() ? Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance()) : nullptr;
	if (PlayerAnimInstance)
	{
		PlayerAnimInstance->Montage_Play(MontageToPlay);
		PlayerAnimInstance->Montage_JumpToSection(SectionToPlay);
		PlayerAnimInstance->OnTransitionableMontageTriggered(false);
		SetCharacterState(ECharacterState::SwitchingWeapon);
	}
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
	SetNormalAttackKeyPressed(true);

	/*
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
		SetCharacterRotation(FRotator(0.f, GetControllerRotationYaw(), 0.f));
	}
	else if (!IsNormalAttacking() && bBackwardPressed)
	{
		PlayAnimationMontage(NormalAttackMontage,
			UCharacterLibrary::SectionName_BackwardSPSwing,
			ECharacterState::Attacking);
		SetCharacterRotation(FRotator(0.f, GetControllerRotationYaw(), 0.f));
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
	*/
}

void APlayerCharacter::OnReleasedNormalAttack()
{
	SetNormalAttackKeyPressed(false);
}

void APlayerCharacter::DoNormalAttack()
{
	if (!GetActiveAnimationReferences() || !GetActiveAnimationReferences()->NormalAttacks.Get())
	{
		return;
	}

	UAnimMontage* NormalAttackMontage = GetActiveAnimationReferences()->NormalAttacks.Get();
	if (bForwardPressed)
	{
		PlayAnimationMontage(NormalAttackMontage,
			UCharacterLibrary::SectionName_ForwardSPSwing,
			ECharacterState::Attacking);
		SetCharacterRotation(FRotator(0.f, GetControllerRotationYaw(), 0.f));
	}
	else if (bBackwardPressed)
	{
		PlayAnimationMontage(NormalAttackMontage,
			UCharacterLibrary::SectionName_BackwardSPSwing,
			ECharacterState::Attacking);
		SetCharacterRotation(FRotator(0.f, GetControllerRotationYaw(), 0.f));
	}
	else
	{
		PlayAnimationMontage(NormalAttackMontage,
			UCharacterLibrary::SectionName_FirstSwing,
			ECharacterState::Attacking);
	}
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
	if (GetCharacterMovementDirection() != ECharMovementDirection::None)
	{
		SetCharacterMovementDirection(ECharMovementDirection::None);
	}
}

/*
void APlayerCharacter::UpdateMovement(float DeltaTime)
{
	float ActorRotationYaw = GetActorRotation().Yaw;
	float DesiredPlayerRotationYaw = GetRotationYawFromAxisInput();
			
	bool bRotatePlayer = DesiredPlayerRotationYaw == ActorRotationYaw ? false : true;

	float ForwardAxisValue = GetController()->InputComponent->GetAxisValue(FName("MoveForward"));
	float RightAxisValue = GetController()->InputComponent->GetAxisValue(FName("MoveRight"));

	// float ForwardAxisValue = InputComponent->GetAxisValue(FName("MoveForward"));
	// float RightAxisValue = InputComponent->GetAxisValue(FName("MoveRight"));
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
		float Speed = (DefaultWalkSpeed * GetStatsComponent()->GetMovementSpeedModifier() * 5) / 16;
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			SetWalkSpeed(Speed);
		}
	}
	else
	{
		float Speed = DefaultWalkSpeed * GetStatsComponent()->GetMovementSpeedModifier();
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			SetWalkSpeed(Speed);
		}
	}

	if (ForwardAxisValue == 0)
	{
		if (RightAxisValue > 0 && GetCharacterMovementDirection() != ECharMovementDirection::R)
		{
			SetCharacterMovementDirection(ECharMovementDirection::R);
		}
		else if (RightAxisValue < 0 && GetCharacterMovementDirection() != ECharMovementDirection::L)
		{
			SetCharacterMovementDirection(ECharMovementDirection::L);
		}
	}
	else
	{
		if (ForwardAxisValue > 0 && GetCharacterMovementDirection() != ECharMovementDirection::F)
		{
			SetCharacterMovementDirection(ECharMovementDirection::F);
		}
		else if (ForwardAxisValue < 0 && GetCharacterMovementDirection() != ECharMovementDirection::B)
		{
			SetCharacterMovementDirection(ECharMovementDirection::B);
		}
	}
}
*/

/*
void APlayerCharacter::UpdateGuardState(float DeltaTime)
{
	// float ForwardAxisValue = InputComponent->GetAxisValue(FName("MoveForward"));
	// float RightAxisValue = InputComponent->GetAxisValue(FName("MoveRight"));
	
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
*/

void APlayerCharacter::UpdateFastMovementState(float DeltaTime)
{
	float ActorRotationYaw = GetActorRotation().Yaw;
	float DesiredPlayerRotationYaw = GetRotationYawFromAxisInput();

	bool bRotatePlayer = DesiredPlayerRotationYaw == ActorRotationYaw ? false : true;

	// float ForwardAxisValue = InputComponent->GetAxisValue(FName("MoveForward"));
	// float RightAxisValue = InputComponent->GetAxisValue(FName("MoveRight"));
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
		float Speed = (DefaultWalkSpeed * GetStatsComponent()->GetMovementSpeedModifier() * 5) / 16;
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			SetWalkSpeed(Speed);
		}
	}
	else
	{
		float Speed = DefaultRunSpeed * GetStatsComponent()->GetMovementSpeedModifier();
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			SetWalkSpeed(Speed);
		}
	}

	if (ForwardAxisValue == 0)
	{
		if (RightAxisValue > 0 && GetCharacterMovementDirection() != ECharMovementDirection::R)
		{
			SetCharacterMovementDirection(ECharMovementDirection::R);
		}
		else if (RightAxisValue < 0 && GetCharacterMovementDirection() != ECharMovementDirection::L)
		{
			SetCharacterMovementDirection(ECharMovementDirection::L);
		}
	}
	else
	{
		if (ForwardAxisValue > 0 && GetCharacterMovementDirection() != ECharMovementDirection::F)
		{
			SetCharacterMovementDirection(ECharMovementDirection::F);
		}
		else if (ForwardAxisValue < 0 && GetCharacterMovementDirection() != ECharMovementDirection::B)
		{
			SetCharacterMovementDirection(ECharMovementDirection::B);
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
	if (GetCharacterMovementDirection() != ECharMovementDirection::F)
	{
		SetCharacterMovementDirection(ECharMovementDirection::F);
	}
	
	float Speed = DefaultWalkSpeed * GetStatsComponent()->GetMovementSpeedModifier();
	if (GetCharacterMovement()->MaxWalkSpeed != Speed)
	{
		SetWalkSpeed(Speed);
	}
}

void APlayerCharacter::StartNormalAttack()
{
	if (bCharacterStateAllowsMovement)
	{
		SetCharacterStateAllowsMovement(false);
	}

	// Determine what normal attack section should we start with
	FName SectionToPlay;
	if (bForwardPressed)
	{
		SectionToPlay = UCharacterLibrary::SectionName_ForwardSPSwing;
	}
	else if (bBackwardPressed)
	{
		SectionToPlay = UCharacterLibrary::SectionName_BackwardSPSwing;
	}
	else
	{
		SectionToPlay = UCharacterLibrary::SectionName_FirstSwing;
	}

	PlayNormalAttackAnimation(NAME_None, SectionToPlay);
	// @note The rotation is handled through the AnimNotify_NormalAttack that gets called when a normal attack starts.
}

void APlayerCharacter::StopNormalAttack()
{
	GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, GetActiveAnimationReferences()->NormalAttacks.Get());
}

void APlayerCharacter::UpdateNormalAttackState(float DeltaTime)
{
	/*
	if (!GetActiveAnimationReferences() || !GetActiveAnimationReferences()->NormalAttacks.Get())
	{
		return;
	}

	UAnimMontage* NormalAttackMontage = GetActiveAnimationReferences()->NormalAttacks.Get();
	if (bNormalAttackSectionChangeAllowed)
	{
		FName CurrentSection = GetMesh()->GetAnimInstance()->Montage_GetCurrentSection(NormalAttackMontage);
		FName ExpectedNextSection = GetNextNormalAttackSectionName(CurrentSection);

		if (ExpectedNextSection != NAME_None)
		{
			FString CurrentSectionString = CurrentSection.ToString();
			if (CurrentSectionString.EndsWith("End"))
			{
				PlayAnimationMontage(NormalAttackMontage, ExpectedNextSection);
				// PlayAnimationMontage(NormalAttackMontage, ExpectedNextSection, ECharacterState::Attacking);
			}
			else
			{
				SetNextMontageSection(CurrentSection, ExpectedNextSection);
				// SetNextMontageSection(CurrentSection, NextSection);
			}
		}
		bNormalAttackSectionChangeAllowed = false;
	}
	*/
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

void APlayerCharacter::DisplayDialogueWidget(FName DialogueWindowID)
{
	if (DialogueWidgetClass.Get())
	{
		DialogueWidget = CreateWidget<UDialogueWindowWidget>(GetGameInstance(), DialogueWidgetClass);
		if (DialogueWidget && HUDWidget)
		{
			HUDWidget->AddDialogueWidget(DialogueWidget);
			HUDWidget->GetSkillBarWidget()->SetVisibility(ESlateVisibility::Hidden);
			HUDWidget->GetInventoryWidget()->SetVisibility(ESlateVisibility::Hidden);
			HUDWidget->GetSkillTreeWidget()->SetVisibility(ESlateVisibility::Hidden);

			DialogueWidget->UpdateDialogueWindow(DialogueWindowID);

			AudioComponent->SetSound(DialogueTriggeredSound);
			AudioComponent->Play();
		}
	}
}

void APlayerCharacter::RemoveDialogueWidget()
{
	if (DialogueWidget)
	{
		DialogueWidget->RemoveFromParent();
		DialogueWidget->MarkPendingKill(); // Would this crash?
		DialogueWidget = nullptr;
	}

	if (HUDWidget)
	{
		HUDWidget->GetSkillBarWidget()->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerCharacter::PlayNormalAttackAnimation(FName OldSection, FName NewSection)
{
	if (!GetActiveAnimationReferences() || !GetActiveAnimationReferences()->NormalAttacks.Get())
	{
		return;
	}
	UAnimMontage* NormalAttackMontage = GetActiveAnimationReferences()->NormalAttacks.Get();
	if (GetMesh()->GetAnimInstance())
	{
		if (OldSection == NAME_None)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(NormalAttackMontage);
			GetMesh()->GetAnimInstance()->Montage_JumpToSection(NewSection, NormalAttackMontage);
			SetCharacterState(ECharacterState::Attacking);
		}
		else
		{

		}
	}

	Server_PlayNormalAttackAnimation(OldSection, NewSection);
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
		AssetsToLoad.Add(AnimationReferences->Flinch.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->HitEffects.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->NormalAttacks.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->Jump.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->Dodge.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->SpecialActions.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->Skills.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->Spells.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->BlockAttack.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->WeaponSwitchFullBody.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->WeaponSwitchUpperBody.ToSoftObjectPath());
		AssetsToLoad.Add(AnimationReferences->Die.ToSoftObjectPath());
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
	// bool bSkillBeingUsedWhileHit;
	TPair<FName, FSkillTableRow*> SkillPair(NAME_None, nullptr);
	if (CanUseAnySkill())
	{
		SkillPair = GetSkillsComponent()->GetSkillFromSkillSlot(SkillButtonIndex);
	}
	else if (IsUsingAnySkill() && bCanUseChainSkill)
	{
		SkillPair = GetSkillsComponent()->GetChainSkillFromSkillSlot(SkillButtonIndex);
	}
	else if (HasBeenHit())
	{
		SkillPair = GetSkillsComponent()->GetHitImmuneSkillFromSkillSlot(SkillButtonIndex);
	}

	/*
	if (!CanUseAnySkill())
	{
#if MESSAGE_LOGGING_ENABLED
		UKismetSystemLibrary::PrintString(this, FString("Can't use any skill right now"));
#endif // MESSAGE_LOGGING_ENABLED
		return;
	}
	*/

	// TPair<FName, FSkillTableRow*> SkillPair = SkillsComponent->GetSkillFromSkillSlot(SkillButtonIndex);
	// There isn't any skill to use
	if (SkillPair.Key == NAME_None || SkillPair.Value == nullptr)
	{
		return;
	}

	if (IsBlocking())
	{
		DisableBlock();
	}

	GetStatsComponent()->ModifyCurrentMana(-SkillPair.Value->ManaRequired);
	GetStatsComponent()->ModifyCurrentStamina(-SkillPair.Value->StaminaRequired);

	SetCurrentActiveSkillID(SkillPair.Key);
	SetCurrentActiveSkill(SkillPair.Value);
	GetSkillsComponent()->OnSkillUsed(SkillButtonIndex, SkillPair.Key, SkillPair.Value);

	bSkillAllowsMovement = SkillPair.Value->bAllowsMovement;
	bSkillHasDirectionalAnimations = SkillPair.Value->bHasDirectionalAnimations;

	// @todo cleaning of crowd control effect timer needs to be handled better
	GetWorld()->GetTimerManager().ClearTimer(CrowdControlTimerHandle);
	// If frozen
	if (CustomTimeDilation == 0.f)
	{
		EndFreeze();
	}

	GetStatsComponent()->AddCrowdControlImmunitiesFromSkill(SkillPair.Value->CrowdControlImmunities);

	if (bSkillAllowsMovement)
	{
		if (IsIdle())
		{
			if (SkillPair.Value->AnimMontage.Get())
			{
				PlayAnimationMontage(SkillPair.Value->AnimMontage.Get(), SkillPair.Value->SkillStartMontageSectionName, ECharacterState::UsingActiveSkill);
			}
		}
		else if (IsMoving() && !bSkillHasDirectionalAnimations)
		{
			if (SkillPair.Value->UpperBodyAnimMontage.Get())
			{
				PlayAnimationMontage(SkillPair.Value->UpperBodyAnimMontage.Get(), SkillPair.Value->SkillStartMontageSectionName, ECharacterState::UsingActiveSkill);
			}
		}
		else if (IsMoving() && bSkillHasDirectionalAnimations)
		{
			if (SkillPair.Value->UpperBodyAnimMontage.Get())
			{
				FString SectionSuffixString;
				if (GetCharacterMovementDirection() == ECharMovementDirection::F)
				{
					SectionSuffixString = FString("_F");
				}
				else if (GetCharacterMovementDirection() == ECharMovementDirection::B)
				{
					SectionSuffixString = FString("_B");
				}
				else if (GetCharacterMovementDirection() == ECharMovementDirection::L)
				{
					SectionSuffixString = FString("_L");
				}
				else if (GetCharacterMovementDirection() == ECharMovementDirection::R)
				{
					SectionSuffixString = FString("_R");
				}
				else
				{
					SectionSuffixString = FString("_F");
				}

				FName SectionToPlay = FName(*(SkillPair.Value->SkillStartMontageSectionName.ToString() + SectionSuffixString));
				PlayAnimationMontage(SkillPair.Value->UpperBodyAnimMontage.Get(), SectionToPlay, ECharacterState::UsingActiveSkill);
			}
		}
	}
	else
	{
		if (SkillPair.Value->AnimMontage.Get())
		{
			SetOffSmoothRotation(GetControllerRotationYaw());
			PlayAnimationMontage(SkillPair.Value->AnimMontage.Get(), SkillPair.Value->SkillStartMontageSectionName, ECharacterState::UsingActiveSkill);
		}
	}
}

void APlayerCharacter::OnReleasingSkillKey(const uint32 SkillButtonIndex)
{
}

void APlayerCharacter::BP_SetCanUseChainSkill(bool bNewValue)
{
	SetCanUseChainSkill(bNewValue);
}

void APlayerCharacter::BP_SetNormalAttackSectionChangeAllowed(bool bNewValue)
{
	SetNormalAttackSectionChangeAllowed(bNewValue);
}

void APlayerCharacter::OnInteractionSphereBeginOverlap_Implementation(AActor* OtherActor)
{
	IInteractionInterface* InteractiveObj = Cast<IInteractionInterface>(OtherActor);
	// If the actor is not interactive
	if (!InteractiveObj)
	{
		return;
	}

	if (ActiveInteractiveActor)
	{
		IInteractionInterface* OldInteractiveObj = Cast<IInteractionInterface>(ActiveInteractiveActor);
		OldInteractiveObj->Execute_DisableCustomDepth(ActiveInteractiveActor);
	}

	InteractiveObj->Execute_EnableCustomDepth(OtherActor);
	ActiveInteractiveActor = OtherActor;
	OverlappingInteractiveActors.Add(OtherActor);

	AudioComponent->SetSound(InteractiveActorDetectedSound);
	AudioComponent->Play();
}

void APlayerCharacter::OnInteractionSphereEndOverlap_Implementation(AActor* OtherActor)
{
	IInteractionInterface* InteractiveObj = Cast<IInteractionInterface>(OtherActor);
	// If the actor is not interactive
	if (!InteractiveObj)
	{
		return;
	}

	InteractiveObj->Execute_DisableCustomDepth(OtherActor);
	OverlappingInteractiveActors.Remove(OtherActor);

	if (ActiveInteractiveActor == OtherActor)
	{
		if (OverlappingInteractiveActors.Num() > 0)
		{
			ActiveInteractiveActor = OverlappingInteractiveActors[OverlappingInteractiveActors.Num() - 1];
			IInteractionInterface* NewInteractiveObj = Cast<IInteractionInterface>(ActiveInteractiveActor);
			NewInteractiveObj->Execute_EnableCustomDepth(ActiveInteractiveActor);

			AudioComponent->SetSound(InteractiveActorDetectedSound);
			AudioComponent->Play();
		}
		else
		{
			ActiveInteractiveActor = nullptr;
		}
	}
}

void APlayerCharacter::StartInteraction()
{
	if (ActiveInteractiveActor)
	{
		IInteractionInterface* InteractiveObj = Cast<IInteractionInterface>(ActiveInteractiveActor);
		if (InteractiveObj)
		{
			InteractiveObj->Execute_OnInteract(ActiveInteractiveActor, this);
		}
	}

	/*
	AEODPlayerController* PC = Cast<AEODPlayerController>(GetController());
	if (PC && ActiveInteractiveActor)
	{
		IInteractionInterface* InteractiveObj = Cast<IInteractionInterface>(ActiveInteractiveActor);
		// Just in case, make sure 'ActiveInteractiveActor' implements 'IInteractionInterface'
		if (InteractiveObj)
		{
			PC->SetViewTargetWithBlend(ActiveInteractiveActor, 0.5, EViewTargetBlendFunction::VTBlend_Linear, 0.f, true);
			SetCharacterState(ECharacterState::Interacting);

			DisplayDialogueWidget();

			AudioComponent->SetSound(InteractionStartSound);
			AudioComponent->Play();

			InteractiveObj->Execute_OnInteract(ActiveInteractiveActor, this);
			// InteractiveObj->Execute_OnInteract(ActiveInteractiveActor, this, DialogueWidget);
		}
	}
	*/
}

// void APlayerCharacter::UpdateInteraction_Implementation()
void APlayerCharacter::UpdateInteraction()
{
	UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
	if (GameSingleton && DialogueWidget && DialogueWidget->GetDialogueWindowID() != NAME_None)
	{
		FDialogueWindow* DialogueWin = GameSingleton->DialogueWindowsDataTable->FindRow<FDialogueWindow>(DialogueWidget->GetDialogueWindowID(), FString("APlayerCharacter::UpdateInteraction()"));
		if (DialogueWin->NextEventID == NAME_None)
		{
			if (DialogueWidget->DialogueOptions.Num() > 0)
			{
				DialogueWidget->SimulateSelectedOptionClick();
			}
			else
			{
				EndInteraction();
			}
		}
		else
		{
			DialogueWidget->UpdateDialogueWindow(DialogueWin->NextEventID);

			AudioComponent->SetSound(DialogueTriggeredSound);
			AudioComponent->Play();
		}
	}
}

void APlayerCharacter::RequestDialogue_Implementation(AActor* Requestor, FName DialogueWindowID)
{
	FocusCameraOnActor(Requestor);
	DisplayDialogueWidget(DialogueWindowID);
	SetCharacterState(ECharacterState::Interacting);
}

void APlayerCharacter::FocusCameraOnActor(AActor* TargetActor)
{
	AEODPlayerController* PC = Cast<AEODPlayerController>(GetController());
	if (PC && TargetActor)
	{
		PC->SetViewTargetWithBlend(TargetActor, 0.5, EViewTargetBlendFunction::VTBlend_Linear, 0.f, true);
	}
}

/*
void APlayerCharacter::UpdateActiveInteraction()
{
	if (DialogueWidget)
	{
		UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
		FDialogueWindow* DialogueWin = GameSingleton->DialogueWindowsDataTable->FindRow<FDialogueWindow>(DialogueWidget->GetDialogueWindowID(), FString("APlayerCharacter::UpdateActiveInteraction()"));
		if (DialogueWin->NextEventID != NAME_None)
		{


			// @todo handle next event
			return;
		}
	}
	
	EndInteraction();
}
*/

void APlayerCharacter::EndInteraction()
{
	AEODPlayerController* PC = Cast<AEODPlayerController>(GetController());
	if (PC)
	{
		PC->SetViewTargetWithBlend(this, 0.5, EViewTargetBlendFunction::VTBlend_Linear, 0.f, true);
		SetCharacterState(ECharacterState::IdleWalkRun);

		RemoveDialogueWidget();

		AudioComponent->SetSound(DialogueEndSound);
		// AudioComponent->SetSound(InteractionEndSound);
		AudioComponent->Play();
	}
}

void APlayerCharacter::FinishDialogue_Implementation(UDialogueWindowWidget* Widget)
{
}

void APlayerCharacter::ExitDialogue_Implementation(UDialogueWindowWidget* Widget)
{
}

void APlayerCharacter::OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (GetController() && GetController()->IsLocalPlayerController())
	{
		// @todo
	}

	if (!bInterrupted  && GetCharacterState() != ECharacterState::Interacting)
	{
		SetCharacterState(ECharacterState::IdleWalkRun);
	}

	// @todo handle skill montage blending out better because montages might be blending out in between transition of standstill and moving animations
	if (GetCurrentActiveSkill() && GetCurrentActiveSkill()->AnimMontage.Get() == AnimMontage)
	{
		GetSkillsComponent()->SetOffChainSkillReset();

		if (GetCurrentActiveSkill()->CrowdControlImmunities == GetStatsComponent()->GetCrowdControlImmunitiesFromSkill())
		{
			GetStatsComponent()->RemoveCrowdControlImmunitiesFromSkil();
		}
	}

	if (GetActiveAnimationReferences()->NormalAttacks.Get() == AnimMontage)
	{
		bNormalAttackSectionChangeAllowed = false;
	}

	// On disable chain skill if the montage blending out is not a flinch montage
	if (GetActiveAnimationReferences()->Flinch.Get() != AnimMontage)
	{
		SetCanUseChainSkill(false);
	}

	if (IsValid(GetGameplaySkillsComponent()) && IsValid(GetController()) && GetController()->IsLocalPlayerController())
	{
		GetGameplaySkillsComponent()->SetCurrentActiveSkill(NAME_None);
	}
}

void APlayerCharacter::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
}

void APlayerCharacter::EquipPrimaryWeapon(FName WeaponID)
{
	SetPrimaryWeaponID(WeaponID);
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

void APlayerCharacter::TurnOnTargetSwitch()
{
	// Empty override to prevent call of Super::TurnOffTargetSwitch()
}

void APlayerCharacter::TurnOffTargetSwitch()
{
	// Empty override to prevent call of Super::TurnOffTargetSwitch()
}

void APlayerCharacter::OnSkillGroupAddedToSkillBar(const FString & SkillGroup)
{
	GetSkillsComponent()->OnSkillGroupAddedToSkillBar(SkillGroup);
}

void APlayerCharacter::OnSkillGroupRemovedFromSkillBar(const FString & SkillGroup)
{
	GetSkillsComponent()->OnSkillGroupRemovedFromSkillBar(SkillGroup);
}

void APlayerCharacter::OnNormalAttackSectionStart(FName SectionName)
{
	FString SkillIDString = FString("");
	SkillIDString += GetGenderPrefix();
	SkillIDString += GetWeaponPrefix();
	SkillIDString += GetNormalAttackSuffix(SectionName);

	if (IsValid(GetGameplaySkillsComponent()) && IsValid(GetController()) && GetController()->IsLocalPlayerController())
	{
		GetGameplaySkillsComponent()->SetCurrentActiveSkill(FName(*SkillIDString));
	}

	// @todo set current active skill
}

void APlayerCharacter::OnRep_PrimaryWeaponID()
{
	SetCurrentPrimaryWeapon(PrimaryWeaponID);
}

void APlayerCharacter::OnRep_SecondaryWeaponID()
{
}

void APlayerCharacter::Server_PlayNormalAttackAnimation_Implementation(FName OldSection, FName NewSection)
{
	Multicast_PlayNormalAttackAnimation(OldSection, NewSection);
}

bool APlayerCharacter::Server_PlayNormalAttackAnimation_Validate(FName OldSection, FName NewSection)
{
	return true;
}

void APlayerCharacter::Multicast_PlayNormalAttackAnimation_Implementation(FName OldSection, FName NewSection)
{
	if (IsLocallyControlled() || !GetActiveAnimationReferences() || !GetActiveAnimationReferences()->NormalAttacks.Get())
	{
		return;
	}
	UAnimMontage* NormalAttackMontage = GetActiveAnimationReferences()->NormalAttacks.Get();
	if (GetMesh()->GetAnimInstance())
	{
		if (OldSection == NAME_None)
		{
			GetMesh()->GetAnimInstance()->Montage_Play(NormalAttackMontage);
			GetMesh()->GetAnimInstance()->Montage_JumpToSection(NewSection, NormalAttackMontage);
			SetCharacterState(ECharacterState::Attacking);
		}
		else
		{

		}
	}
}

/*
void APlayerCharacter::OnRep_WeaponSlots(TArray<UWeaponSlot*> OldWeaponSlots)
{
	UKismetSystemLibrary::PrintString(this, FString("Weapon slots got replicated"));

	FString Message = FString::FromInt(WeaponSlots.Num());
	UKismetSystemLibrary::PrintString(this, Message);

	ActiveWeaponSlotIndex = 0;
	UWeaponSlot* WeaponSlot = WeaponSlots[ActiveWeaponSlotIndex];

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = this;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APrimaryWeapon* Wep = GetWorld()->SpawnActor<APrimaryWeapon>(APrimaryWeapon::StaticClass(), SpawnInfo);

	// UKismetSystemLibrary::PrintString(this, WeaponSlot->PrimaryWeaponID.ToString());

	if (WeaponSlot)
	{
		WeaponSlot->AttachPrimaryWeapon(Wep);
	}
}
*/

void APlayerCharacter::Server_SetPrimaryWeaponID_Implementation(FName NewWeaponID)
{
	SetPrimaryWeaponID(NewWeaponID);
}

bool APlayerCharacter::Server_SetPrimaryWeaponID_Validate(FName NewWeaponID)
{
	return true;
}

void APlayerCharacter::Server_AddPrimaryWeaponToCurrentSlot_Implementation(FName WeaponID, UWeaponDataAsset* WeaponDataAsset)
{
	UKismetSystemLibrary::PrintString(this, FString("Server RPC called"));

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = this;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APrimaryWeapon* Wep = GetWorld()->SpawnActor<APrimaryWeapon>(APrimaryWeapon::StaticClass(), SpawnInfo);

	Multicast_AddPrimaryWeaponToCurrentSlot(WeaponID, WeaponDataAsset, Wep);
}

bool APlayerCharacter::Server_AddPrimaryWeaponToCurrentSlot_Validate(FName WeaponID, UWeaponDataAsset* WeaponDataAsset)
{
	return true;
}

void APlayerCharacter::Multicast_AddPrimaryWeaponToCurrentSlot_Implementation(FName WeaponID, UWeaponDataAsset* WeaponDataAsset, AWeaponBase* PrimaryWep)
{
	UKismetSystemLibrary::PrintString(this, FString("Multicast called"));

	/*
	PrimaryWeaponID = WeaponID;
	UWeaponSlot* WeaponSlot = GetCurrentWeaponSlot();
	if (!WeaponSlot)
	{
		// WeaponSlot = NewObject<UWeaponSlot>((UObject*)GetTransientPackage(), UWeaponSlot::StaticClass());
		WeaponSlot = NewObject<UWeaponSlot>(this, UWeaponSlot::StaticClass(), FName("SlotObj"), RF_Transient);
	}

	if (WeaponSlot)
	{
		WeaponSlot->AttachPrimaryWeapon(WeaponID, WeaponDataAsset, PrimaryWep);
	}

	if (WeaponSlot)
	{
		ActiveWeaponSlotIndex = WeaponSlots.Add(WeaponSlot);
	}
	*/
}
