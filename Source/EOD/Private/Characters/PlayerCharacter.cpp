// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerCharacter.h"
#include "PlayerAnimInstance.h"
#include "GameSingleton.h"
#include "WeaponLibrary.h"
#include "SkillsComponent.h"
#include "InventoryComponent.h"
#include "PlayerStatsComponent.h"
#include "EODPreprocessors.h"
#include "EODSaveGame.h"
#include "HUDWidget.h"
#include "NPCBase.h"
#include "EODPlayerController.h"
#include "DialogueWindowWidget.h"
#include "DialogueLibrary.h"
#include "WeaponDataAsset.h"
#include "GameplaySkillsComponent.h"
#include "EODCharacterMovementComponent.h"
#include "PlayerSkillsComponent.h"

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
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerSkillsComponent>(AEODCharacterBase::GameplaySkillsComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	// The player should be free to look around with mouse without actually rotating the posessed character
	bUseControllerRotationYaw = false;

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

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, PrimaryWeaponID);

}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	LoadUnequippedWeaponAnimationReferences();
	//~ @todo
	/*
	if (IsValid(GetCharacterStatsComponent()))
	{
		SetWalkSpeed(DefaultWalkSpeed * GetCharacterStatsComponent()->GetMovementSpeedModifier());
	}
	else
	{
		SetWalkSpeed(DefaultWalkSpeed);
	}
	*/

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = this;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PrimaryWeapon = GetWorld()->SpawnActor<APrimaryWeapon>(APrimaryWeapon::StaticClass(), SpawnInfo);
	SecondaryWeapon = GetWorld()->SpawnActor<ASecondaryWeapon>(ASecondaryWeapon::StaticClass(), SpawnInfo);

	// @note Set secondary weapon first and primary weapon later during initialization
	SetCurrentSecondaryWeapon(SecondaryWeaponID);
	SetCurrentPrimaryWeapon(PrimaryWeaponID);


	if (GetGameplaySkillsComponent())
	{
		GetGameplaySkillsComponent()->InitializeSkills(this);
	}
}

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

	// SetMasterPoseComponentForMeshes();

	/*
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
	*/
}

bool APlayerCharacter::CanMove() const
{
	// @todo, uncomment and replace return code with following comment:
	// return IsIdleOrMoving() || IsRunning() || bCharacterStateAllowsMovement;
	// return IsIdleOrMoving() || IsBlocking() || IsAutoRunning() || (IsUsingAnySkill() && bSkillAllowsMovement) || IsFastRunning() || IsSwitchingWeapon() || bCharacterStateAllowsMovement;
	// return IsIdleOrMoving() || IsBlocking() || (IsUsingAnySkill() && bSkillAllowsMovement) || IsFastRunning() || IsSwitchingWeapon() || bCharacterStateAllowsMovement;
	return IsIdleOrMoving() || IsBlocking() || IsFastRunning() || IsSwitchingWeapon() || bCharacterStateAllowsMovement;
}

bool APlayerCharacter::CanJump() const
{
	return IsIdleOrMoving() || IsBlocking();
	// return IsIdleOrMoving() || IsBlocking() || IsAutoRunning();
}

bool APlayerCharacter::CanDodge() const
{
	FPlayerAnimationReferencesTableRow* AnimationRef = GetActiveAnimationReferences();
	UAnimMontage* Animation = AnimationRef ? AnimationRef->Dodge.Get() : nullptr;

	// @todo add UsingSkill, Looting, Interacting, etc. to this too
	// bool bStateAllowsDodge = IsIdleOrMoving() || IsGuardActive() || IsCastingSpell() || IsNormalAttacking();

	// If we have a valid animation for dodge and the character state allows dodging
	// return Animation && bStateAllowsDodge;

	//~ @todo
	return true;
}

bool APlayerCharacter::CanGuardAgainstAttacks() const
{
	return (IsIdleOrMoving() || IsNormalAttacking()) && !(IsWeaponSheathed() || GetEquippedWeaponType() == EWeaponType::None);
}

bool APlayerCharacter::CanNormalAttack() const
{
	return IsIdleOrMoving() &&
		// The player must have a weapon equipped to normal attack and it shouldn't be sheathed
		(GetEquippedWeaponType() != EWeaponType::None && !IsWeaponSheathed());
}

bool APlayerCharacter::CanUseAnySkill() const
{
	return (GetEquippedWeaponType() != EWeaponType::None) && !IsWeaponSheathed() && (IsIdleOrMoving() || IsBlocking() || IsFastRunning() || IsNormalAttacking());
}

bool APlayerCharacter::CanUseSkill(FSkillTableRow* Skill)
{
	//~ @todo
	/*
	if (Skill)
	{
		if ((Skill->SupportedWeapons & (1 << (uint8)GetEquippedWeaponType())) &&
			GetCharacterStatsComponent()->GetCurrentMana() > Skill->ManaRequired &&
			GetCharacterStatsComponent()->GetCurrentStamina() > Skill->StaminaRequired)
		{
			return true;
		}
	}
	*/

	return false;
}

EWeaponType APlayerCharacter::GetEquippedWeaponType() const
{
	return PrimaryWeapon ? PrimaryWeapon->GetWeaponType() : EWeaponType::None;
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
			// PlayAnimationMontage(FlinchMontage,
				// UCharacterLibrary::SectionName_ForwardFlinch);
		}
		else
		{
			// PlayAnimationMontage(FlinchMontage,
				// UCharacterLibrary::SectionName_BackwardFlinch);
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
			// PlayAnimationMontage(HitMontage,
				// UCharacterLibrary::SectionName_ForwardInterrupt,
				// ECharacterState::GotHit);
		}
		else
		{
			// PlayAnimationMontage(HitMontage,
				// UCharacterLibrary::SectionName_BackwardInterrupt,
				// ECharacterState::GotHit);
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
	//~ @todo
	// CustomTimeDilation = GetCharacterStatsComponent()->GetActiveTimeDilation();
}

bool APlayerCharacter::CCEKnockdown_Implementation(const float Duration)
{
	if (CanKnockdown() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->HitEffects.Get())
	{
		// PlayAnimationMontage(GetActiveAnimationReferences()->HitEffects.Get(),
			// UCharacterLibrary::SectionName_KnockdownStart,
			// ECharacterState::GotHit);
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AEODCharacterBase::CCEEndKnockdown, Duration, false);

		return true;
	}

	return false;
}

void APlayerCharacter::CCEEndKnockdown_Implementation()
{
	// PlayAnimationMontage(GetActiveAnimationReferences()->HitEffects.Get(),
		// UCharacterLibrary::SectionName_KnockdownEnd,
		// ECharacterState::GotHit);
}

bool APlayerCharacter::CCEKnockback_Implementation(const float Duration, const FVector & ImpulseDirection)
{
	if (CanKnockdown() && GetActiveAnimationReferences() && GetActiveAnimationReferences()->HitEffects.Get())
	{
		// PlayAnimationMontage(GetActiveAnimationReferences()->HitEffects.Get(),
			// UCharacterLibrary::SectionName_KnockdownStart,
			// ECharacterState::GotHit);
		GetWorld()->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &AEODCharacterBase::CCEEndKnockdown, Duration, false);
		PushBack(ImpulseDirection);

		return true;
	}

	return false;
}

void APlayerCharacter::BlockAttack()
{
	if (GetActiveAnimationReferences() && GetActiveAnimationReferences()->BlockAttack.Get())
	{
		// PlayAnimationMontage(GetActiveAnimationReferences()->BlockAttack.Get(),
			// UCharacterLibrary::SectionName_BlockAttack);
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
	if (UWeaponLibrary::IsWeaponDualHanded(PrimaryWeapon->GetWeaponType()))
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

void APlayerCharacter::ZoomInCamera()
{
	if (GetCameraBoomComponent()->TargetArmLength >= CameraArmMinimumLength)
		GetCameraBoomComponent()->TargetArmLength -= CameraZoomRate;
}

void APlayerCharacter::ZoomOutCamera()
{
	if (GetCameraBoomComponent()->TargetArmLength <= CameraArmMaximumLength)
		GetCameraBoomComponent()->TargetArmLength += CameraZoomRate;
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

void APlayerCharacter::StartDodge()
{
	if (Controller && Controller->IsLocalPlayerController())
	{
		// DodgeIndex
		// 0 = Forward Dodge
		// 1 = Backward Dodge
		// 2 = Left Dodge
		// 3 = Right Dodge

		uint8 DodgeIndex = 0;
		if (ForwardAxisValue == 0)
		{
			if (RightAxisValue > 0)
			{
				DodgeIndex = 3;
			}
			else if (RightAxisValue < 0)
			{
				DodgeIndex = 2;
			}
			else
			{
				DodgeIndex = 1;
			}
		}
		else
		{
			if (ForwardAxisValue > 0)
			{
				DodgeIndex = 0;
			}
			else if (ForwardAxisValue < 0)
			{
				DodgeIndex = 1;
			}
		}

		float DesiredYaw = GetControllerRotationYaw();
		if (ForwardAxisValue != 0)
		{
			DesiredYaw = GetRotationYawFromAxisInput();
		}

		// Initiate dodge over network
		if (Role < ROLE_Authority)
		{
			Server_Dodge(DodgeIndex, DesiredYaw);
		}
		else
		{
			TriggeriFrames(DodgeImmunityDuration, DodgeImmunityTriggerDelay);			
		}

		FCharacterStateInfo NewStateInfo(ECharacterState::Dodging, DodgeIndex);
		NewStateInfo.NewReplicationIndex = CharacterStateInfo.NewReplicationIndex + 1;
		CharacterStateInfo = NewStateInfo;

		SetActorRotation(FRotator(0.f, DesiredYaw, 0.f));
		UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
		if (MoveComp)
		{
			MoveComp->bUseControllerDesiredRotation = false;
			MoveComp->SetDesiredCustomRotationYaw_LocalOnly(DesiredYaw);
		}
	}

	// Following variables are only relevant to owner
	bCharacterStateAllowsMovement = false;
	bCharacterStateAllowsRotation = false;

	FName SectionToPlay = NAME_None;
	if (CharacterStateInfo.SubStateIndex == 0)
	{
		SectionToPlay = UCharacterLibrary::SectionName_ForwardDodge;
	}
	else if (CharacterStateInfo.SubStateIndex == 1)
	{
		SectionToPlay = UCharacterLibrary::SectionName_BackwardDodge;
	}
	else if (CharacterStateInfo.SubStateIndex == 2)
	{
		SectionToPlay = UCharacterLibrary::SectionName_LeftDodge;
	}
	else if (CharacterStateInfo.SubStateIndex == 3)
	{
		SectionToPlay = UCharacterLibrary::SectionName_RightDodge;
	}

	FPlayerAnimationReferencesTableRow* AnimRef = GetActiveAnimationReferences();
	UAnimMontage* DodgeMontage = AnimRef ? AnimRef->Dodge.Get() : nullptr;
	check(DodgeMontage);

	float MontageDuration = PlayAnimMontage(DodgeMontage, 1.f, SectionToPlay);

	// The total duration of dodge montage is 4 times the dodge animation duration since it includes dodge animations for all 4 directions (left, right, forward, backward)
	float ActualDuration = (MontageDuration / 4) - DodgeMontage->GetDefaultBlendOutTime();

	UWorld* World = GetWorld();
	check(World);
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &APlayerCharacter::FinishDodge);
	World->GetTimerManager().SetTimer(FinishDodgeTimerHandle, TimerDelegate, ActualDuration, false);

}

void APlayerCharacter::CancelDodge()
{
	UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().ClearTimer(FinishDodgeTimerHandle);

	FPlayerAnimationReferencesTableRow* AnimRef = GetActiveAnimationReferences();
	UAnimMontage* DodgeMontage = AnimRef ? AnimRef->Dodge.Get() : nullptr;
	check(DodgeMontage);

	StopAnimMontage(DodgeMontage);
}

void APlayerCharacter::FinishDodge()
{
	ResetState();
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
		int DodgeStaminaCost = 30;
		//~ @todo
		// int32 DodgeCost = DodgeStaminaCost * GetCharacterStatsComponent()->GetStaminaConsumptionModifier();
		// int32 DodgeCost = DodgeStaminaCost;
		// GetCharacterStatsComponent()->ModifyCurrentStamina(-DodgeCost);

		// float ForwardAxisValue = InputComponent->GetAxisValue(FName("MoveForward"));
		// float RightAxisValue = InputComponent->GetAxisValue(FName("MoveRight"));
		float DesiredYaw = GetControllerRotationYaw();

		if (ForwardAxisValue != 0)
		{
			DesiredYaw = GetRotationYawFromAxisInput();
		}

		SetCharacterRotation(FRotator(0.f, DesiredYaw, 0.f));
		UAnimMontage* DodgeMontage = GetActiveAnimationReferences()->Dodge.Get();
		//~ @todo
		/*
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
		*/

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
	UWorld* World = GetWorld();
	if (World)
	{
		bBackwardPressed = false;
		bForwardPressed = true;
		World->GetTimerManager().SetTimer(SPAttackTimerHandle, this, &APlayerCharacter::DisableForwardPressed, 0.1f, false);
	}
}

void APlayerCharacter::OnPressedBackward()
{
	UWorld* World = GetWorld();
	if (World)
	{
		bForwardPressed = false;
		bBackwardPressed = true;
		World->GetTimerManager().SetTimer(SPAttackTimerHandle, this, &APlayerCharacter::DisableBackwardPressed, 0.1f, false);
	}
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

		/*
		if (IsAutoRunning())
		{
			DisableAutoRun();
		}
		*/

		Jump();
		// SetCharacterState(ECharacterState::Jumping);
	}
}

void APlayerCharacter::OnInteract()
{
	//~ @todo
	/*
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
	*/
}

void APlayerCharacter::ToggleSheathe()
{
	if (CanToggleSheathe())
	{
		bool bNewValue = !IsWeaponSheathed();
		SetWeaponSheathed(bNewValue);
		SetCharacterStateAllowsMovement(true);
		SetCharacterStateAllowsRotation(true);
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
		// SetCharacterState(ECharacterState::SwitchingWeapon);
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
	// SetNormalAttackKeyPressed(true);

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
	// SetNormalAttackKeyPressed(false);
}

void APlayerCharacter::DoNormalAttack()
{
	if (!GetActiveAnimationReferences() || !GetActiveAnimationReferences()->NormalAttacks.Get())
	{
		return;
	}

	//~ @todo
	/*
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
	*/
}

void APlayerCharacter::OnToggleAutoRun()
{
	//~ @todo
	/*
	if (GetCharacterState() == ECharacterState::AutoRun)
	{
		DisableAutoRun();
	}
	else if (CanAutoRun())
	{
		EnableAutoRun();
	}
	*/
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
		float Speed = (DefaultWalkSpeed * GetCharacterStatsComponent()->GetMovementSpeedModifier() * 5) / 16;
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			SetWalkSpeed(Speed);
		}
	}
	else
	{
		float Speed = DefaultWalkSpeed * GetCharacterStatsComponent()->GetMovementSpeedModifier();
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
		float Speed = (DefaultWalkSpeed * MovementSpeedModifier * 5) / 16;
		if (GetCharacterMovement()->MaxWalkSpeed != Speed)
		{
			SetWalkSpeed(Speed);
		}
	}
	else
	{
		float Speed = DefaultRunSpeed * MovementSpeedModifier;
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
	
	float Speed = DefaultWalkSpeed * MovementSpeedModifier;
	if (GetCharacterMovement()->MaxWalkSpeed != Speed)
	{
		SetWalkSpeed(Speed);
	}
}

void APlayerCharacter::StartNormalAttack()
{
	if (Controller && Controller->IsLocalPlayerController())
	{
		uint8 AttackIndex;
		if (bForwardPressed)
		{
			AttackIndex = 11;
		}
		else if (bBackwardPressed)
		{
			AttackIndex = 12;
		}
		else
		{
			AttackIndex = 1;
		}

		FCharacterStateInfo NewStateInfo(ECharacterState::Attacking, AttackIndex);
		NewStateInfo.NewReplicationIndex = CharacterStateInfo.NewReplicationIndex + 1;
		CharacterStateInfo = NewStateInfo;

		// float DesiredRotationYaw = GetControllerRotationYaw();

		if (Role < ROLE_Authority)
		{
			Server_NormalAttack(AttackIndex);
			// Server_NormalAttack(AttackIndex, DesiredRotationYaw);
		}

		UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
		if (MoveComp)
		{
			MoveComp->bUseControllerDesiredRotation = false;
			// MoveComp->SetDesiredCustomRotationYaw_LocalOnly(DesiredRotationYaw);
		}
	}

	bCharacterStateAllowsMovement = false;
	bCharacterStateAllowsRotation = false;

	// Determine what normal attack section should we start with
	FName SectionToPlay = NAME_None;
	if (CharacterStateInfo.SubStateIndex == 11)
	{
		SectionToPlay = UCharacterLibrary::SectionName_ForwardSPSwing;
	}
	else if (CharacterStateInfo.SubStateIndex == 12)
	{
		SectionToPlay = UCharacterLibrary::SectionName_BackwardSPSwing;
	}
	else if (CharacterStateInfo.SubStateIndex == 1)
	{
		SectionToPlay = UCharacterLibrary::SectionName_FirstSwing;
	}

	FPlayerAnimationReferencesTableRow* AnimRef = GetActiveAnimationReferences();
	UAnimMontage* AttackMontage = AnimRef ? AnimRef->NormalAttacks.Get() : nullptr;

	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage, 1.0, SectionToPlay);
	}



	/*
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
	*/
}

void APlayerCharacter::StopNormalAttack()
{
	// GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, GetActiveAnimationReferences()->NormalAttacks.Get());
}

void APlayerCharacter::CancelNormalAttack()
{
	FPlayerAnimationReferencesTableRow* AnimRef = GetActiveAnimationReferences();
	UAnimMontage* NormalAttackMontage = AnimRef ? AnimRef->NormalAttacks.Get() : nullptr;
	StopAnimMontage(NormalAttackMontage);
}

void APlayerCharacter::FinishNormalAttack()
{
	ResetState();
}

void APlayerCharacter::UpdateNormalAttackState(float DeltaTime)
{
	if (!bNormalAttackSectionChangeAllowed)
	{
		return;
	}

	// This update function must be called only on owner client
	if (!Controller || !Controller->IsLocalPlayerController())
	{
		return;
	}

	FPlayerAnimationReferencesTableRow* AnimRef = GetActiveAnimationReferences();
	UAnimMontage* NormalAttackMontage = AnimRef ? AnimRef->NormalAttacks.Get() : nullptr;
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;

	if (!NormalAttackMontage || !AnimInstance)
	{
		return;
	}

	FName CurrentSection = AnimInstance->Montage_GetCurrentSection(NormalAttackMontage);
	FName ExpectedNextSection = GetNextNormalAttackSectionName(CurrentSection);
	if (ExpectedNextSection != NAME_None)
	{
		uint8 AttackIndex = GetNormalAttackIndex(ExpectedNextSection);
		FCharacterStateInfo StateInfo(ECharacterState::Attacking, AttackIndex);
		StateInfo.NewReplicationIndex = CharacterStateInfo.NewReplicationIndex + 1;
		CharacterStateInfo = StateInfo;

		if (Role < ROLE_Authority)
		{
			Server_NormalAttack(AttackIndex);
		}

		ChangeNormalAttackSection(CurrentSection, ExpectedNextSection);
	}

	bNormalAttackSectionChangeAllowed = false;

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
			PlayNormalAttackAnimation(CurrentSection, ExpectedNextSection);
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

void APlayerCharacter::SaveCharacterState()
{
	if (IsValid(GetGameplaySkillsComponent()))
	{
		GetGameplaySkillsComponent()->SaveSkillBarLayout();
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
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (OldSection == NAME_None)
		{
			AnimInstance->Montage_Play(NormalAttackMontage);
			AnimInstance->Montage_JumpToSection(NewSection, NormalAttackMontage);
		}
		else
		{
			FString OldSectionString = OldSection.ToString();
			if (OldSectionString.EndsWith("End"))
			{
				AnimInstance->Montage_Play(NormalAttackMontage);
				AnimInstance->Montage_JumpToSection(NewSection, NormalAttackMontage);
			}
			else
			{
				FName CurrentSection = AnimInstance->Montage_GetCurrentSection(NormalAttackMontage);
				if (CurrentSection == OldSection)
				{
					AnimInstance->Montage_SetNextSection(CurrentSection, NewSection, NormalAttackMontage);
				}
				else
				{
					AnimInstance->Montage_Play(NormalAttackMontage);
					AnimInstance->Montage_JumpToSection(NewSection, NormalAttackMontage);
				}
			}
		}

		// SetCharacterState(ECharacterState::Attacking);
	}

	Server_PlayNormalAttackAnimation(OldSection, NewSection);
}

void APlayerCharacter::ChangeNormalAttackSection(FName OldSection, FName NewSection)
{
	FPlayerAnimationReferencesTableRow* AnimRef = GetActiveAnimationReferences();
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	UAnimMontage* NormalAttackMontage = AnimRef ? AnimRef->NormalAttacks.Get() : nullptr;

	if (!NormalAttackMontage || !AnimInstance)
	{
		return;
	}

	if (OldSection == NAME_None)
	{
		AnimInstance->Montage_Play(NormalAttackMontage);
		AnimInstance->Montage_JumpToSection(NewSection, NormalAttackMontage);
	}
	else
	{
		FString OldSectionString = OldSection.ToString();
		if (OldSectionString.EndsWith("End"))
		{
			AnimInstance->Montage_Play(NormalAttackMontage);
			AnimInstance->Montage_JumpToSection(NewSection, NormalAttackMontage);
		}
		else
		{
			AnimInstance->Montage_SetNextSection(OldSection, NewSection, NormalAttackMontage);
		}
	}


	/*
	if (!GetActiveAnimationReferences() || !GetActiveAnimationReferences()->NormalAttacks.Get())
	{
		return;
	}

	UAnimMontage* NormalAttackMontage = GetActiveAnimationReferences()->NormalAttacks.Get();
	if (GetMesh()->GetAnimInstance())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (OldSection == NAME_None)
		{
			AnimInstance->Montage_Play(NormalAttackMontage);
			AnimInstance->Montage_JumpToSection(NewSection, NormalAttackMontage);
		}
		else
		{
			FString OldSectionString = OldSection.ToString();
			if (OldSectionString.EndsWith("End"))
			{
				AnimInstance->Montage_Play(NormalAttackMontage);
				AnimInstance->Montage_JumpToSection(NewSection, NormalAttackMontage);
			}
			else
			{
				FName CurrentSection = AnimInstance->Montage_GetCurrentSection(NormalAttackMontage);
				if (CurrentSection == OldSection)
				{
					AnimInstance->Montage_SetNextSection(CurrentSection, NewSection, NormalAttackMontage);
				}
				else
				{
					AnimInstance->Montage_Play(NormalAttackMontage);
					AnimInstance->Montage_JumpToSection(NewSection, NormalAttackMontage);
				}
			}
		}

		SetCharacterState(ECharacterState::Attacking);
	}

	Server_PlayNormalAttackAnimation(OldSection, NewSection);
	*/
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

	//~ @todo
	// GetCharacterStatsComponent()->ModifyCurrentMana(-SkillPair.Value->ManaRequired);
	// GetCharacterStatsComponent()->ModifyCurrentStamina(-SkillPair.Value->StaminaRequired);

	SetCurrentActiveSkillID(SkillPair.Key);
	SetCurrentActiveSkill(SkillPair.Value);
	GetSkillsComponent()->OnSkillUsed(SkillButtonIndex, SkillPair.Key, SkillPair.Value);

	// bSkillAllowsMovement = SkillPair.Value->bAllowsMovement;
	bSkillHasDirectionalAnimations = SkillPair.Value->bHasDirectionalAnimations;

	// @todo cleaning of crowd control effect timer needs to be handled better
	GetWorld()->GetTimerManager().ClearTimer(CrowdControlTimerHandle);
	// If frozen
	if (CustomTimeDilation == 0.f)
	{
		//~ @todo
		// EndFreeze();
	}

	//~ @todo
	// GetCharacterStatsComponent()->AddCrowdControlImmunitiesFromSkill(SkillPair.Value->CrowdControlImmunities);

	//~ @todo
	/*
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
	*/
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
	// SetCharacterState(ECharacterState::Interacting);
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
		// SetCharacterState(ECharacterState::IdleWalkRun);

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
	if (IsNormalAttacking())
	{
		// If the montage blending out wasn't interrupted
		if (!bInterrupted)
		{
			FPlayerAnimationReferencesTableRow* AnimRef = GetActiveAnimationReferences();
			UAnimMontage* NormalAttackMontage = AnimRef ? AnimRef->NormalAttacks.Get() : nullptr;
			if (NormalAttackMontage == AnimMontage)
			{
				FinishNormalAttack();
			}
		}
	}


	if (GetController() && GetController()->IsLocalPlayerController())
	{
		// @todo
	}

	//~ @todo
	/*
	if (!bInterrupted  && GetCharacterState() != ECharacterState::Interacting)
	{
		SetCharacterState(ECharacterState::IdleWalkRun);
	}
	*/

	// @todo handle skill montage blending out better because montages might be blending out in between transition of standstill and moving animations
	if (GetCurrentActiveSkill() && GetCurrentActiveSkill()->AnimMontage.Get() == AnimMontage)
	{
		GetSkillsComponent()->SetOffChainSkillReset();


		//~ @todo
		/*
		if (GetCurrentActiveSkill()->CrowdControlImmunities == GetCharacterStatsComponent()->GetCrowdControlImmunitiesFromSkill())
		{
			GetCharacterStatsComponent()->RemoveCrowdControlImmunitiesFromSkil();
		}
		*/
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
	if (UWeaponLibrary::IsWeaponDualHanded(PrimaryWeapon->GetWeaponType()))
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

void APlayerCharacter::OnRep_CharacterStateInfo(const FCharacterStateInfo& OldStateInfo)
{
	Super::OnRep_CharacterStateInfo(OldStateInfo);

	if (CharacterStateInfo.CharacterState == ECharacterState::Attacking)
	{
		if (CharacterStateInfo.SubStateIndex == 1 ||
			CharacterStateInfo.SubStateIndex == 11 ||
			CharacterStateInfo.SubStateIndex == 12)
		{
		}
		else
		{
			FPlayerAnimationReferencesTableRow* AnimRef = GetActiveAnimationReferences();
			UAnimMontage* NormalAttackMontage = AnimRef ? AnimRef->NormalAttacks.Get() : nullptr;
			UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;

			if (NormalAttackMontage && AnimInstance)
			{
				FName OldSection = AnimInstance->Montage_GetCurrentSection(NormalAttackMontage);
				FName NewSection = GetNormalAttackSectionName(CharacterStateInfo.SubStateIndex);
				ChangeNormalAttackSection(OldSection, NewSection);
			}
		}
	}
}

void APlayerCharacter::Server_Dodge_Implementation(uint8 DodgeIndex, float RotationYaw)
{
	FCharacterStateInfo StateInfo(ECharacterState::Dodging, DodgeIndex);
	StateInfo.NewReplicationIndex = CharacterStateInfo.NewReplicationIndex + 1;
	CharacterStateInfo = StateInfo;
	SetActorRotation(FRotator(0.f, RotationYaw, 0.f));
	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
	if (MoveComp)
	{
		MoveComp->bUseControllerDesiredRotation = false;
		MoveComp->SetDesiredCustomRotationYaw(RotationYaw);
	}

	TriggeriFrames(DodgeImmunityDuration, DodgeImmunityTriggerDelay);
	StartDodge();
}

bool APlayerCharacter::Server_Dodge_Validate(uint8 DodgeIndex, float RotationYaw)
{
	return true;
}

void APlayerCharacter::Multicast_Dodge_Implementation(uint8 DodgeIndex, float RotationYaw)
{
}

void APlayerCharacter::Server_NormalAttack_Implementation(uint8 AttackIndex)
{
	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
	if (MoveComp)
	{
		MoveComp->bUseControllerDesiredRotation = false;
	}

	FCharacterStateInfo NewStateInfo(ECharacterState::Attacking, AttackIndex);
	NewStateInfo.NewReplicationIndex = CharacterStateInfo.NewReplicationIndex + 1;
	CharacterStateInfo = NewStateInfo;

	if (AttackIndex == 1 || AttackIndex == 11 || AttackIndex == 12)
	{
		StartNormalAttack();
	}
	else
	{
		FPlayerAnimationReferencesTableRow* AnimRef = GetActiveAnimationReferences();
		UAnimMontage* NormalAttackMontage = AnimRef ? AnimRef->NormalAttacks.Get() : nullptr;
		UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;

		if (NormalAttackMontage && AnimInstance)
		{
			FName OldSection = AnimInstance->Montage_GetCurrentSection(NormalAttackMontage);
			FName NewSection = GetNormalAttackSectionName(AttackIndex);
			ChangeNormalAttackSection(OldSection, NewSection);
		}
	}
}

bool APlayerCharacter::Server_NormalAttack_Validate(uint8 AttackIndex)
{
	return true;
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
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (OldSection == NAME_None)
		{
			AnimInstance->Montage_Play(NormalAttackMontage);
			AnimInstance->Montage_JumpToSection(NewSection, NormalAttackMontage);
		}
		else
		{
			FString OldSectionString = OldSection.ToString();
			if (OldSectionString.EndsWith("End"))
			{
				AnimInstance->Montage_Play(NormalAttackMontage);
				AnimInstance->Montage_JumpToSection(NewSection, NormalAttackMontage);
			}
			else
			{
				FName CurrentSection = AnimInstance->Montage_GetCurrentSection(NormalAttackMontage);
				if (CurrentSection == OldSection)
				{
					AnimInstance->Montage_SetNextSection(CurrentSection, NewSection, NormalAttackMontage);
				}
				else
				{
					AnimInstance->Montage_Play(NormalAttackMontage);
					AnimInstance->Montage_JumpToSection(NewSection, NormalAttackMontage);
				}
			}
		}

		// SetCharacterState(ECharacterState::Attacking);
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
