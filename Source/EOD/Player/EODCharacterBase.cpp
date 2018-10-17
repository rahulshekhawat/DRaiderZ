// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODCharacterBase.h"
#include "CharAnimInstance.h"
#include "Core/EODPreprocessors.h"
#include "Components/StatsComponentBase.h"
#include "Statics/EODBlueprintFunctionLibrary.h"

#include "UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AEODCharacterBase::AEODCharacterBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Stats Component
	StatsComp = ObjectInitializer.CreateDefaultSubobject<UStatsComponentBase>(this, FName("Character Stats Component"));
	
	// Initialize variables
	CharacterState = ECharacterState::IdleWalkRun;
	bGodMode = false;
	TargetSwitchDuration = 0.1f;

}

void AEODCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEODCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEODCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AEODCharacterBase, CharacterState, COND_SkipOwner);

}

void AEODCharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

FORCEINLINE bool AEODCharacterBase::IsAlive() const
{
	return StatsComp->GetCurrentHealth() > 0;
}

FORCEINLINE bool AEODCharacterBase::IsDead() const
{
	return StatsComp->GetCurrentHealth() <= 0;
}

FORCEINLINE bool AEODCharacterBase::IsIdle() const
{
	return (CharacterState == ECharacterState::IdleWalkRun && GetVelocity().Size() == 0);
}

FORCEINLINE bool AEODCharacterBase::IsMoving() const
{
	return (CharacterState == ECharacterState::IdleWalkRun && GetVelocity().Size() != 0);
}

FORCEINLINE bool AEODCharacterBase::IsIdleOrMoving() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

FORCEINLINE bool AEODCharacterBase::IsJumping() const
{
	return CharacterState == ECharacterState::Jumping;
}

FORCEINLINE bool AEODCharacterBase::IsDodging() const
{
	return CharacterState == ECharacterState::Dodging;
}

FORCEINLINE bool AEODCharacterBase::IsDodgingDamage() const
{
	return bHasActiveiFrames;
}

FORCEINLINE bool AEODCharacterBase::IsBlocking() const
{
	return CharacterState == ECharacterState::Blocking;
}

FORCEINLINE bool AEODCharacterBase::IsBlockingDamage() const
{
	return bIsBlockingDamage;
}

FORCEINLINE bool AEODCharacterBase::IsCastingSpell() const
{
	return CharacterState == ECharacterState::CastingSpell;
}

FORCEINLINE bool AEODCharacterBase::IsNormalAttacking() const
{
	// return CharacterState == ECharacterState::Attacking && CurrentActiveSkill != nullptr;
	return CharacterState == ECharacterState::Attacking;
}

FORCEINLINE bool AEODCharacterBase::IsUsingAnySkill() const
{
	// return CharacterState == ECharacterState::UsingActiveSkill && CurrentActiveSkill != nullptr;
	return CharacterState == ECharacterState::UsingActiveSkill;
}

FORCEINLINE bool AEODCharacterBase::IsUsingSkill(int32 SkillIndex) const
{
	// return IsUsingAnySkill() && CurrentActiveSkill == GetSkill(SkillIndex);
	return true;
}

FORCEINLINE bool AEODCharacterBase::HasBeenHit() const
{
	return CharacterState == ECharacterState::GotHit;
}

FORCEINLINE bool AEODCharacterBase::IsCriticalHit(const FSkill* HitSkill) const
{
	bool bCriticalHit = false;

	if (HitSkill)
	{
		switch (HitSkill->DamageType)
		{
		case EDamageType::Physical:
			bCriticalHit = StatsComp->GetPhysicalCritRate() >= FMath::RandRange(0.f, 100.f) ? true : false;
			break;
		case EDamageType::Magickal:
			bCriticalHit = StatsComp->GetMagickCritRate() >= FMath::RandRange(0.f, 100.f) ? true : false;
			break;
		default:
			break;
		}
	}

	return bCriticalHit;
}

bool AEODCharacterBase::CanMove() const
{
	// Mobs can only move in IdleWalkRun state
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool AEODCharacterBase::CanJump() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool AEODCharacterBase::CanDodge() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool AEODCharacterBase::CanBlock() const
{
	return IsIdleOrMoving();
}

bool AEODCharacterBase::CanRespawn() const
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

bool AEODCharacterBase::CanUseSkill(int32 SkillIndex) const
{
	return false;
}

FORCEINLINE bool AEODCharacterBase::CanFlinch() const
{
	return !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::Flinch);
	// @todo - GetCurrentActiveSkill()->SkillLevelUpInfo.CrowdControlImmunities
}

FORCEINLINE bool AEODCharacterBase::CanStun() const
{
	return !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::Stunned);
}

FORCEINLINE bool AEODCharacterBase::CanKnockdown() const
{
	return !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::KnockedDown);
}

FORCEINLINE bool AEODCharacterBase::CanKnockback() const
{
	return !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::KnockedBack);
}

FORCEINLINE bool AEODCharacterBase::CanFreeze() const
{
	return !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::Crystalized);
}

FORCEINLINE bool AEODCharacterBase::CanInterrupt() const
{
	return !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::Interrupt);
}

FORCEINLINE bool AEODCharacterBase::NeedsHealing() const
{
	return StatsComp->IsLowOnHealth();
}

bool AEODCharacterBase::BP_NeedsHealing() const
{
	return NeedsHealing();
}

bool AEODCharacterBase::IsHealing() const
{
	return false;
}

void AEODCharacterBase::EnableiFrames(float Duration)
{
	bHasActiveiFrames = true;

	if (Duration > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, this, &AEODCharacterBase::DisableiFrames, Duration, false);
	}
}

void AEODCharacterBase::DisableiFrames()
{
	bHasActiveiFrames = false;
}

void AEODCharacterBase::EnableDamageBlocking()
{
	bIsBlockingDamage = true;
}

void AEODCharacterBase::DisableDamageBlocking()
{
	bIsBlockingDamage = false;
	// Clear block damage timer just in case it is still active
	GetWorld()->GetTimerManager().ClearTimer(BlockTimerHandle); 
}

FORCEINLINE void AEODCharacterBase::OnSuccessfulDodge(AEODCharacterBase* AttackInstigator)
{
	TWeakObjectPtr<AEODCharacterBase> AttackInstigatorWeakPtr(AttackInstigator);
	OnSuccessfulDodgeEvent.Broadcast(AttackInstigatorWeakPtr);
}

FORCEINLINE void AEODCharacterBase::OnSuccessfulBlock(AEODCharacterBase * AttackInstigator)
{
	TWeakObjectPtr<AEODCharacterBase> AttackInstigatorWeakPtr(AttackInstigator);
	OnSuccessfulBlockEvent.Broadcast(AttackInstigatorWeakPtr);
	BlockedAttack();
}

FORCEINLINE void AEODCharacterBase::OnAttackDeflected(AEODCharacterBase * AttackBlocker, bool bSkillIgnoresBlock)
{
	TWeakObjectPtr<AEODCharacterBase> AttackBlockerWeakPtr(AttackBlocker);
	OnAttackDeflectedEvent.Broadcast(AttackBlockerWeakPtr);

	if (!bSkillIgnoresBlock)
	{
		// PlayAnimationMontage()
	}
}

FORCEINLINE void AEODCharacterBase::SetOffTargetSwitch()
{
	TurnOnTargetSwitch();
}

void AEODCharacterBase::TurnOnTargetSwitch()
{
	GetMesh()->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 1.f);
	GetWorld()->GetTimerManager().SetTimer(TargetSwitchTimerHandle, this, &AEODCharacterBase::TurnOffTargetSwitch, TargetSwitchDuration, false);
}

void AEODCharacterBase::TurnOffTargetSwitch()
{
	GetMesh()->SetScalarParameterValueOnMaterials(FName("Target_Switch_On"), 0.f);
}

void AEODCharacterBase::Die(ECauseOfDeath CauseOfDeath, AEODCharacterBase * InstigatingChar)
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
		StatsComp->ModifyBaseHealth(-StatsComp->GetMaxHealth());
		SetCharacterState(ECharacterState::Dead);

		// @todo play death animation and death sound
	}
}

FORCEINLINE ECharacterState AEODCharacterBase::GetCharacterState() const
{
	return CharacterState;
}

void AEODCharacterBase::SetCharacterState(const ECharacterState NewState)
{
	CharacterState = NewState;

	if (Role < ROLE_Authority)
	{
		Server_SetCharacterState(NewState);
	}
}

void AEODCharacterBase::SetWalkSpeed(const float WalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	if (Role < ROLE_Authority)
	{
		Server_SetWalkSpeed(WalkSpeed);
	}
}

void AEODCharacterBase::SetCharacterRotation(const FRotator NewRotation)
{
	GetCharacterMovement()->FlushServerMoves();
	SetActorRotation(NewRotation);

	if (Role < ROLE_Authority)
	{
		Server_SetCharacterRotation(NewRotation);
	}
}

void AEODCharacterBase::SetUseControllerRotationYaw(const bool bNewBool)
{
	bUseControllerRotationYaw = bNewBool;

	if (Role < ROLE_Authority)
	{
		Server_SetUseControllerRotationYaw(bNewBool);
	}
}

FORCEINLINE EFaction AEODCharacterBase::GetFaction() const
{
	return Faction;
}

/*
FORCEINLINE FSkill * AEODCharacterBase::GetSkill(FName SkillID) const
{
	if (IDToSkillMap.Contains(SkillID))
	{
		return IDToSkillMap[SkillID];
	}

	return nullptr;
}
*/

/*
FSkill * AEODCharacterBase::GetSkill(int32 SkillIndex) const
{
	if (Skills.Num() > SkillIndex)
	{
		return Skills[SkillIndex];
	}

	return nullptr;
}
*/

/*
bool AEODCharacterBase::UseSkill(int32 SkillIndex)
{
	if (CanUseAnySkill())
	{
		FSkill* SkillToUse = GetSkill(SkillIndex);

		if (!SkillToUse)
		{
			// unable to use skill - return false
			return false;
		}

		PlayAnimationMontage(SkillToUse->AnimationMontage_GenderOne, SkillToUse->SkillStartMontageSectionName, ECharacterState::UsingActiveSkill);
		CurrentActiveSkill = SkillToUse;
		return true;
	}

	return false;
}
*/

bool AEODCharacterBase::UseSkill(FName SkillID)
{
	return false;
}

void AEODCharacterBase::StartSkill(FName SkillID)
{
}

void AEODCharacterBase::StopSkill(FName SkillID)
{
}

EEODTaskStatus AEODCharacterBase::CheckSkillStatus(FName SkillID)
{
	return EEODTaskStatus();
}

/*
EEODTaskStatus AEODCharacterBase::CheckSkillStatus(int32 SkillIndex)
{
	EEODTaskStatus TaskStatus = EEODTaskStatus::Inactive;

	FSkill* SkillToCheck = GetSkill(SkillIndex);
	if (SkillToCheck == CurrentActiveSkill)
	{
		return EEODTaskStatus::Active;
	}

	if (GetLastUsedSkill().LastUsedSkill != CurrentActiveSkill)
	{
		return EEODTaskStatus::Inactive;
	}

	if (GetLastUsedSkill().bInterrupted)
	{
		return EEODTaskStatus::Aborted;
	}
	else
	{
		return EEODTaskStatus::Finished;
	}
}
*/

/*
int32 AEODCharacterBase::GetMostWeightedSkillIndex() const
{
	// @todo definition
	return 0;
}
*/

FName AEODCharacterBase::GetMostWeightedMeleeSkillID(AEODCharacterBase const * const TargetCharacter) const
{
	return FName();
}

FORCEINLINE FName AEODCharacterBase::GetCurrentActiveSkillID() const
{
	return CurrentActiveSkillID;
}

FORCEINLINE void AEODCharacterBase::SetCurrentActiveSkillID(const FName SkillID)
{
	CurrentActiveSkillID = SkillID;
}

FName AEODCharacterBase::BP_GetCurrentActiveSkillID() const
{
	return GetCurrentActiveSkillID();
}

FORCEINLINE FLastUsedSkillInfo & AEODCharacterBase::GetLastUsedSkill()
{
	return LastUsedSkillInfo;
}

FLastUsedSkillInfo& AEODCharacterBase::BP_GetLastUsedSkill()
{
	return GetLastUsedSkill();
}

void AEODCharacterBase::ApplyStatusEffect(const UStatusEffectBase * StatusEffect)
{
	// @todo definition
}

void AEODCharacterBase::RemoveStatusEffect(const UStatusEffectBase * StatusEffect)
{
	// @todo definition
}

/*
void AEODCharacterBase::OnMeleeCollision(UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit)
{
	FSkill* ActiveSkill = GetCurrentActiveSkill();
	check(ActiveSkill); // Make sure ActiveSkil is not a nullptr

	FEODDamage EODDamage;
	EODDamage.bUnblockable = ActiveSkill->SkillLevelUpInfo.bUnblockable;
	EODDamage.bUndodgable = ActiveSkill->SkillLevelUpInfo.bUndodgable;
	EODDamage.DamageType = ActiveSkill->DamageType;
	EODDamage.CrowdControlEffect = ActiveSkill->SkillLevelUpInfo.CrowdControlEffect;
	EODDamage.CrowdControlEffectDuration = ActiveSkill->SkillLevelUpInfo.CrowdControlEffectDuration;

	if (ActiveSkill->DamageType == EDamageType::Magickal)
	{
		EODDamage.NormalDamage = (ActiveSkill->SkillLevelUpInfo.DamagePercent * StatsComp->GetMagickAttack()) / 100;
		EODDamage.CritDamage = EODDamage.NormalDamage * UCombatLibrary::MagickalCritMultiplier + StatsComp->GetMagickCritBonus();
		EODDamage.CritRate = StatsComp->GetMagickCritRate();
	}
	else
	{
		EODDamage.NormalDamage = (ActiveSkill->SkillLevelUpInfo.DamagePercent * StatsComp->GetPhysicalAttack()) / 100;
		EODDamage.CritDamage = EODDamage.NormalDamage * UCombatLibrary::PhysicalCritMultiplier + StatsComp->GetPhysicalCritBonus();
		EODDamage.CritRate = StatsComp->GetPhysicalCritRate();
	}

	TArray<TWeakObjectPtr<AEODCharacterBase>> CharactersSuccessfullyHit;
	TArray<TWeakObjectPtr<AEODCharacterBase>> CharactersHitWithCriticalDamage;

	for (FHitResult& HitResult : HitResults)
	{
		// @todo handle damage for non AEODCharacterBase actors
		AEODCharacterBase* HitCharacter = Cast<AEODCharacterBase>(HitResult.Actor.Get());
		if (!HitCharacter)
		{
			continue;
		}

		FEODDamageResult EODDamageResult = HitCharacter->ApplyEODDamage(this, EODDamage, HitResult);

		if (EODDamageResult.CharacterResponseToDamage == ECharacterResponseToDamage::Damaged ||
			EODDamageResult.CharacterResponseToDamage == ECharacterResponseToDamage::Blocked)
		{
			TWeakObjectPtr<AEODCharacterBase> HitCharacterWeakPtr(HitCharacter);
			CharactersSuccessfullyHit.Add(HitCharacterWeakPtr);
			if (EODDamageResult.bCritHit)
			{
				CharactersHitWithCriticalDamage.Add(HitCharacterWeakPtr);
			}
		}
	}

	if (CharactersSuccessfullyHit.Num() == 0)
	{
		OnUnsuccessfulHit.Broadcast(TArray<TWeakObjectPtr<AEODCharacterBase>>());
	}
	else
	{
		OnSuccessfulHit.Broadcast(CharactersSuccessfullyHit);

		switch (ActiveSkill->DamageType)
		{
		case EDamageType::Physical:
			OnSuccessfulPhysicalAttack.Broadcast(CharactersSuccessfullyHit);
			break;
		case EDamageType::Magickal:
			OnSuccessfulMagickAttack.Broadcast(CharactersSuccessfullyHit);
			break;
		default:
			break;
		}

		if (CharactersHitWithCriticalDamage.Num() > 0)
		{
			OnCriticalHit.Broadcast(CharactersHitWithCriticalDamage);
		}
	}
}

FEODDamageResult AEODCharacterBase::ApplyEODDamage(AEODCharacterBase * InstigatingChar, const FEODDamage & EODDamage, const FHitResult & CollisionHitResult)
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

	return EODDamageResult;
}
*/

void AEODCharacterBase::OnMontageBlendingOut(UAnimMontage * AnimMontage, bool bInterrupted)
{
}

void AEODCharacterBase::OnMontageEnded(UAnimMontage * AnimMontage, bool bInterrupted)
{
}

FORCEINLINE void AEODCharacterBase::PlayAnimationMontage(UAnimMontage * MontageToPlay, FName SectionToPlay)
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay, MontageToPlay);
	}
}

FORCEINLINE void AEODCharacterBase::PlayAnimationMontage(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay, MontageToPlay);
		CharacterState = NewState;
	}

	Server_PlayAnimationMontage(MontageToPlay, SectionToPlay, NewState);
}

void AEODCharacterBase::BP_PlayAnimationMontage(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	PlayAnimationMontage(MontageToPlay, SectionToPlay, NewState);
}

void AEODCharacterBase::SetNextMontageSection(FName CurrentSection, FName NextSection)
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_SetNextSection(CurrentSection, NextSection);
	}

	Server_SetNextMontageSection(CurrentSection, NextSection);
}

bool AEODCharacterBase::DeltaRotateCharacterToDesiredYaw(float DesiredYaw, float DeltaTime, float Precision, float RotationRate)
{
	float CurrentYaw = GetActorRotation().Yaw;
	float YawDiff = FMath::FindDeltaAngleDegrees(CurrentYaw, DesiredYaw);
	if (FMath::Abs(YawDiff) < Precision)
	{
		return true;
	}

	float Multiplier = YawDiff / FMath::Abs(YawDiff);
	float RotateBy = Multiplier * RotationRate * DeltaTime;
	if (FMath::Abs(YawDiff) <= FMath::Abs(RotateBy))
	{
		SetCharacterRotation(FRotator(0.f, DesiredYaw, 0.f));
		return true;
	}
	else if (FMath::Abs(YawDiff) <= FMath::Abs(RotateBy) + Precision)
	{
		SetCharacterRotation(FRotator(0.f, CurrentYaw + RotateBy, 0.f));
		return true;
	}
	else
	{
		SetCharacterRotation(FRotator(0.f, CurrentYaw + RotateBy, 0.f));
		return false;
	}
}

void AEODCharacterBase::OnRep_CharacterState(ECharacterState OldState)
{
	//~ @todo : Cleanup old state
}

void AEODCharacterBase::Server_SetCharacterState_Implementation(ECharacterState NewState)
{
	SetCharacterState(NewState);
}

bool AEODCharacterBase::Server_SetCharacterState_Validate(ECharacterState NewState)
{
	return true;
}

void AEODCharacterBase::Server_SetNextMontageSection_Implementation(FName CurrentSection, FName NextSection)
{

	Multicast_SetNextMontageSection(CurrentSection, NextSection);
}

bool AEODCharacterBase::Server_SetNextMontageSection_Validate(FName CurrentSection, FName NextSection)
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
	SetCharacterRotation(NewRotation);
}

bool AEODCharacterBase::Server_SetCharacterRotation_Validate(FRotator NewRotation)
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

void AEODCharacterBase::Multicast_SetNextMontageSection_Implementation(FName CurrentSection, FName NextSection)
{
	FString Message = FString("Multi cast called");
	UKismetSystemLibrary::PrintString(this, Message, true, false);

	if (!IsLocallyControlled() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_SetNextSection(CurrentSection, NextSection);
	}
}

void AEODCharacterBase::Server_PlayAnimationMontage_Implementation(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	MultiCast_PlayAnimationMontage(MontageToPlay, SectionToPlay, NewState);
}

bool AEODCharacterBase::Server_PlayAnimationMontage_Validate(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	return true;
}

void AEODCharacterBase::MultiCast_PlayAnimationMontage_Implementation(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	if (!IsLocallyControlled() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay);
		CharacterState = NewState;
	}
}
