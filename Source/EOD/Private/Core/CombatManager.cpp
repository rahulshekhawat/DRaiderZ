// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CombatManager.h"
#include "EODPreprocessors.h"
#include "PlayerCharacter.h"
#include "AttackDodgedEvent.h"
#include "StatsComponentBase.h"
#include "GameplaySkillsComponent.h"

#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

ACombatManager::ACombatManager(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	BlockDetectionAngle = 60.f;
	PhysicalCritMultiplier = 1.6f;
	MagickalCritMultiplier = 1.4f;
}

void ACombatManager::BeginPlay()
{
	Super::BeginPlay();

}

void ACombatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatManager::OnMeleeAttack(AActor* HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
	if (!IsValid(HitInstigator) || HitResults.Num() == 0)
	{
		return;
	}
	
	AEODCharacterBase* InstigatingCharacter = Cast<AEODCharacterBase>(HitInstigator);
	if (InstigatingCharacter)
	{
		ProcessCharacterAttack(InstigatingCharacter, bHit, HitResults);
	}
	else
	{
		ProcessActorAttack(HitInstigator, bHit, HitResults);
	}
}

void ACombatManager::NativeDisplayDamage(
	const AEODCharacterBase* HitInstigator,
	const AEODCharacterBase* HitCharacter,
	const FHitResult& LineHitResult,
	const float ActualDamage,
	const bool bCriticalHit)
{
	// Do no display anything if none of the involved characters is a player character
	if (!HitInstigator->IsA(APlayerCharacter::StaticClass()) && !HitCharacter->IsA(APlayerCharacter::StaticClass()))
	{
		return;
	}

	FString DisplayMessage = FString::FromInt(ActualDamage);
	// If the damaged character was player (@todo change it to check for locally controlled player)
	if (HitCharacter->IsA(APlayerCharacter::StaticClass()))
	{
		DisplayDamageMessage(DisplayMessage, PlayerDamagedTextColor, LineHitResult.ImpactPoint);
	}
	else
	{
		if (bCriticalHit)
		{
			DisplayDamageMessage(DisplayMessage, NPCCritDamagedTextColor, LineHitResult.ImpactPoint);
		}
		else
		{
			DisplayDamageMessage(DisplayMessage, NPCNormalDamagedTextColor, LineHitResult.ImpactPoint);
		}
	}
}

/*
float ACombatManager::GetActualDamage(const AEODCharacterBase* HitInstigator,
									  const AEODCharacterBase* HitCharacter,
									  const FSkillDamageInfo& SkillDamageInfo,
									  const bool bCriticalHit,
									  const bool bAttackBlocked)
{
	float ActualDamage = 0.f;
	float CritMultiplier = 1.f;
	float CritBonus = 0.f;
	float DamageReductionOnBlock = 0.f;

	if (SkillDamageInfo.DamageType == EDamageType::Physical)
	{
		ActualDamage = HitInstigator->GetCharacterStatsComponent()->GetPhysicalAttack();
		CritBonus = HitInstigator->GetCharacterStatsComponent()->GetPhysicalCritBonus();
		DamageReductionOnBlock = HitInstigator->GetCharacterStatsComponent()->GetPhysicalDamageReductionOnBlock();
		CritMultiplier = PhysicalCritMultiplier;
	}
	else if (SkillDamageInfo.DamageType == EDamageType::Magickal)
	{
		ActualDamage = HitInstigator->GetCharacterStatsComponent()->GetMagickAttack();
		CritBonus = HitInstigator->GetCharacterStatsComponent()->GetMagickCritBonus();
		DamageReductionOnBlock = HitInstigator->GetCharacterStatsComponent()->GetMagickDamageReductionOnBlock();
		CritMultiplier = MagickalCritMultiplier;
	}

	ActualDamage = ActualDamage * SkillDamageInfo.DamagePercent / 100.f;

	if (bCriticalHit)
	{
		ActualDamage = ActualDamage * CritMultiplier + CritBonus;
	}

	if (bAttackBlocked)
	{
		ActualDamage = ActualDamage * (1 - DamageReductionOnBlock);
	}
	return ActualDamage;
}
*/

float ACombatManager::GetActualDamage(
	const AEODCharacterBase* HitInstigator,
	const AEODCharacterBase* HitCharacter,
	const FSkillTableRow* SkillUsed,
	const bool bCriticalHit,
	const bool bAttackBlocked)
{
	check(HitInstigator || HitCharacter || SkillUsed)

	float ActualDamage = 0.f;
	float CritMultiplier = 1.f;
	float CritBonus = 0.f;
	float DamageReductionOnBlock = 0.f;

	if (SkillUsed->DamageType == EDamageType::Physical)
	{
		ActualDamage = HitInstigator->GetCharacterStatsComponent()->GetPhysicalAttack();
		CritBonus = HitInstigator->GetCharacterStatsComponent()->GetPhysicalCritBonus();
		DamageReductionOnBlock = HitInstigator->GetCharacterStatsComponent()->GetPhysicalDamageReductionOnBlock();
		CritMultiplier = PhysicalCritMultiplier;
	}
	else if (SkillUsed->DamageType == EDamageType::Magickal)
	{
		ActualDamage = HitInstigator->GetCharacterStatsComponent()->GetMagickAttack();
		CritBonus = HitInstigator->GetCharacterStatsComponent()->GetMagickCritBonus();
		DamageReductionOnBlock = HitInstigator->GetCharacterStatsComponent()->GetMagickDamageReductionOnBlock();
		CritMultiplier = MagickalCritMultiplier;
	}

	ActualDamage = ActualDamage * SkillUsed->DamagePercent / 100.f;

	if (bCriticalHit)
	{
		ActualDamage = ActualDamage * CritMultiplier + CritBonus;
	}

	if (bAttackBlocked)
	{
		ActualDamage = ActualDamage * (1 - DamageReductionOnBlock);
	}

	return ActualDamage;
}

void ACombatManager::GetLineHitResult(const AActor* HitInstigator, const AActor* HitTarget, FHitResult& OutHitResult, bool& bOutLineHitResultFound) const
{
	bOutLineHitResultFound = false;
	if (!IsValid(HitInstigator) || !IsValid(HitTarget))
	{
		return;
	}

	FCollisionQueryParams QueryParams = UCombatLibrary::GenerateCombatCollisionQueryParams(HitInstigator);
	FVector LineStart = HitInstigator->GetActorLocation();
	FVector LineEnd = HitTarget->GetActorLocation();
	LineEnd.Z = LineStart.Z < LineEnd.Z ? LineStart.Z : LineEnd.Z;

	TArray<FHitResult> LineHitResults;
	GetWorld()->LineTraceMultiByChannel(LineHitResults, LineStart, LineEnd, COLLISION_COMBAT, QueryParams);
	for (FHitResult& LineHitResult : LineHitResults)
	{
		if (LineHitResult.GetActor() == HitTarget)
		{
			OutHitResult = LineHitResult;
			bOutLineHitResultFound = true;
			break;
		}
	}
}

void ACombatManager::GetLineHitResult(const AActor* HitInstigator, const UPrimitiveComponent* HitComponent, FHitResult& OutHitResult, bool& bOutLineHitResultFound) const
{
	bOutLineHitResultFound = false;
	if (!IsValid(HitInstigator) || !IsValid(HitComponent))
	{
		return;
	}

	FCollisionQueryParams QueryParams = UCombatLibrary::GenerateCombatCollisionQueryParams(HitInstigator);
	FVector LineStart = HitInstigator->GetActorLocation();
	FVector LineEnd = HitComponent->GetComponentLocation();
	LineEnd.Z = LineStart.Z < LineEnd.Z ? LineStart.Z : LineEnd.Z;

	TArray<FHitResult> LineHitResults;
	GetWorld()->LineTraceMultiByChannel(LineHitResults, LineStart, LineEnd, COLLISION_COMBAT, QueryParams);
	for (FHitResult& LineHitResult : LineHitResults)
	{
		if (LineHitResult.GetComponent() == HitComponent)
		{
			OutHitResult = LineHitResult;
			bOutLineHitResultFound = true;
			break;
		}
	}
}

void ACombatManager::ProcessActorAttack(AActor* HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults)
{

}

void ACombatManager::ProcessCharacterAttack(AEODCharacterBase* HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
	FSkillTableRow* HitSkill = nullptr;
	if (IsValid(HitInstigator) && IsValid(HitInstigator->GetGameplaySkillsComponent()))
	{
		HitSkill = HitInstigator->GetGameplaySkillsComponent()->GetCurrentActiveSkill();
	}

	// Do no process if hit with an invalid skill
	if (!HitSkill)
	{
		return;
	}

	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		// Do not process if hit actor is not valid
		if (!IsValid(HitActor))
		{
			continue;
		}

		bool bHitActorWasDamaged;
		float ActualDamageToHitActor;
		AEODCharacterBase* HitCharacter = Cast<AEODCharacterBase>(HitActor);
		if (HitCharacter && HitCharacter->IsAlive())
		{
			CharacterToCharacterAttack(HitInstigator, HitCharacter, HitSkill, HitResult, bHitActorWasDamaged, ActualDamageToHitActor);
		}
		else
		{
			CharacterToActorAttack(HitInstigator, HitActor, HitSkill, HitResult, bHitActorWasDamaged, ActualDamageToHitActor);
		}
	}
}

void ACombatManager::CharacterToCharacterAttack(
	AEODCharacterBase* HitInstigator,
	AEODCharacterBase* HitCharacter,
	const FSkillTableRow* SkillUsed,
	const FHitResult& HitResult,
	bool& bOutHitCharacterReceivedDamage,
	float& OutDamageInflicted)
{
	check(HitInstigator && HitCharacter && SkillUsed);

	bOutHitCharacterReceivedDamage = false;
	OutDamageInflicted = 0.f;
	if (!AreEnemies(HitInstigator, HitCharacter))
	{
		return;
	}

	if (!SkillUsed->bUndodgable && HitCharacter->IsDodgingDamage())
	{
		HitCharacter->DisplayTextOnPlayerScreen(FString("Dodge"), DodgeTextColor, HitResult.ImpactPoint);
		HitInstigator->DisplayTextOnPlayerScreen(FString("Dodge"), DodgeTextColor, HitResult.ImpactPoint);
		UAttackDodgedEvent* DodgeEvent = NewObject<UAttackDodgedEvent>();
		DodgeEvent->AddToRoot();
		HitCharacter->OnDodgingAttack.Broadcast(HitCharacter, HitInstigator, HitCharacter, DodgeEvent);
		DodgeEvent->RemoveFromRoot();
		DodgeEvent->MarkPendingKill();
		return;
	}

	FHitResult LineHitResult;
	bool bLineHitResultFound;
	GetLineHitResult(HitInstigator, HitResult.GetComponent(), LineHitResult, bLineHitResultFound);

	// Cause a crash if no line hit result was found
	check(bLineHitResultFound);

	bool bCritHit = GetCritChanceBoolean(HitInstigator, HitCharacter, SkillUsed->DamageType);
	float BCAngle = GetBCAngle(HitCharacter, LineHitResult);

	bool bAttackBlocked = false;
	if (!SkillUsed->bUnblockable && HitCharacter->IsBlockingDamage())
	{
		bAttackBlocked = BCAngle < BlockDetectionAngle ? true : false;
		if (bAttackBlocked)
		{
			// HitCharacter->OnBlockingAttack.Broadcast()
			// HitInstigator->OnDeflectingAttack.Broadcast()
			// @fix
			// HitCharacter->OnSuccessfulBlock(HitInstigator);
			// HitInstigator->OnAttackDeflected(HitCharacter, SkillDamageInfo.bIgnoresBlock);
		}		
	}

	float ActualDamage = GetActualDamage(HitInstigator, HitCharacter, SkillUsed, bCritHit, bAttackBlocked);
	HitCharacter->GetCharacterStatsComponent()->ModifyCurrentHealth(-ActualDamage);
	int32 ResultingHitCharacterHP = HitCharacter->GetCharacterStatsComponent()->GetCurrentHealth();


	/*
	bool bCCEApplied = ApplyCrowdControlEffects(HitInstigator, HitCharacter, SkillUsed, LineHitResult, BCAngle);
	if (ResultingHitCharacterHP <= 0)
	{
		HitCharacter->InitiateDeathSequence();
	}
	NativeDisplayDamage(HitInstigator, HitCharacter, LineHitResult, ActualDamage, bCritHit);

	// @todo make camera shake interesting
	PlayCameraShake(ECameraShakeType::Medium, LineHitResult.ImpactPoint);
	SpawnHitSFX(LineHitResult.ImpactPoint, LineHitResult.ImpactNormal);
	PlayHitSound(HitInstigator, LineHitResult.ImpactPoint, bCritHit);

	if (bAttackBlocked)
	{
		return;
	}

	HitCharacter->SetOffTargetSwitch();
	*/
}

void ACombatManager::CharacterToActorAttack(
	AEODCharacterBase* HitInstigator,
	AActor* HitActor,
	const FSkillTableRow* SkillUsed,
	const FHitResult& HitResult,
	bool& bOutHitActorReceivedDamage,
	float& OutDamageInflicted)
{
	check(HitInstigator && HitActor && SkillUsed);

	bOutHitActorReceivedDamage = false;
	OutDamageInflicted = 0.f;
}

/*
void ACombatManager::CharacterToCharacterAttack(AEODCharacterBase* HitInstigator,
												AEODCharacterBase* HitCharacter,
												const FSkillDamageInfo& SkillDamageInfo,
												const FHitResult& HitResult)
{
	check(HitInstigator);
	check(HitCharacter);

	if (!AreEnemies(HitInstigator, HitCharacter))
	{
		return;
	}

	if (!SkillDamageInfo.bUndodgable && HitCharacter->IsDodgingDamage())
	{
		DisplayStatusEffectMessage(FString("Dodge"), DodgeTextColor, HitResult.ImpactPoint);
		UAttackDodgedEvent* DodgeEvent = NewObject<UAttackDodgedEvent>();
		DodgeEvent->AddToRoot();
		HitCharacter->OnDodgingAttack.Broadcast(HitCharacter, HitInstigator, HitCharacter, DodgeEvent);
		DodgeEvent->RemoveFromRoot();
		DodgeEvent->MarkPendingKill();
		return;
	}

	FHitResult LineHitResult;
	bool bLineHitResultFound = GetLineHitResult(HitInstigator, HitCharacter, LineHitResult);
	bool bCritHit = GetCritChanceBoolean(HitInstigator, HitCharacter, SkillDamageInfo.DamageType);
	float BCAngle = GetBCAngle(HitCharacter, LineHitResult);

	bool bAttackBlocked = false;
	if (!SkillDamageInfo.bUnblockable && HitCharacter->IsBlockingDamage())
	{
		bAttackBlocked = BCAngle < BlockDetectionAngle ? true : false;

		if (bAttackBlocked)
		{
			// @fix
			// HitCharacter->OnSuccessfulBlock(HitInstigator);
			// HitInstigator->OnAttackDeflected(HitCharacter, SkillDamageInfo.bIgnoresBlock);
		}
	}

	float ActualDamage = GetActualDamage(HitInstigator, HitCharacter, SkillDamageInfo, bCritHit, bAttackBlocked);
	HitCharacter->GetCharacterStatsComponent()->ModifyCurrentHealth(-ActualDamage);
	int32 ResultingHitCharacterHP = HitCharacter->GetCharacterStatsComponent()->GetCurrentHealth();
	bool bCCEApplied = ApplyCrowdControlEffects(HitInstigator, HitCharacter, SkillDamageInfo, LineHitResult, BCAngle);
	if (ResultingHitCharacterHP <= 0)
	{
		HitCharacter->InitiateDeathSequence();
	}
	NativeDisplayDamage(HitInstigator, HitCharacter, LineHitResult, ActualDamage, bCritHit);

	// @todo make camera shake interesting
	PlayCameraShake(ECameraShakeType::Medium, LineHitResult.ImpactPoint);
	SpawnHitSFX(LineHitResult.ImpactPoint, LineHitResult.ImpactNormal);
	PlayHitSound(HitInstigator, LineHitResult.ImpactPoint, bCritHit);

	if (bAttackBlocked)
	{
		return;
	}

	HitCharacter->SetOffTargetSwitch();

	// Rest of the function definition is inside BP_CharacterToCharacterAttack for now
	BP_CharacterToCharacterAttack(HitInstigator, HitCharacter, SkillDamageInfo, HitResult, LineHitResult);
}

void ACombatManager::CharacterToActorAttack(AEODCharacterBase* HitInstigator, AActor* HitActor,
											const FSkillDamageInfo& SkillDamageInfo,
											const FHitResult& HitResult)
{
}
*/

/*
bool ACombatManager::ApplyCrowdControlEffects(AEODCharacterBase* HitInstigator,
											  AEODCharacterBase* HitCharacter,
											  const FSkillDamageInfo& SkillDamageInfo,
											  const FHitResult& LineHitResult,
											  const float BCAngle)
{
	bool bCCEApplied = false;
	switch (SkillDamageInfo.CrowdControlEffect)
	{
	case ECrowdControlEffect::Flinch:
		// bCCEApplied = HitCharacter->Flinch(BCAngle);
		bCCEApplied = HitCharacter->CCEFlinch(BCAngle);
		break;
	case ECrowdControlEffect::Interrupt:
		bCCEApplied = HitCharacter->CCEInterrupt(BCAngle);
		break;
	case ECrowdControlEffect::KnockedDown:
		HitCharacter->SetActorRotation(FRotationMatrix::MakeFromX(HitInstigator->GetActorForwardVector() * -1).Rotator());
		// bCCEApplied = HitCharacter->CCEKnockdown(SkillDamageInfo.CrowdControlEffectDuration);
		bCCEApplied = HitCharacter->CCEKnockback(SkillDamageInfo.CrowdControlEffectDuration, 0.2 * HitInstigator->GetActorForwardVector());
		break;
	case ECrowdControlEffect::KnockedBack:
		HitCharacter->SetActorRotation(FRotationMatrix::MakeFromX(HitInstigator->GetActorForwardVector() * -1).Rotator());
		// bCCEApplied = HitCharacter->CCEKnockback(SkillDamageInfo.CrowdControlEffectDuration, -(LineHitResult.ImpactNormal));
		bCCEApplied = HitCharacter->CCEKnockback(SkillDamageInfo.CrowdControlEffectDuration, HitInstigator->GetActorForwardVector());
		break;
	case ECrowdControlEffect::Stunned:
		bCCEApplied = HitCharacter->CCEStun(SkillDamageInfo.CrowdControlEffectDuration);
		break;
	case ECrowdControlEffect::Crystalized:
		bCCEApplied = HitCharacter->CCEFreeze(SkillDamageInfo.CrowdControlEffectDuration);
		break;
	default:
		break;
	}

	return bCCEApplied;
}
*/

bool ACombatManager::AreEnemies(AEODCharacterBase* CharOne, AEODCharacterBase* CharTwo)
{
	// return CharOne->GetFaction() != CharTwo->GetFaction();
	// @todo Need a better system to determine if two characters are enemies
	return true;
}
