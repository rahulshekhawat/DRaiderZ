// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerCharacter.h"
#include "PlayerAnimInstance.h"
#include "GameSingleton.h"
#include "WeaponLibrary.h"
#include "InventoryComponent.h"
#include "PlayerStatsComponent.h"
#include "EODSaveGame.h"
#include "NPCBase.h"
#include "EODPlayerController.h"
#include "DialogueWindowWidget.h"
#include "DialogueLibrary.h"
#include "GameplaySkillsComponent.h"
#include "EODCharacterMovementComponent.h"
#include "PlayerSkillsComponent.h"

#include "TimerManager.h"
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

const FName APlayerCharacter::SystemAudioComponentName(TEXT("System Audio Component"));
const FName APlayerCharacter::InteractionSphereComponentName(TEXT("Interaction Sphere"));

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerSkillsComponent>(AEODCharacterBase::GameplaySkillsComponentName))
{
	// The player should be free to look around with mouse without actually rotating the posessed character
	bUseControllerRotationYaw = false;

	SystemAudioComponent = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, APlayerCharacter::SystemAudioComponentName);
	if (SystemAudioComponent)
	{
		SystemAudioComponent->SetupAttachment(RootComponent);
	}

	InteractionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, APlayerCharacter::InteractionSphereComponentName);
	if (InteractionSphere)
	{
		InteractionSphere->SetupAttachment(RootComponent);
		InteractionSphere->SetSphereRadius(150);
		InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	MaxWeaponSlots = 2;
	
	/*
	OnPrimaryWeaponEquipped.AddDynamic(this, &APlayerCharacter::ActivateStatusEffectFromWeapon);
	OnPrimaryWeaponUnequipped.AddDynamic(this, &APlayerCharacter::DeactivateStatusEffectFromWeapon);

	OnSecondaryWeaponEquipped.AddDynamic(this, &APlayerCharacter::ActivateStatusEffectFromWeapon);
	OnSecondaryWeaponUnequipped.AddDynamic(this, &APlayerCharacter::DeactivateStatusEffectFromWeapon);
	*/

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnInteractionSphereBeginOverlap);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnInteractionSphereEndOverlap);

}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, PrimaryWeaponID);
	DOREPLIFETIME(APlayerCharacter, SecondaryWeaponID);
	// DOREPLIFETIME(APlayerCharacter, LastAttackResponses);

}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (GetGameplaySkillsComponent())
	{
		GetGameplaySkillsComponent()->InitializeSkills(this);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


}

bool APlayerCharacter::CanMove() const
{
	// @todo, uncomment and replace return code with following comment:
	return IsIdleOrMoving() || IsBlocking() || IsSwitchingWeapon() || bCharacterStateAllowsMovement;
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
	bool bStateAllowsDodge = IsIdleOrMoving() || IsBlocking() || IsCastingSpell() || IsNormalAttacking();

	// If we have a valid animation for dodge and the character state allows dodging
	return Animation && bStateAllowsDodge;
}

bool APlayerCharacter::CanGuardAgainstAttacks() const
{
	return (IsIdleOrMoving() || IsNormalAttacking()) && !(IsWeaponSheathed() || GetEquippedWeaponType() == EWeaponType::None);
}

bool APlayerCharacter::CanUseAnySkill() const
{
	return (GetEquippedWeaponType() != EWeaponType::None) && !IsWeaponSheathed() && (IsIdleOrMoving() || IsBlocking() || IsNormalAttacking());
}

void APlayerCharacter::PlaySystemSound(USoundBase* SoundToPlay)
{
	check(SystemAudioComponent);
	SystemAudioComponent->SetSound(SoundToPlay);
	SystemAudioComponent->Play();
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
	if (!CanInterrupt())
	{
		return false;
	}

	FPlayerAnimationReferencesTableRow* AnimRef = GetActiveAnimationReferences();
	UAnimMontage* AnimMontage = AnimRef ? AnimRef->Interrupt.Get() : nullptr;
	if (AnimMontage)
	{
		if (BCAngle <= 90)
		{
			PlayAnimMontage(AnimMontage, 1.f, UCharacterLibrary::SectionName_ForwardInterrupt);
		}
		else
		{
			PlayAnimMontage(AnimMontage, 1.f, UCharacterLibrary::SectionName_BackwardInterrupt);
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
	if (!CanKnockdown())
	{
		return false;
	}

	FPlayerAnimationReferencesTableRow* AnimRef = GetActiveAnimationReferences();
	UAnimMontage* AnimMontage = AnimRef ? AnimRef->Knockdown.Get() : nullptr;
	UWorld* World = GetWorld();

	if (AnimMontage && World)
	{
		PlayAnimMontage(AnimMontage, 1.f, UCharacterLibrary::SectionName_KnockdownStart);
		World->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &APlayerCharacter::CCEEndKnockdown, Duration, false);
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

bool APlayerCharacter::CCEKnockback_Implementation(const float Duration, const FVector& ImpulseDirection)
{
	if (!CanKnockdown())
	{
		return false;
	}

	FPlayerAnimationReferencesTableRow* AnimRef = GetActiveAnimationReferences();
	UAnimMontage* AnimMontage = AnimRef ? AnimRef->Knockdown.Get() : nullptr;
	UWorld* World = GetWorld();

	if (AnimMontage && World)
	{
		PlayAnimMontage(AnimMontage, 1.f, UCharacterLibrary::SectionName_KnockdownStart);
		World->GetTimerManager().SetTimer(CrowdControlTimerHandle, this, &APlayerCharacter::CCEEndKnockdown, Duration, false);
		return true;
	}

	return false;
}

void APlayerCharacter::PlayAttackBlockedAnimation()
{
	if (GetActiveAnimationReferences() && GetActiveAnimationReferences()->BlockAttack.Get())
	{
		// PlayAnimationMontage(GetActiveAnimationReferences()->BlockAttack.Get(),
			// UCharacterLibrary::SectionName_BlockAttack);
	}
}

/*
void APlayerCharacter::ActivateStatusEffectFromWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset)
{
}

void APlayerCharacter::DeactivateStatusEffectFromWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset)
{
}
*/

void APlayerCharacter::AddPrimaryWeapon(FName WeaponID)
{

	/*
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
	*/

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

	// UWeaponDataAsset* WeaponDataAsset = UWeaponLibrary::GetWeaponDataAsset(WeaponID);
	// if (WeaponDataAsset)
	{
		//~ @todo
		// Server_AddPrimaryWeaponToCurrentSlot(WeaponID, WeaponDataAsset);
	}
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

void APlayerCharacter::SwitchToInteractionState()
{
	CharacterStateInfo.CharacterState = ECharacterState::Interacting;
	CharacterStateInfo.SubStateIndex = 0;
}

void APlayerCharacter::ToggleSheathe()
{
	if (CanToggleSheathe())
	{
		bool bNewValue = !IsWeaponSheathed();
		SetWeaponSheathed(bNewValue);
		UpdatePCTryingToMove();
		StartWeaponSwitch();
		bCharacterStateAllowsMovement = true;
		bCharacterStateAllowsRotation = true;

		UPlayerSkillsComponent* SkillComp = Cast<UPlayerSkillsComponent>(GetGameplaySkillsComponent());
		check(SkillComp);
		EWeaponType NewWeaponType = IsWeaponSheathed() ? EWeaponType::None : GetEquippedWeaponType();
		SkillComp->OnPlayerWeaponChanged();
	}
}

void APlayerCharacter::StartWeaponSwitch()
{
	FPlayerAnimationReferencesTableRow* EquippedAnimRef = GetEquippedWeaponAnimationReferences();
	if (!EquippedAnimRef)
	{
		return;
	}

	UAnimMontage* MontageToPlay = IsPCTryingToMove() ? EquippedAnimRef->WeaponSwitchUpperBody.Get() : EquippedAnimRef->WeaponSwitchFullBody.Get();
	FName SectionToPlay = IsWeaponSheathed() ? UCharacterLibrary::SectionName_SheatheWeapon : UCharacterLibrary::SectionName_UnsheatheWeapon;

	UPlayerAnimInstance* PlayerAnimInstance = GetMesh() ? Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance()) : nullptr;

	UWorld* World = GetWorld();
	if (PlayerAnimInstance && World)
	{
		float MontageLength = PlayerAnimInstance->Montage_Play(MontageToPlay);
		PlayerAnimInstance->Montage_JumpToSection(SectionToPlay);
		PlayerAnimInstance->OnTransitionableMontageTriggered(IsPCTryingToMove());

		float ActualLength = MontageLength / 2.f;
		World->GetTimerManager().SetTimer(FinishWeaponSwitchTimerHandle, this, &APlayerCharacter::FinishWeaponSwitch, ActualLength, false);
		
		CharacterStateInfo.CharacterState = ECharacterState::SwitchingWeapon;
	}
}

void APlayerCharacter::CancelWeaponSwitch()
{
}

void APlayerCharacter::FinishWeaponSwitch()
{
	ResetState();
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
}

void APlayerCharacter::SaveCharacterState()
{
	if (IsValid(GetGameplaySkillsComponent()))
	{
		// GetGameplaySkillsComponent()->SaveSkillBarLayout();
	}
}

void APlayerCharacter::PostAttack(const TArray<FAttackResponse>& AttackResponses, const TArray<AActor*> HitActors)
{
	LastAttackResponses = AttackResponses;
	/*
	if (AttackResponses.Num() == 0 && GameplayAudioComponent)
	{
		GameplayAudioComponent->SetSound(GetMeleeHitMissSound());
		GameplayAudioComponent->Play();
	}
	*/
}

TSharedPtr<FAttackResponse> APlayerCharacter::ReceiveAttack(
	AActor* HitInstigator,
	ICombatInterface* InstigatorCI,
	const TSharedPtr<FAttackInfo>& AttackInfoPtr,
	const FHitResult& DirectHitResult,
	const bool bLineHitResultFound,
	const FHitResult& LineHitResult)
{
	return TSharedPtr<FAttackResponse>();
}

float APlayerCharacter::GetActualDamage(
	AActor* HitInstigator,
	ICombatInterface* InstigatorCI,
	const TSharedPtr<FAttackInfo>& AttackInfoPtr,
	const bool bCritHit,
	const bool bAttackBlocked)
{
	return 0.0f;
}

USoundBase* APlayerCharacter::GetMeleeHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const
{
	// Crit
	// Flesh
	// Metal
	// Stone
	// Undead
	// Wood

	USoundBase* Sound = nullptr;
	EWeaponType WeaponType = GetEquippedWeaponType();
	switch (WeaponType)
	{
	case EWeaponType::GreatSword:
		Sound = GetGreatswordHitSound(HitSurface, bCritHit);
		break;
	case EWeaponType::WarHammer:
		Sound = GetWarhammerHitSound(HitSurface, bCritHit);
		break;
	case EWeaponType::LongSword:
		Sound = GetLongswordHitSound(HitSurface, bCritHit);
		break;
	case EWeaponType::Mace:
		Sound = GetMaceHitSound(HitSurface, bCritHit);
		break;
	case EWeaponType::Dagger:
		Sound = GetDaggerHitSound(HitSurface, bCritHit);
		break;
	case EWeaponType::Staff:
		Sound = GetStaffHitSound(HitSurface, bCritHit);
		break;
	default:
		break;
	}
	return Sound;
}

USoundBase* APlayerCharacter::GetMeleeHitMissSound() const
{
	USoundBase* Sound = nullptr;
	EWeaponType WeaponType = GetEquippedWeaponType();

	switch (WeaponType)
	{
	case EWeaponType::GreatSword:
		// Slash2
		Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Slash2HitMissSounds);
		break;
	case EWeaponType::WarHammer:
		// blunt3
		Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt3HitMissSounds);
		break;
	case EWeaponType::LongSword:
		// Slash1
		Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SlashHitMissSounds);
		break;
	case EWeaponType::Mace:
		// blunt1
		Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.BluntHitMissSounds);
		break;
	case EWeaponType::Dagger:
		// slice
		Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SliceHitMissSounds);
		break;
	case EWeaponType::Staff:
		// blunt2
		Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt2HitMissSounds);
		break;
	default:
		break;
	}

	return Sound;
}

void APlayerCharacter::DisplayDialogueWidget(FName DialogueWindowID)
{
	//~ @todo
	/*
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
	*/
}

void APlayerCharacter::RemoveDialogueWidget()
{
	if (DialogueWidget)
	{
		DialogueWidget->RemoveFromParent();
		DialogueWidget->MarkPendingKill(); // Would this crash?
		DialogueWidget = nullptr;
	}

	/*
	if (HUDWidget)
	{
		HUDWidget->GetSkillBarWidget()->SetVisibility(ESlateVisibility::Visible);
	}
	*/
}

void APlayerCharacter::CreateGhostTrail_Implementation()
{
}

void APlayerCharacter::OnPressingSkillKey(const uint32 SkillButtonIndex)
{
}

void APlayerCharacter::OnReleasingSkillKey(const uint32 SkillButtonIndex)
{
}

void APlayerCharacter::SetCanUseChainSkill(bool bNewValue)
{
	if (GetGameplaySkillsComponent())
	{
		GetGameplaySkillsComponent()->SetCanUseChainSkill(bNewValue);
	}
}

void APlayerCharacter::OnInteractionSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
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

	GameplayAudioComponent->SetSound(InteractiveActorDetectedSound);
	GameplayAudioComponent->Play();
}

void APlayerCharacter::OnInteractionSphereEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
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

			GameplayAudioComponent->SetSound(InteractiveActorDetectedSound);
			GameplayAudioComponent->Play();
		}
		else
		{
			ActiveInteractiveActor = nullptr;
		}
	}
}

bool APlayerCharacter::CanStartInteraction() const
{
	return IsIdleOrMoving();
}

void APlayerCharacter::TriggerInteraction()
{
	if (IsInteracting())
	{
		UpdateInteraction();
	}
	else if (CanStartInteraction())
	{
		StartInteraction();
	}
}

void APlayerCharacter::StartInteraction()
{
	IInteractionInterface* InteractionObj = Cast<IInteractionInterface>(ActiveInteractiveActor);
	if (InteractionObj)
	{
		InteractionObj->Execute_OnInteract(ActiveInteractiveActor, this);
	}




	/*
	if (ActiveInteractiveActor)
	{
		IInteractionInterface* InteractiveObj = Cast<IInteractionInterface>(ActiveInteractiveActor);
		if (InteractiveObj)
		{
			InteractiveObj->Execute_OnInteract(ActiveInteractiveActor, this);
		}
	}

	AEODPlayerController* PC = Cast<AEODPlayerController>(GetController());
	if (PC && ActiveInteractiveActor)
	{
		IInteractionInterface* InteractiveObj = Cast<IInteractionInterface>(ActiveInteractiveActor);
		// Just in case, make sure 'ActiveInteractiveActor' implements 'IInteractionInterface'
		if (InteractiveObj)
		{
			PC->SetViewTargetWithBlend(ActiveInteractiveActor, 0.5, EViewTargetBlendFunction::VTBlend_Linear, 0.f, true);

			FCharacterStateInfo NewStateInfo;
			NewStateInfo.CharacterState = ECharacterState::Interacting;
			NewStateInfo.NewReplicationIndex = CharacterStateInfo.NewReplicationIndex + 1;
			CharacterStateInfo = NewStateInfo;
			SetCharacterStateAllowsMovement_Local(false);
			SetCharacterStateAllowsRotation(false);

			// DisplayDialogueWidget();

			if (GameplayAudioComponent)
			{
				GameplayAudioComponent->SetSound(DialogueTriggeredSound);
				GameplayAudioComponent->Play();
			}

			InteractiveObj->Execute_OnInteract(ActiveInteractiveActor, this);
		}
	}
	else
	{
		CancelInteraction();
	}
	*/
}

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

			GameplayAudioComponent->SetSound(DialogueTriggeredSound);
			GameplayAudioComponent->Play();
		}
	}
}

void APlayerCharacter::CancelInteraction()
{
	AEODPlayerController* PC = Cast<AEODPlayerController>(GetController());
	if (PC)
	{
		PC->SetViewTargetWithBlend(this, 0.5, EViewTargetBlendFunction::VTBlend_Linear, 0.f, true);
		ResetState();

		RemoveDialogueWidget();

		GameplayAudioComponent->SetSound(DialogueEndSound);
		GameplayAudioComponent->Play();
	}
}

void APlayerCharacter::FinishInteraction()
{
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

		GameplayAudioComponent->SetSound(DialogueEndSound);
		// AudioComponent->SetSound(InteractionEndSound);
		GameplayAudioComponent->Play();
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
	//~ @todo
	// if (GetCurrentActiveSkill() && GetCurrentActiveSkill()->AnimMontage.Get() == AnimMontage)
	{
		//~ @todo
		// GetSkillsComponent()->SetOffChainSkillReset();


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
		// GetGameplaySkillsComponent()->SetCurrentActiveSkill(NAME_None);
	}
}

void APlayerCharacter::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
}

void APlayerCharacter::EquipPrimaryWeapon(FName WeaponID)
{
	SetPrimaryWeaponID(WeaponID);
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
	// GetSkillsComponent()->OnSkillGroupAddedToSkillBar(SkillGroup);
}

void APlayerCharacter::OnSkillGroupRemovedFromSkillBar(const FString & SkillGroup)
{
	// GetSkillsComponent()->OnSkillGroupRemovedFromSkillBar(SkillGroup);
}

void APlayerCharacter::OnRep_LastAttackResponses(const TArray<FAttackResponse>& OldAttackResponses)
{

}

void APlayerCharacter::OnRep_PrimaryWeaponID()
{
	SetCurrentPrimaryWeapon(PrimaryWeaponID);
}

void APlayerCharacter::OnRep_SecondaryWeaponID()
{
}

void APlayerCharacter::OnRep_LastReceivedHit(const FReceivedHitInfo& OldHitInfo)
{
}

void APlayerCharacter::OnRep_CharacterStateInfo(const FCharacterStateInfo& OldStateInfo)
{
	// If the new character state is IdleWalkRun but the old character state wasn't idle walk run
	if (CharacterStateInfo.CharacterState == ECharacterState::IdleWalkRun && OldStateInfo.CharacterState != ECharacterState::IdleWalkRun)
	{
		ResetState();
	}
	else if (CharacterStateInfo.CharacterState == ECharacterState::Dodging)
	{
		StartDodge();
	}
	else if (CharacterStateInfo.CharacterState == ECharacterState::Blocking)
	{
		if (OldStateInfo.CharacterState == ECharacterState::Attacking)
		{
			CancelNormalAttack();
		}

		StartBlockingAttacks();
	}
	else if (CharacterStateInfo.CharacterState == ECharacterState::Attacking)
	{
		if (CharacterStateInfo.SubStateIndex == 1 ||
			CharacterStateInfo.SubStateIndex == 11 ||
			CharacterStateInfo.SubStateIndex == 12)
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
				FName NewSection = GetNormalAttackSectionName(CharacterStateInfo.SubStateIndex);
				ChangeNormalAttackSection(OldSection, NewSection);
			}
		}
	}
	else if (CharacterStateInfo.CharacterState == ECharacterState::UsingActiveSkill)
	{
		UGameplaySkillsComponent* SkillsComp = GetGameplaySkillsComponent();
		if (SkillsComp)
		{
			if (CharacterStateInfo.SubStateIndex > 100)
			{
				SkillsComp->ReleaseSkill(CharacterStateInfo.SubStateIndex);
			}
			else
			{
				SkillsComp->TriggerSkill(CharacterStateInfo.SubStateIndex);
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

void APlayerCharacter::Server_SetPrimaryWeaponID_Implementation(FName NewWeaponID)
{
	SetPrimaryWeaponID(NewWeaponID);
}

bool APlayerCharacter::Server_SetPrimaryWeaponID_Validate(FName NewWeaponID)
{
	return true;
}
