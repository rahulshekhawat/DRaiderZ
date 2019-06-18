// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODCharacterBase.h"
#include "RideBase.h"
#include "CharAnimInstance.h"
#include "InteractionInterface.h"
#include "EODBlueprintFunctionLibrary.h"
#include "EODPlayerController.h"
#include "EODAIControllerBase.h"
#include "StatsComponentBase.h"
#include "GameplaySkillsComponent.h"
#include "EODCharacterMovementComponent.h"
#include "StatsComponentBase.h"
#include "PlayerStatsComponent.h"
#include "ActiveSkillBase.h"
#include "AIStatsComponent.h"
#include "PlayerStatsComponent.h"
#include "AttackDodgedEvent.h"
#include "EODGameInstance.h"
#include "DynamicHUDWidget.h"
#include "AISkillBase.h"
#include "EODWidgetComponent.h"
#include "DamageNumberWidget.h"

#include "IdleWalkRunState.h"
#include "DeadState.h"
#include "DodgeState.h"
#include "GuardState.h"
#include "HitInCombatState.h"
#include "NormalAttackState.h"
#include "UsingSkillState.h"

#include "UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"

/**
 * EOD Character stats
 */
DECLARE_CYCLE_STAT(TEXT("EOD ChararaterTick"), STAT_EODCharacterTick, STATGROUP_EOD);

const FName AEODCharacterBase::CameraComponentName(TEXT("Camera"));
const FName AEODCharacterBase::SpringArmComponentName(TEXT("Camera Boom"));
const FName AEODCharacterBase::GameplaySkillsComponentName(TEXT("Skill Manager"));
const FName AEODCharacterBase::GameplayAudioComponentName(TEXT("Gameplay Audio Component"));

AEODCharacterBase::AEODCharacterBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UEODCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	SkillManager = ObjectInitializer.CreateDefaultSubobject<UGameplaySkillsComponent>(this, AEODCharacterBase::GameplaySkillsComponentName);
	CameraBoomComponent = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, AEODCharacterBase::SpringArmComponentName);
	if (CameraBoomComponent)
	{
		CameraBoomComponent->bUsePawnControlRotation = true;
		CameraBoomComponent->SetupAttachment(RootComponent);
		CameraBoomComponent->AddLocalOffset(FVector(0.f, 0.f, 60.f));
	}

	CameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, AEODCharacterBase::CameraComponentName);
	if (CameraComponent)
	{
		CameraComponent->SetupAttachment(CameraBoomComponent, USpringArmComponent::SocketName);
	}

	GameplayAudioComponent = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, AEODCharacterBase::GameplayAudioComponentName);
	if (GameplayAudioComponent)
	{
		GameplayAudioComponent->SetupAttachment(RootComponent);
	}

	if (GetMesh())
	{
		GetMesh()->SetReceivesDecals(false);
	}

	SetReplicates(true);
	SetReplicateMovement(true);
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetIsReplicated(true);
	}

	CameraZoomRate = 15;
	CameraArmMinimumLength = 50;
	CameraArmMaximumLength = 500;

	DefaultWalkSpeed = 400.f;
	DefaultRunSpeed = 600.f;
	DefaultWalkSpeedWhileBlocking = 150.f;

	// CharacterState = ECharacterState::IdleWalkRun;
	bGodMode = false;
	TargetSwitchDuration = 0.1f;

	// By default the weapon should be sheathed
	bWeaponSheathed = true;

	DodgeImmunityTriggerDelay = 0.1f;
	DodgeImmunityDuration = 0.4;
	DamageBlockTriggerDelay = 0.2f;

	Faction = EFaction::Player;

	CharacterStateInfo = FCharacterStateInfo();
	bCharacterStateAllowsMovement = true;
	bCharacterStateAllowsRotation = true;

	MovementSpeedModifier = 1.f;

}

void AEODCharacterBase::Tick(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_EODCharacterTick);

	Super::Tick(DeltaTime);

	ResetTickDependentData();

	if (Controller && Controller->IsLocalPlayerController())
	{
		bool bCanGuardAgainstAttacks = CanGuardAgainstAttacks();
		// If character wants to guard but it's guard is not yet active
		if (bWantsToGuard && !IsBlocking() && bCanGuardAgainstAttacks)
		{
			StartBlockingAttacks();
		}
		// If the character guard is active but it doesn't want to guard anymore
		else if (!bWantsToGuard && IsBlocking())
		{
			StopBlockingAttacks();
		}

		bool bCanNormalAttack = CanNormalAttack();
		if (bWantsToNormalAttack && !IsNormalAttacking() && bCanNormalAttack)
		{
			StartNormalAttack();
		}
		else if (bWantsToNormalAttack && IsNormalAttacking())
		{
			UpdateNormalAttackState(DeltaTime);
		}

		UpdateMovement(DeltaTime);
		UpdateRotation(DeltaTime);
	}
}

void AEODCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEODCharacterBase, CurrentRide);
	DOREPLIFETIME(AEODCharacterBase, MovementSpeedModifier);
	DOREPLIFETIME(AEODCharacterBase, LastReceivedHit);
	DOREPLIFETIME(AEODCharacterBase, Health);
	DOREPLIFETIME(AEODCharacterBase, Mana);
	DOREPLIFETIME(AEODCharacterBase, bInCombat);

	DOREPLIFETIME_CONDITION(AEODCharacterBase, CharacterStateInfo, COND_SkipOwner);

	DOREPLIFETIME_CONDITION(AEODCharacterBase, bIsRunning, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, bWeaponSheathed, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, bPCTryingToMove, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, BlockMovementDirectionYaw, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, CharacterMovementDirection, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AEODCharacterBase, bCharacterStateAllowsMovement, COND_SkipOwner);
}

void AEODCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Intentional additional call to BindUIDelegates (another in Restart())
	BindUIDelegates();

	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
	if (MoveComp)
	{
		MoveComp->SetDesiredCustomRotation(GetActorRotation());
	}



}

void AEODCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AEODCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// @todo - Enable interaction sphere on client.
	if (NewController && NewController->IsLocalPlayerController())
	{
		// EnableInteractionSphere();
	}
	else
	{
		// DisableInteractionSphere();
	}
}

void AEODCharacterBase::UnPossessed()
{
	Super::UnPossessed();
}

void AEODCharacterBase::Restart()
{
	Super::Restart();

	// Intentional additional call to BindUIDelegates (another in BeginPlay())
	BindUIDelegates();
}

float AEODCharacterBase::BP_GetRotationYawFromAxisInput()
{
	return GetRotationYawFromAxisInput();
}

void AEODCharacterBase::Display3DDamageNumbers(
	const float DamageValue,
	const bool bCritHit,
	const AActor* DamagedActor,
	const AActor* DamageInstigator,
	const FVector& HitLocation)
{
	UEODGameInstance* EODGI = Cast<UEODGameInstance>(GetGameInstance());
	APlayerController* LPC = EODGI ? EODGI->GetFirstLocalPlayerController() : nullptr;
	UClass* WidgetClass = EODGI ? EODGI->DamageWidgetClass.Get() : nullptr;

	if (!LPC || !WidgetClass)
	{
		return;
	}

	APawn* LocalPlayerPawn = LPC->GetPawn();
	check(LocalPlayerPawn);

	// If neither the damaged actor or damage instigator is a player controlled character, then we do not want to display damage
	if (LocalPlayerPawn != DamagedActor && LocalPlayerPawn != DamageInstigator)
	{
		return;
	}

	UDamageNumberWidget* DamageWidget = CreateWidget<UDamageNumberWidget>(LPC, WidgetClass);
	if (DamageWidget)
	{
		FVector2D WidgetSize = FVector2D(72, 60);
		DamageWidget->SetDamageValue(DamageValue);
		FLinearColor FinalColor;
		if (DamagedActor == LocalPlayerPawn)
		{
			FinalColor = EODGI->PlayerDamagedTextColor;
		}
		else
		{
			FinalColor = bCritHit ? EODGI->NPCCritDamagedTextColor : EODGI->NPCNormalDamagedTextColor;
		}
		DamageWidget->SetDamageColor(FinalColor);
	}

	UEODWidgetComponent* WidgetComp = NewObject<UEODWidgetComponent>(this, UEODWidgetComponent::StaticClass(), NAME_None, RF_Transient);
	WidgetComp->RegisterComponent();
	WidgetComp->SetWorldLocation(HitLocation);
	WidgetComp->SetWidget(DamageWidget);
	WidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WidgetComp->SetTickWhenOffscreen(true);
	WidgetComp->bOrientPitch = false;
	
	//~ @todo set proper widget size;

	WidgetComp->Activate();
	WidgetComponents.Add(WidgetComp);

	UWorld* World = GetWorld();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AEODCharacterBase::RemoveWidgetComponent, WidgetComp);

	FTimerHandle TempHandle;
	World->GetTimerManager().SetTimer(TempHandle, TimerDelegate, 5.f, false);

	int32 Size = WidgetComponents.Num();
	PrintToScreen(this, FString::FromInt(Size), 10.f);
}

float AEODCharacterBase::BP_GetControllerRotationYaw() const
{
	return (Controller ? FMath::UnwindDegrees(Controller->GetControlRotation().Yaw) : 0.0f);
}

/*
void AEODCharacterBase::CreateAndDisplayTextOnPlayerScreen_Implementation(const FString& Message, const FLinearColor& TextColor, const FVector& TextPosition)
{
}
*/

void AEODCharacterBase::TriggeriFrames(float Duration, float Delay)
{
	if (Role < ROLE_Authority)
	{
		Server_TriggeriFrames(Duration, Delay);
	}
	else
	{
		UWorld* World = GetWorld();
		check(World);
		// Using a delegate because I need to pass an argument to EnableiFrames function
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &AEODCharacterBase::EnableiFrames, Duration);
		World->GetTimerManager().SetTimer(DodgeImmunityTimerHandle, TimerDelegate, Delay, false);
	}
}

bool AEODCharacterBase::BP_HasBeenHit() const
{
	return HasBeenHit();
}

bool AEODCharacterBase::CanMove() const
{
	// Mobs can only move in IdleWalkRun state
	// return CharacterStateInfo.CharacterState == ECharacterState::IdleWalkRun || (IsUsingAnySkill() && bSkillAllowsMovement);
	return CharacterStateInfo.CharacterState == ECharacterState::IdleWalkRun;
}

bool AEODCharacterBase::CanJump() const
{
	return CharacterStateInfo.CharacterState == ECharacterState::IdleWalkRun;
}

bool AEODCharacterBase::CanDodge() const
{
	return CharacterStateInfo.CharacterState == ECharacterState::IdleWalkRun;
}

bool AEODCharacterBase::CanGuardAgainstAttacks() const
{
	return (IsIdleOrMoving() || IsNormalAttacking()) && !(IsWeaponSheathed());
}

void AEODCharacterBase::AddGameplayTagModifier(FGameplayTagMod TagMod)
{
	if (!TagMod.Tags.IsEmpty())
	{
		TagModifiers.AddUnique(TagMod);
		GameplayTagContainer.AppendTags(TagMod.Tags);
	}
}

void AEODCharacterBase::RemoveGameplayTagModifier(FGameplayTagMod TagMod)
{
	if (!TagMod.Tags.IsEmpty())
	{
		if (TagModifiers.Contains(TagMod))
		{
			TagModifiers.Remove(TagMod);
			GameplayTagContainer.RemoveTags(TagMod.Tags);
		}

		for (const FGameplayTagMod& _TagMod : TagModifiers)
		{
			GameplayTagContainer.AppendTags(_TagMod.Tags);
		}
	}
}

bool AEODCharacterBase::IsAlive() const
{
	//~ @todo
	// return Health.CurrentValue > 0;
	return true;
}

bool AEODCharacterBase::IsDead() const
{
	//~ @todo
	// return Health.CurrentValue <= 0;
	return false;
}

bool AEODCharacterBase::CanRespawn() const
{
	return false;
}

bool AEODCharacterBase::NeedsHealing() const
{
	return false;
}

bool AEODCharacterBase::CanNormalAttack() const
{
	return IsIdleOrMoving();
}

bool AEODCharacterBase::CanUseAnySkill() const
{
	return IsIdleOrMoving();
}

bool AEODCharacterBase::CanUseSkill(FName SkillID, UGameplaySkillBase* Skill)
{
	return false;
}

void AEODCharacterBase::OnSkillActivated(uint8 SkillIndex, FName SkillGroup, UGameplaySkillBase* Skill)
{
}

bool AEODCharacterBase::IsHealing() const
{
	return false;
}

bool AEODCharacterBase::CCEFlinch(const float BCAngle)
{
	return false;
}

bool AEODCharacterBase::CCEInterrupt(const float BCAngle)
{
	return false;
}

bool AEODCharacterBase::CCEStun(const float Duration)
{
	return false;
}

void AEODCharacterBase::CCERemoveStun()
{
}

bool AEODCharacterBase::CCEFreeze(const float Duration)
{
	return false;
}

void AEODCharacterBase::CCEUnfreeze()
{
}

bool AEODCharacterBase::CCEKnockdown(const float Duration)
{
	return false;
}

void AEODCharacterBase::CCEEndKnockdown()
{
}

bool AEODCharacterBase::CCEKnockback(const float Duration, const FVector & ImpulseDirection)
{
	return false;
}

void AEODCharacterBase::PushBack_Implementation(const FVector& ImpulseDirection)
{
}

void AEODCharacterBase::DodgeAttack(AActor* HitInstigator, ICombatInterface* InstigatorCI, const TSharedPtr<FAttackInfo>& AttackInfoPtr)
{
	UAttackDodgedEvent* DodgeEvent = NewObject<UAttackDodgedEvent>(this, UAttackDodgedEvent::StaticClass(), FName("DodgeEvent"), RF_Transient);
	DodgeEvent->AddToRoot();
	this->OnDodgingAttack.Broadcast(this, HitInstigator, this, DodgeEvent);
	DodgeEvent->RemoveFromRoot();
	DodgeEvent->MarkPendingKill();
}

void AEODCharacterBase::BlockAttack(AActor* HitInstigator, ICombatInterface* InstigatorCI, const TSharedPtr<FAttackInfo>& AttackInfoPtr)
{
	//~ @todo block event
	PlayAttackBlockedAnimation();
}

void AEODCharacterBase::InitiateDeathSequence_Implementation()
{
}

void AEODCharacterBase::CreateGhostTrail_Implementation()
{
}

void AEODCharacterBase::PlayAttackBlockedAnimation()
{
}

void AEODCharacterBase::EnableiFrames(float Duration)
{
	UWorld* World = GetWorld();
	if (Duration > 0.f && World)
	{
		bActiveiFrames = true;
		World->GetTimerManager().SetTimer(DodgeImmunityTimerHandle, this, &AEODCharacterBase::DisableiFrames, Duration, false);
	}
}

void AEODCharacterBase::DisableiFrames()
{
	bActiveiFrames = false;
}

void AEODCharacterBase::BindUIDelegates()
{
	//~ @todo
	/*
	if (GetController() && GetController()->IsLocalPlayerController())
	{
		AEODPlayerController* PC = Cast<AEODPlayerController>(Controller);
		if (IsValid(PC))
		{
			PC->InitializeHUDWidget();
			// PC->CreateHUDWidget();
		}
		else
		{
			return;
		}

		if (IsValid(PC->GetHUDWidget()))
		{
			UStatusIndicatorWidget* StatusIndicatorWidget = PC->GetHUDWidget()->GetStatusIndicatorWidget();
			if (IsValid(CharacterStatsComponent) && IsValid(StatusIndicatorWidget))
			{
				CharacterStatsComponent->OnHealthChanged.AddUniqueDynamic(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateHealthBar);
				CharacterStatsComponent->OnManaChanged.AddUniqueDynamic(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateManaBar);
				CharacterStatsComponent->OnStaminaChanged.AddUniqueDynamic(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateStaminaBar);
			}

			UDynamicSkillBarWidget* SkillBarWidget = PC->GetHUDWidget()->GetSkillBarWidget();
			// if (IsValid(SkillManager) && IsValid(SkillBarWidget))
			{
				// The saved skill bar layout may not have already been loaded
				// SkillManager->LoadSkillBarLayout();
				// SkillBarWidget->UpdateSkillBarLayout(SkillManager->GetSkillBarLayout());
				// SkillBarWidget->OnNewSkillAdded.AddUniqueDynamic(SkillManager, &UGameplaySkillsComponent::AddNewSkill);
			}
		}
	}
	*/
}

void AEODCharacterBase::UnbindUIDelegates()
{
}

void AEODCharacterBase::ResetTickDependentData()
{
	bDesiredRotationYawFromAxisInputUpdated = false;
}

void AEODCharacterBase::EnableDamageBlocking()
{
	bBlockingDamage = true;
}

void AEODCharacterBase::DisableDamageBlocking()
{
	bBlockingDamage = false;
	// Clear block damage timer just in case it is still active
	UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().ClearTimer(DamageBlockingTimerHandle);
}

bool AEODCharacterBase::BP_IsInCombat() const
{
	return IsInCombat();
}

AActor* AEODCharacterBase::GetInterfaceOwner()
{
	return this;
}

TSharedPtr<FAttackInfo> AEODCharacterBase::GetAttackInfoPtr(const FName& SkillGroup, const int32 CollisionIndex)
{
	UGameplaySkillBase* Skill = SkillManager ? SkillManager->GetSkillForSkillGroup(SkillGroup) : nullptr;
	TSharedPtr<FAttackInfo> AttackInfoPtr = Skill ? Skill->GetAttackInfoPtr(CollisionIndex) : TSharedPtr<FAttackInfo>();
	return AttackInfoPtr;
}

TSharedPtr<FAttackResponse> AEODCharacterBase::ReceiveAttack(
	AActor* HitInstigator,
	ICombatInterface* InstigatorCI,
	const TSharedPtr<FAttackInfo>& AttackInfoPtr,
	const FHitResult& DirectHitResult,
	const bool bLineHitResultFound,
	const FHitResult& LineHitResult)
{
	UStatsComponentBase* StatsComp = GetStatsComponent();
	if (!StatsComp || !InstigatorCI || !AttackInfoPtr.IsValid())
	{
		return TSharedPtr<FAttackResponse>();
	}

	FReceivedHitInfo ReceivedHitInfo;
	ReceivedHitInfo.HitInstigator = HitInstigator;

	// Handle dodge
	if (!AttackInfoPtr->bUndodgable && this->IsDodgingDamage())
	{
		DodgeAttack(HitInstigator, InstigatorCI, AttackInfoPtr);

		// Replicate Hit Info
		ReceivedHitInfo.DamageResult = EDamageResult::Dodged;
		ReceivedHitInfo.ReplicationIndex = GetLastReceivedHitInfo().ReplicationIndex + 1;
		SetLastReceivedHitInfo(ReceivedHitInfo);


		TSharedPtr<FAttackResponse> AttackResponsePtr = TSharedPtr<FAttackResponse>(new FAttackResponse);
		AttackResponsePtr->DamageResult = EDamageResult::Dodged;
		return AttackResponsePtr;
	}

	//~ possible alternative to current damage blocking
	/*
	bool bAttackBlocked = false;
	if (!AttackInfoPtr->bUnblockable && this->IsBlockingDamage())
	{
		const FVector& HIVec = HitInstigator->GetActorForwardVector();
		const FVector& ForVec = this->GetActorForwardVector();

		float Angle = UEODBlueprintFunctionLibrary::CalculateAngleBetweenVectors(HIVec, ForVec);
		if (Angle >= 120)
		{
			bAttackBlocked = true;
		}
	}
	*/

	ReceivedHitInfo.bCritHit = AttackInfoPtr->CritRate >= FMath::RandRange(0.f, 100.f) ? true : false;

	bool bAttackBlocked = false;
	if (bLineHitResultFound)
	{
		ReceivedHitInfo.BCAngle = UEODBlueprintFunctionLibrary::CalculateAngleBetweenVectors(GetActorForwardVector(), LineHitResult.ImpactNormal);
		if (!AttackInfoPtr->bUnblockable && this->IsBlockingDamage())
		{
			bAttackBlocked = ReceivedHitInfo.BCAngle < UCombatLibrary::BlockDetectionAngle ? true : false;
			if (bAttackBlocked)
			{
				BlockAttack(HitInstigator, InstigatorCI, AttackInfoPtr);
				ReceivedHitInfo.DamageResult = EDamageResult::Blocked;
			}
		}
	}
	else
	{
		ReceivedHitInfo.BCAngle = UEODBlueprintFunctionLibrary::CalculateAngleBetweenVectors(GetActorForwardVector(), DirectHitResult.ImpactNormal);
	}

	bool bCCEApplied = ApplyCCE(
		HitInstigator,
		AttackInfoPtr->CrowdControlEffect,
		AttackInfoPtr->CrowdControlEffectDuration,
		ReceivedHitInfo.BCAngle,
		bAttackBlocked);

	ReceivedHitInfo.CrowdControlEffect = bCCEApplied ? AttackInfoPtr->CrowdControlEffect : ECrowdControlEffect::Flinch;
	ReceivedHitInfo.CrowdControlEffectDuration = AttackInfoPtr->CrowdControlEffectDuration;
	ReceivedHitInfo.ActualDamage = GetActualDamage(HitInstigator, InstigatorCI, AttackInfoPtr, ReceivedHitInfo.bCritHit, bAttackBlocked);

	if (!bAttackBlocked && ReceivedHitInfo.ActualDamage == 0)
	{
		ReceivedHitInfo.DamageResult = EDamageResult::Nullified;
	}

	ReceivedHitInfo.HitLocation = bLineHitResultFound ? LineHitResult.ImpactPoint : DirectHitResult.ImpactPoint;
	UPhysicalMaterial* PhysMat = LineHitResult.PhysMaterial.Get();
	if (PhysMat)
	{
		ReceivedHitInfo.HitSurface = PhysMat->SurfaceType;
	}

	ReceivedHitInfo.ReplicationIndex = GetLastReceivedHitInfo().ReplicationIndex + 1;
	SetLastReceivedHitInfo(ReceivedHitInfo);

	StatsComp->Health.ModifyCurrentValue(-ReceivedHitInfo.ActualDamage);
	TriggerReceivedHitCosmetics(ReceivedHitInfo);

	TSharedPtr<FAttackResponse> AttackResponsePtr = 
		TSharedPtr<FAttackResponse>(
			new FAttackResponse(
				ReceivedHitInfo.DamageResult,
				ReceivedHitInfo.CrowdControlEffect,
				ReceivedHitInfo.ActualDamage,
				ReceivedHitInfo.bCritHit
			));

	return AttackResponsePtr;
}

float AEODCharacterBase::GetActualDamage(
	AActor* HitInstigator,
	ICombatInterface* InstigatorCI,
	const TSharedPtr<FAttackInfo>& AttackInfoPtr,
	const bool bCritHit,
	const bool bAttackBlocked)
{
	float ActualDamage = 0.f;

	UStatsComponentBase* StatsComp = GetStatsComponent();
	if (!StatsComp || !AttackInfoPtr.IsValid())
	{
		return ActualDamage;
	}

	if (bCritHit)
	{
		ActualDamage = AttackInfoPtr->CritDamage;
	}
	else
	{
		ActualDamage = AttackInfoPtr->NormalDamage;
	}

	if (bAttackBlocked)
	{
		float DamageReductionOnBlock = 0.1f;
		if (StatsComp)
		{
			if (AttackInfoPtr->DamageType == EDamageType::Magickal)
			{
				DamageReductionOnBlock = StatsComp->MagickalDamageReductionOnBlock.GetValue();
			}
			else
			{
				DamageReductionOnBlock = StatsComp->PhysicalDamageReductionOnBlock.GetValue();
			}
		}
		DamageReductionOnBlock = DamageReductionOnBlock > 100.f ? 100.f : DamageReductionOnBlock;
		ActualDamage = ActualDamage * (1 - DamageReductionOnBlock / 100.f);
	}

	return ActualDamage;
}

void AEODCharacterBase::TriggerReceivedHitCosmetics(const FReceivedHitInfo& HitInfo)
{
	if (HitInfo.DamageResult == EDamageResult::Dodged)
	{
		//~ @todo Display dodge message
		//~ @todo Play dodge sound
	}
	else if (HitInfo.DamageResult == EDamageResult::Blocked)
	{
		//~ @todo Play attack blocked animation
		//~ @todo Play attack blocked sound
	}
	else if (HitInfo.DamageResult == EDamageResult::Immune)
	{
		//~ @todo Display immune message
	}
	else if (HitInfo.DamageResult == EDamageResult::Nullified || HitInfo.DamageResult == EDamageResult::Damaged)
	{
		SetOffTargetSwitch(TargetSwitchDuration);

	}

	UEODGameInstance* EODGI = Cast<UEODGameInstance>(GetGameInstance());
	if (EODGI)
	{
		EODGI->DisplayDamageNumbers(
			LastReceivedHit.ActualDamage,
			LastReceivedHit.bCritHit,
			this,
			LastReceivedHit.HitInstigator,
			LastReceivedHit.HitLocation);
	}

	ICombatInterface* InstigatorCI = Cast<ICombatInterface>(LastReceivedHit.HitInstigator);
	if (InstigatorCI)
	{
		USoundBase* Sound = InstigatorCI->GetMeleeHitSound(LastReceivedHit.HitSurface, LastReceivedHit.bCritHit);
		if (Sound && GameplayAudioComponent)
		{
			GameplayAudioComponent->SetSound(Sound);
			GameplayAudioComponent->Play();

		}
	}
}

void AEODCharacterBase::PreCCEStateEnter()
{
	if (IsUsingAnySkill())
	{
		UGameplaySkillsComponent* SkillsComponent = GetGameplaySkillsComponent();
		check(SkillsComponent);
		SkillsComponent->CancelAllActiveSkills();
	}
	else if (IsDodging())
	{
		CancelDodge();
	}
	else if (IsBlocking())
	{
		StopBlockingAttacks();
	}

}

bool AEODCharacterBase::ApplyCCE(
	AActor* HitInstigator,
	ECrowdControlEffect CCEToApply,
	float CCEDuration,
	float BCAngle,
	bool bAttackBlocked)
{
	bool bCCEApplied = false;
	if (!bAttackBlocked)
	{
		switch (CCEToApply)
		{
		case ECrowdControlEffect::Flinch:
			bCCEApplied = CCEFlinch(BCAngle);
			break;
		case ECrowdControlEffect::Interrupt:
			bCCEApplied = CCEInterrupt(BCAngle);
			break;
		case ECrowdControlEffect::KnockedDown:
			bCCEApplied = CCEKnockdown(CCEDuration);
			// If the character has a controller it means either character is locally controlled or on server, 
			// so we don't need check for ENetRole
			//~ @note:	Previously I was rotating the character only on server, which resulted in character rotating
			//			everywhere except for the client owner. So now, I am forcing the rotation on client owner too.
			//~ @see:	Bug # (@todo bug index)
			if (bCCEApplied && Controller)
			{
				FVector OrientationVector = HitInstigator->GetActorLocation() - GetActorLocation();
				FRotator OrientationRotator = OrientationVector.ToOrientationRotator();

				float DesiredYaw = OrientationRotator.Yaw;
				UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
				if (MoveComp)
				{
					MoveComp->SetDesiredCustomRotationYaw(DesiredYaw);
				}

				SetCharacterRotationYaw(DesiredYaw);
			}
			break;
		case ECrowdControlEffect::KnockedBack:
			bCCEApplied = CCEKnockback(CCEDuration, HitInstigator->GetActorForwardVector());
			if (bCCEApplied && Controller)
			{
				FVector OrientationVector = HitInstigator->GetActorLocation() - GetActorLocation();
				FRotator OrientationRotator = OrientationVector.ToOrientationRotator();

				float DesiredYaw = OrientationRotator.Yaw;

				UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
				if (MoveComp)
				{
					MoveComp->SetDesiredCustomRotationYaw(DesiredYaw);
				}

				SetCharacterRotationYaw(DesiredYaw);
			}
			break;
		case ECrowdControlEffect::Stunned:
			bCCEApplied = CCEStun(CCEDuration);
			break;
		case ECrowdControlEffect::Crystalized:
			bCCEApplied = CCEFreeze(CCEDuration);
			break;
		default:
			break;
		}
	}
	return bCCEApplied;
}

TSharedPtr<FAttackInfo> AEODCharacterBase::GetAttackInfoPtrFromNormalAttack(const FString& NormalAttackStr)
{
	if (Role < ROLE_Authority)
	{
		return TSharedPtr<FAttackInfo>();
	}

	int32 NormalAttackIndex = GetAttackIndexFromNormalAttackString(NormalAttackStr);
	check(NormalAttackIndex != 0);
	
	float DamagePercent = 100.f;
	if (NormalAttackIndex > 10)
	{
		DamagePercent = 150.f;
	}
	else
	{
		EWeaponType WeaponType = GetWeaponTypeFromNormalAttackString(NormalAttackStr);
		check(WeaponType != EWeaponType::None);

		switch (WeaponType)
		{
		case EWeaponType::GreatSword:
		case EWeaponType::WarHammer:
			DamagePercent += ((NormalAttackIndex - 1) * 25);
			break;
		case EWeaponType::LongSword:
			break;
		case EWeaponType::Mace:
			break;
		case EWeaponType::Dagger:
			DamagePercent += ((NormalAttackIndex - 1) * 10);
			break;
		case EWeaponType::Staff:
			DamagePercent += ((NormalAttackIndex - 1) * 12);
			break;
		case EWeaponType::Shield:
		case EWeaponType::None:
		default:
			DamagePercent = 0.f;
			break;
		}
	}

	EDamageType DamageType = EDamageType::Physical;
	UStatsComponentBase* StatsComp = GetStatsComponent();
	check(StatsComp && DamagePercent != 0.f);

	float CritRate = DamageType == EDamageType::Magickal ? StatsComp->MagickalCritRate.GetValue() : StatsComp->PhysicalCritRate.GetValue();
	float NormalDamage =
		DamageType == EDamageType::Magickal ?
		((DamagePercent / 100.f) * StatsComp->MagickalAttack.GetValue()) :
		((DamagePercent / 100.f) * StatsComp->PhysicalAttack.GetValue());
	float CritDamage =
		DamageType == EDamageType::Magickal ?
		(NormalDamage * UCombatLibrary::MagickalCritMultiplier + StatsComp->MagickalCritBonus.GetValue()) :
		(NormalDamage * UCombatLibrary::PhysicalCritMultiplier + StatsComp->PhysicalCritBonus.GetValue());

	TSharedPtr<FAttackInfo> AttackInfoPtr =
		MakeShareable(new FAttackInfo(
			false,
			false,
			CritRate,
			NormalDamage,
			CritDamage,
			DamageType,
			ECrowdControlEffect::Flinch,
			0.f
		));

	return AttackInfoPtr;
}

EWeaponType AEODCharacterBase::GetWeaponTypeFromNormalAttackString(const FString& NormalAttackStr)
{
	if (NormalAttackStr.StartsWith(TEXT("gs"), ESearchCase::CaseSensitive))
	{
		return EWeaponType::GreatSword;
	}
	else if (NormalAttackStr.StartsWith(TEXT("wh"), ESearchCase::CaseSensitive))
	{
		return EWeaponType::WarHammer;
	}
	else if (NormalAttackStr.StartsWith(TEXT("ls"), ESearchCase::CaseSensitive))
	{
		return EWeaponType::LongSword;
	}
	else if (NormalAttackStr.StartsWith(TEXT("mc"), ESearchCase::CaseSensitive))
	{
		return EWeaponType::Mace;
	}
	else if (NormalAttackStr.StartsWith(TEXT("st"), ESearchCase::CaseSensitive))
	{
		return EWeaponType::Staff;
	}
	else if (NormalAttackStr.StartsWith(TEXT("dg"), ESearchCase::CaseSensitive))
	{
		return EWeaponType::Dagger;
	}

	return EWeaponType::None;
}

int32 AEODCharacterBase::GetAttackIndexFromNormalAttackString(const FString& NormalAttackStr)
{
	if (NormalAttackStr.EndsWith(TEXT("01"), ESearchCase::CaseSensitive))
	{
		return 1;
	}
	else if (NormalAttackStr.EndsWith(TEXT("02"), ESearchCase::CaseSensitive))
	{
		return 2;
	}
	else if (NormalAttackStr.EndsWith(TEXT("03"), ESearchCase::CaseSensitive))
	{
		return 3;
	}
	else if (NormalAttackStr.EndsWith(TEXT("04"), ESearchCase::CaseSensitive))
	{
		return 4;
	}
	else if (NormalAttackStr.EndsWith(TEXT("05"), ESearchCase::CaseSensitive))
	{
		return 5;
	}
	else if (NormalAttackStr.EndsWith(TEXT("11"), ESearchCase::CaseSensitive))
	{
		return 11;
	}
	else if (NormalAttackStr.EndsWith(TEXT("12"), ESearchCase::CaseSensitive))
	{
		return 12;
	}

	return 0;	
}

void AEODCharacterBase::BP_SetWalkSpeed(const float WalkSpeed)
{
	SetWalkSpeed(WalkSpeed);
}

void AEODCharacterBase::BP_SetCharacterRotation(const FRotator NewRotation)
{
	SetCharacterRotation(NewRotation);
}

void AEODCharacterBase::BP_SetUseControllerRotationYaw(const bool bNewBool)
{
	SetUseControllerRotationYaw(bNewBool);
}

EEODTaskStatus AEODCharacterBase::CheckSkillStatus(FName SkillID)
{
	return EEODTaskStatus();
}

FName AEODCharacterBase::GetMostWeightedMeleeSkillID(AEODCharacterBase const * const TargetCharacter) const
{
	return FName();
}

FName AEODCharacterBase::GetCurrentActiveSkillID() const
{
	FName ActiveSkillID = NAME_None;
	if (SkillManager)
	{
		TArray<UGameplaySkillBase*> ActiveSkills = SkillManager->GetActiveSkills();
		if (ActiveSkills.Num() > 0)
		{
			UGameplaySkillBase* Skill = ActiveSkills[0];
			if (Skill)
			{
				ActiveSkillID = Skill->GetSkillGroup();
			}
		}
	}
	return ActiveSkillID;
}

UGameplaySkillBase* AEODCharacterBase::GetCurrentActiveSkill() const
{
	if (SkillManager)
	{
		TArray<UGameplaySkillBase*> ActiveSkills = SkillManager->GetActiveSkills();
		if (ActiveSkills.Num() > 0)
		{
			UGameplaySkillBase* Skill = ActiveSkills[0];
			return Skill;
		}
	}
	return nullptr;
}

const FLastUsedSkillInfo& AEODCharacterBase::BP_GetLastUsedSkill() const
{
	return GetLastUsedSkill();
}

void AEODCharacterBase::SetLastUsedSkill(const FLastUsedSkillInfo& SkillInfo)
{
	LastUsedSkillInfo = SkillInfo;
}

UGameplaySkillBase* AEODCharacterBase::GetSkill(FName SkillID) const
{
	return SkillManager ? SkillManager->GetSkillForSkillGroup(SkillID) : nullptr;
}

bool AEODCharacterBase::UseSkill_Implementation(FName SkillID, UGameplaySkillBase* Skill)
{
	return false;
}

void AEODCharacterBase::OnMontageBlendingOut(UAnimMontage * AnimMontage, bool bInterrupted)
{
}

void AEODCharacterBase::OnMontageEnded(UAnimMontage * AnimMontage, bool bInterrupted)
{
}

void AEODCharacterBase::RemoveWidgetComponent(UEODWidgetComponent* WidgetComp)
{
	if (WidgetComp && WidgetComponents.Contains(WidgetComp))
	{
		WidgetComponents.Remove(WidgetComp);
		WidgetComp->UnregisterComponent();
		WidgetComp->Deactivate();
		WidgetComp->DestroyComponent();
	}
}

bool AEODCharacterBase::DeltaRotateCharacterToDesiredYaw(float DesiredYaw, float DeltaTime, float Precision, float RotationRate)
{
	float CurrentYaw = GetActorRotation().Yaw;
	if (FMath::IsNearlyEqual(CurrentYaw, DesiredYaw, Precision))
	{
		return true;
	}

	float DeltaRotationYaw = (RotationRate >= 0.f) ? (RotationRate * DeltaTime) : 360.f;
	float NewRotationYaw = FMath::FixedTurn(CurrentYaw, DesiredYaw, DeltaRotationYaw);
	SetCharacterRotation(FRotator(0.f, NewRotationYaw, 0.f));

	if (FMath::IsNearlyEqual(CurrentYaw, NewRotationYaw, Precision))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AEODCharacterBase::Die(ECauseOfDeath CauseOfDeath, AActor* EventInstigator, AController* EventOwner)
{
	if (bGodMode || IsDead())
	{
		// cannot die
		return;
	}

	if (CauseOfDeath == ECauseOfDeath::ZeroHP)
	{

	}
	else
	{
		// Set current hp to 0
		//~ @todo
		// GetCharacterStatsComponent()->ModifyBaseHealth(-GetCharacterStatsComponent()->GetMaxHealth());
		// SetCharacterState(ECharacterState::Dead);

		// @todo play death animation and death sound
	}
}

float AEODCharacterBase::GetOrientationYawToActor(const AActor* TargetActor)
{
	FVector OrientationVector = TargetActor->GetActorLocation() - GetActorLocation();
	FRotator OrientationRotator = OrientationVector.ToOrientationRotator();
	return OrientationRotator.Yaw;
}

void AEODCharacterBase::UpdateHealth(int32 MaxHealth, int32 CurrentHealth)
{
	Health = FCharacterStat(MaxHealth, CurrentHealth);

	if (Health.CurrentValue <= 0)
	{
		InitiateDeathSequence();
	}
}

void AEODCharacterBase::UpdateMana(int32 MaxMana, int32 CurrentMana)
{
	Mana = FCharacterStat(MaxMana, CurrentMana);
}

void AEODCharacterBase::SetCharacterLevel(int32 NewLevel)
{
	InGameLevel = NewLevel;
}

void AEODCharacterBase::TurnOnTargetSwitch()
{
	if (GetMesh())
	{
		GetMesh()->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 1.f);
	}
}

void AEODCharacterBase::TurnOffTargetSwitch()
{
	if (GetMesh())
	{
		GetMesh()->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 0.f);
	}
}

void AEODCharacterBase::OnRep_WeaponSheathed()
{
	StartWeaponSwitch();
	// PlayToggleSheatheAnimation();
}

void AEODCharacterBase::OnRep_InCombat()
{
}

void AEODCharacterBase::OnRep_CharacterState(ECharacterState OldState)
{
	//~ @todo : Cleanup old state
}

void AEODCharacterBase::OnRep_CharacterStateInfo(const FCharacterStateInfo& OldStateInfo)
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
		StartNormalAttack();
	}
	else if (CharacterStateInfo.CharacterState == ECharacterState::UsingActiveSkill)
	{
		if (SkillManager)
		{
			if (CharacterStateInfo.SubStateIndex > 100)
			{
				SkillManager->ReleaseSkill(CharacterStateInfo.SubStateIndex);
			}
			else
			{
				SkillManager->TriggerSkill(CharacterStateInfo.SubStateIndex);
			}
		}
	}
}

void AEODCharacterBase::OnRep_LastReceivedHit(const FReceivedHitInfo& OldHitInfo)
{
	bool bAttackBlocked = LastReceivedHit.DamageResult == EDamageResult::Blocked ? true : false;
	ApplyCCE(LastReceivedHit.HitInstigator, LastReceivedHit.CrowdControlEffect, LastReceivedHit.CrowdControlEffectDuration, LastReceivedHit.BCAngle, bAttackBlocked);

	TriggerReceivedHitCosmetics(LastReceivedHit);
}

void AEODCharacterBase::OnRep_Health(FCharacterStat& OldHealth)
{
}

void AEODCharacterBase::OnRep_Mana(FCharacterStat& OldMana)
{
}

void AEODCharacterBase::OnRep_InGameLevel(int32 OldLevel)
{

}

void AEODCharacterBase::OnRep_LastAttackResponses()
{
}

void AEODCharacterBase::Server_Dodge_Implementation(uint8 DodgeIndex, float RotationYaw)
{
}

bool AEODCharacterBase::Server_Dodge_Validate(uint8 DodgeIndex, float RotationYaw)
{
	return true;
}

void AEODCharacterBase::Server_StartBlockingAttacks_Implementation()
{
	if (IsNormalAttacking())
	{
		CancelNormalAttack();
	}

	FCharacterStateInfo StateInfo(ECharacterState::Blocking);
	CharacterStateInfo = StateInfo;
	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
	if (MoveComp)
	{
		MoveComp->bUseControllerDesiredRotation = true;
	}
	
	StartBlockingDamage(DamageBlockTriggerDelay);
	StartBlockingAttacks();
}

bool AEODCharacterBase::Server_StartBlockingAttacks_Validate()
{
	return true;
}

void AEODCharacterBase::Server_StopBlockingAttacks_Implementation()
{
	StopBlockingDamage();
	ResetState();
}

bool AEODCharacterBase::Server_StopBlockingAttacks_Validate()
{
	return true;
}

void AEODCharacterBase::Server_NormalAttack_Implementation(uint8 AttackIndex)
{
}

bool AEODCharacterBase::Server_NormalAttack_Validate(uint8 AttackIndex)
{
	return true;
}

void AEODCharacterBase::Server_SpawnAndMountRideableCharacter_Implementation(TSubclassOf<ARideBase> RideCharacterClass)
{
	SpawnAndMountRideableCharacter(RideCharacterClass);
}

bool AEODCharacterBase::Server_SpawnAndMountRideableCharacter_Validate(TSubclassOf<ARideBase> RideCharacterClass)
{
	return true;
}

/*
void AEODCharacterBase::Client_DisplayTextOnPlayerScreen_Implementation(const FString& Message, const FLinearColor& TextColor, const FVector& TextPosition)
{
	if (IsPlayerControlled())
	{
		CreateAndDisplayTextOnPlayerScreen(Message, TextColor, TextPosition);
	}
}
*/

void AEODCharacterBase::Server_StopBlockingDamage_Implementation()
{
	StopBlockingDamage();
}

bool AEODCharacterBase::Server_StopBlockingDamage_Validate()
{
	return true;
}

void AEODCharacterBase::Server_StartBlockingDamage_Implementation(float Delay)
{
	StartBlockingDamage(Delay);
}

bool AEODCharacterBase::Server_StartBlockingDamage_Validate(float Delay)
{
	return true;
}

void AEODCharacterBase::Server_TriggeriFrames_Implementation(float Duration, float Delay)
{
	TriggeriFrames(Duration, Delay);
}

bool AEODCharacterBase::Server_TriggeriFrames_Validate(float Duration, float Delay)
{
	return true;
}

void AEODCharacterBase::Server_SetNextMontageSection_Implementation(FName CurrentSection, FName NextSection, UAnimMontage* Montage)
{
	Multicast_SetNextMontageSection(CurrentSection, NextSection, Montage);
}

bool AEODCharacterBase::Server_SetNextMontageSection_Validate(FName CurrentSection, FName NextSection, UAnimMontage* Montage)
{
	return true;
}

void AEODCharacterBase::Server_SetUseControllerRotationYaw_Implementation(bool bNewBool)
{
	SetUseControllerRotationYaw(bNewBool);
}

bool AEODCharacterBase::Server_SetUseControllerRotationYaw_Validate(bool bNewBool)
{
	return true;
}

void AEODCharacterBase::Server_SetCharacterRotation_Implementation(FRotator NewRotation)
{
	SetActorRotation(NewRotation);
	// Multicast_SetCharacterRotation(NewRotation);
}

bool AEODCharacterBase::Server_SetCharacterRotation_Validate(FRotator NewRotation)
{
	return true;
}

void AEODCharacterBase::Server_SetCharacterRotationYaw_Implementation(float NewRotationYaw)
{
	const FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FRotator(CurrentRotation.Pitch, NewRotationYaw, CurrentRotation.Roll));
}

bool AEODCharacterBase::Server_SetCharacterRotationYaw_Validate(float NewRotationYaw)
{
	return true;
}

void AEODCharacterBase::Server_SetWalkSpeed_Implementation(float WalkSpeed)
{
	SetWalkSpeed(WalkSpeed);
}

bool AEODCharacterBase::Server_SetWalkSpeed_Validate(float WalkSpeed)
{
	return true;
}

void AEODCharacterBase::Multicast_SetNextMontageSection_Implementation(FName CurrentSection, FName NextSection, UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (!IsLocallyControlled() && AnimInstance)
	{
		AnimInstance->Montage_SetNextSection(CurrentSection, NextSection, Montage);
	}
}

void AEODCharacterBase::Server_PlayAnimMontage_Implementation(UAnimMontage* MontageToPlay, FName SectionToPlay)
{
	Multicast_PlayAnimMontage(MontageToPlay, SectionToPlay);
}

bool AEODCharacterBase::Server_PlayAnimMontage_Validate(UAnimMontage* MontageToPlay, FName SectionToPlay)
{
	return true;
}

void AEODCharacterBase::Multicast_PlayAnimMontage_Implementation(UAnimMontage* MontageToPlay, FName SectionToPlay)
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (!IsLocallyControlled() && AnimInstance)
	{
		AnimInstance->Montage_Play(MontageToPlay);
		AnimInstance->Montage_JumpToSection(SectionToPlay);
	}
}

void AEODCharacterBase::Multicast_HealthUpdated_Implementation(int32 BaseHealth, int32 MaxHealth, int32 CurrentHealth)
{
}

void AEODCharacterBase::Multicast_ManaUpdated_Implementation(int32 BaseMana, int32 MaxMana, int32 CurrentMana)
{
}

void AEODCharacterBase::StartDodge()
{
}

void AEODCharacterBase::CancelDodge()
{
}

void AEODCharacterBase::FinishDodge()
{
}

void AEODCharacterBase::StartBlockingAttacks()
{
	//~ @todo handle previous state cleanup

	bool bIsLocalPlayerController = Controller && Controller->IsLocalPlayerController();
	if (bIsLocalPlayerController)
	{
		if (IsNormalAttacking())
		{
			CancelNormalAttack();
		}

		FCharacterStateInfo StateInfo(ECharacterState::Blocking);
		StateInfo.NewReplicationIndex = CharacterStateInfo.NewReplicationIndex + 1;
		CharacterStateInfo = StateInfo;
		UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
		if (MoveComp)
		{
			MoveComp->bUseControllerDesiredRotation = true;
		}

		if (Role < ROLE_Authority)
		{
			Server_StartBlockingAttacks();
		}
		else
		{
			StartBlockingDamage(DamageBlockTriggerDelay);
		}
	}

	bCharacterStateAllowsMovement = true;
	bCharacterStateAllowsRotation = false;
}

void AEODCharacterBase::StopBlockingAttacks()
{
	if (Role < ROLE_Authority)
	{
		Server_StopBlockingAttacks();
	}
	
	ResetState();
}

void AEODCharacterBase::OnJumpAnimationStart()
{
	if (IsBlocking())
	{
		StopBlockingAttacks();
	}

	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
	if (MoveComp)
	{
		MoveComp->bUseControllerDesiredRotation = false;
		MoveComp->SetDesiredCustomRotationYaw_LocalOnly(GetActorRotation().Yaw);
	}
	bCharacterStateAllowsMovement = false;
	bCharacterStateAllowsRotation = false;

	// If the controller exists for this character, then either we are server or owner client
	if (Controller)
	{
		FCharacterStateInfo StateInfo(ECharacterState::Jumping);
		StateInfo.NewReplicationIndex = CharacterStateInfo.NewReplicationIndex + 1;
		CharacterStateInfo = StateInfo;
	}
}

void AEODCharacterBase::OnJumpAnimationFinish()
{
	ResetState();
}

void AEODCharacterBase::MoveForward(const float Value)
{
	ForwardAxisValue = Value;

	if (Value != 0 && CanMove())
	{
		FRotator Rotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
		FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AEODCharacterBase::MoveRight(const float Value)
{
	RightAxisValue = Value;

	if (Value != 0 && CanMove())
	{
		FVector Direction = FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AEODCharacterBase::UpdateRotation(float DeltaTime)
{
	if (bCharacterStateAllowsRotation)
	{
		FRotator DesiredRotation = FRotator(0.f, GetRotationYawFromAxisInput(), 0.f);
		UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
		if (MoveComp)
		{
			MoveComp->SetDesiredCustomRotation(DesiredRotation);
		}
	}
}

void AEODCharacterBase::UpdateMovement(float DeltaTime)
{
	if (!bCharacterStateAllowsMovement)
	{
		return;
	}

	// The movement speed calculations are done each frame because the movement speed modifier may change because of buffs.
	// But no server RPC is called unless the new movement speed is different from the last speed.
	if (IsBlocking())
	{
		float NewSpeed = DefaultWalkSpeedWhileBlocking * MovementSpeedModifier;
		SetWalkSpeed(NewSpeed);

		if (ForwardAxisValue == 0)
		{
			if (RightAxisValue > 0)
			{
				SetBlockMovementDirectionYaw(90.f);
			}
			else if (RightAxisValue < 0)
			{
				SetBlockMovementDirectionYaw(-90.f);
			}
			else
			{
				SetBlockMovementDirectionYaw(0.f);
			}
		}
		else
		{
			float NewYaw = FMath::RadiansToDegrees(FMath::Atan2(RightAxisValue, ForwardAxisValue));
			SetBlockMovementDirectionYaw(NewYaw);
		}
	}
	else if (IsIdleOrMoving())
	{
		UpdateCharacterMovementDirection();
		float DesiredSpeed = IsRunning() ? DefaultRunSpeed : DefaultWalkSpeed;
		if (ForwardAxisValue < 0)
		{
			float NewSpeed = (DesiredSpeed * MovementSpeedModifier) * (5.f / 16.f);
			SetWalkSpeed(NewSpeed);
		}
		else
		{
			float NewSpeed = DesiredSpeed * MovementSpeedModifier;
			SetWalkSpeed(NewSpeed);
		}
	}
	else
	{
		// Almost same as IsIdleOrMoving() except the PCTryingToMove update
		UpdatePCTryingToMove();
		UpdateCharacterMovementDirection();
		float DesiredSpeed = IsRunning() ? DefaultRunSpeed : DefaultWalkSpeed;
		if (ForwardAxisValue < 0)
		{
			float NewSpeed = (DesiredSpeed * MovementSpeedModifier) * (5.f / 16.f);
			SetWalkSpeed(NewSpeed);
		}
		else
		{
			float NewSpeed = DesiredSpeed * MovementSpeedModifier;
			SetWalkSpeed(NewSpeed);
		}
	}
}

bool AEODCharacterBase::CanStartInteraction() const
{
	return false;
}

void AEODCharacterBase::TriggerInteraction()
{
}

void AEODCharacterBase::StartInteraction()
{
}

void AEODCharacterBase::UpdateInteraction()
{
}

void AEODCharacterBase::CancelInteraction()
{
}

void AEODCharacterBase::FinishInteraction()
{
}

void AEODCharacterBase::ToggleSheathe()
{
}

void AEODCharacterBase::StartWeaponSwitch()
{
}

void AEODCharacterBase::CancelWeaponSwitch()
{
}

void AEODCharacterBase::FinishWeaponSwitch()
{
}

void AEODCharacterBase::StartNormalAttack()
{
}

void AEODCharacterBase::CancelNormalAttack()
{
}

void AEODCharacterBase::FinishNormalAttack()
{
}

void AEODCharacterBase::UpdateNormalAttackState(float DeltaTime)
{
}

void AEODCharacterBase::InitiateRotationToYawFromAxisInput()
{
	float UpdatedYaw = GetRotationYawFromAxisInput();
	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
	if (MoveComp)
	{
		MoveComp->SetDesiredCustomRotationYaw(UpdatedYaw);
	}
}

void AEODCharacterBase::OnPressedForward()
{
}

void AEODCharacterBase::OnPressedBackward()
{
}

void AEODCharacterBase::OnReleasedForward()
{
}

void AEODCharacterBase::OnReleasedBackward()
{
}

void AEODCharacterBase::ResetState()
{
	FCharacterStateInfo StateInfo(ECharacterState::IdleWalkRun);
	StateInfo.NewReplicationIndex = CharacterStateInfo.NewReplicationIndex + 1;
	CharacterStateInfo = StateInfo;
	bCharacterStateAllowsMovement = true;
	bCharacterStateAllowsRotation = true;
	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(GetCharacterMovement());
	if (MoveComp)
	{
		MoveComp->bUseControllerDesiredRotation = false;
	}
}

void AEODCharacterBase::SpawnAndMountRideableCharacter(TSubclassOf<ARideBase> RideCharacterClass)
{
	// Only call the server RPC if RideCharacterClass points to a valid class
	if (Role < ROLE_Authority && RideCharacterClass.Get())
	{
		Server_SpawnAndMountRideableCharacter(RideCharacterClass);
		return;
	}

	//~ @todo Cleanup/destroy ride spawned previously

	UWorld* World = GetWorld();
	AController* PlayerController = GetController();
	if (World && PlayerController && RideCharacterClass.Get())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;
		SpawnParams.Owner = this->Controller;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		CurrentRide = World->SpawnActor<ARideBase>(RideCharacterClass, GetActorLocation(), GetActorRotation(), SpawnParams);

		if (CurrentRide)
		{
			PlayerController->Possess(CurrentRide);
			CurrentRide->MountCharacter(this);
		}
	}
}

void AEODCharacterBase::OnMountingRide(ARideBase* RideCharacter)
{
	if (IsValid(RideCharacter))
	{
		UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
		// Disable movement and collision
		if (MoveComp && CapsuleComp)
		{
			MoveComp->DisableMovement();
			CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(RideCharacter->MountedCharacter_IdealAnimation);
		}
	}
}

UStatsComponentBase* AEODCharacterBase::GetStatsComponent() const
{
	AEODAIControllerBase* AIController = Cast<AEODAIControllerBase>(Controller);
	if (AIController)
	{
		return AIController->GetStatsComponent();
	}
	else
	{
		AEODPlayerController* PC = Cast<AEODPlayerController>(Controller);
		if (PC)
		{
			return PC->GetStatsComponent();
		}
	}

	return nullptr;
}

void AEODCharacterBase::Server_SetIsRunning_Implementation(bool bValue)
{
	SetIsRunning(bValue);
}

bool AEODCharacterBase::Server_SetIsRunning_Validate(bool bValue)
{
	return true;
}

void AEODCharacterBase::Server_SetCharacterStateAllowsMovement_Implementation(bool bNewValue)
{
	SetCharacterStateAllowsMovement(bNewValue);
}

bool AEODCharacterBase::Server_SetCharacterStateAllowsMovement_Validate(bool bNewValue)
{
	return true;
}

void AEODCharacterBase::Server_SetPCTryingToMove_Implementation(bool bNewValue)
{
	SetPCTryingToMove(bNewValue);
}

bool AEODCharacterBase::Server_SetPCTryingToMove_Validate(bool bNewValue)
{
	return true;
}

void AEODCharacterBase::Server_SetCharMovementDir_Implementation(ECharMovementDirection NewDirection)
{
	SetCharacterMovementDirection(NewDirection);
}

bool AEODCharacterBase::Server_SetCharMovementDir_Validate(ECharMovementDirection NewDirection)
{
	return true;
}

void AEODCharacterBase::Server_SetWeaponSheathed_Implementation(bool bNewValue)
{
	SetWeaponSheathed(bNewValue);
	StartWeaponSwitch();
}

bool AEODCharacterBase::Server_SetWeaponSheathed_Validate(bool bNewValue)
{
	return true;
}

void AEODCharacterBase::Server_SetBlockMovementDirectionYaw_Implementation(float NewYaw)
{
	SetBlockMovementDirectionYaw(NewYaw);
}

bool AEODCharacterBase::Server_SetBlockMovementDirectionYaw_Validate(float NewYaw)
{
	return true;
}

bool AEODCharacterBase::CanFlinch() const
{
	if (CharacterStateInfo.CharacterState == ECharacterState::Dead)
	{
		return false;
	}

	UStatsComponentBase* StatsComp = GetStatsComponent();
	if (StatsComp)
	{
		return !StatsComp->CCImmunities.HasCCImmunity(ECrowdControlEffect::Flinch);
	}
	return true;
}

bool AEODCharacterBase::CanStun() const
{
	if (CharacterStateInfo.CharacterState == ECharacterState::Dead)
	{
		return false;
	}

	UStatsComponentBase* StatsComp = GetStatsComponent();
	if (StatsComp)
	{
		return !StatsComp->CCImmunities.HasCCImmunity(ECrowdControlEffect::Stunned);
	}
	return true;
}

bool AEODCharacterBase::CanKnockdown() const
{
	if (CharacterStateInfo.CharacterState == ECharacterState::Dead)
	{
		return false;
	}

	UStatsComponentBase* StatsComp = GetStatsComponent();
	if (StatsComp)
	{
		return !StatsComp->CCImmunities.HasCCImmunity(ECrowdControlEffect::KnockedDown);
	}
	return true;
}

bool AEODCharacterBase::CanKnockback() const
{
	if (CharacterStateInfo.CharacterState == ECharacterState::Dead)
	{
		return false;
	}

	UStatsComponentBase* StatsComp = GetStatsComponent();
	if (StatsComp)
	{
		return !StatsComp->CCImmunities.HasCCImmunity(ECrowdControlEffect::KnockedBack);
	}
	return true;
}

bool AEODCharacterBase::CanFreeze() const
{
	if (CharacterStateInfo.CharacterState == ECharacterState::Dead)
	{
		return false;
	}

	UStatsComponentBase* StatsComp = GetStatsComponent();
	if (StatsComp)
	{
		return !StatsComp->CCImmunities.HasCCImmunity(ECrowdControlEffect::Crystalized);
	}
	return true;
}

bool AEODCharacterBase::CanInterrupt() const
{
	if (CharacterStateInfo.CharacterState == ECharacterState::Dead)
	{
		return false;
	}

	UStatsComponentBase* StatsComp = GetStatsComponent();
	if (StatsComp)
	{
		return !StatsComp->CCImmunities.HasCCImmunity(ECrowdControlEffect::Interrupt);
	}
	return true;
}
