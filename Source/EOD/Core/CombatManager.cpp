// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CombatManager.h"
#include "EODPreprocessors.h"
#include "Player/EODCharacterBase.h"
#include "Player/PlayerCharacter.h"
#include "Player/Components/StatsComponentBase.h"

#include "Engine/World.h"

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

void ACombatManager::OnMeleeAttack(AActor * HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults)
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

FORCEINLINE float ACombatManager::CalculateAngleBetweenVectors(FVector Vec1, FVector Vec2)
{
	FVector NormalizedVec1 = Vec1.GetSafeNormal();
	FVector NormalizedVec2 = Vec2.GetSafeNormal();
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(NormalizedVec1, NormalizedVec2)));

	return Angle;
}

FORCEINLINE void ACombatManager::NativeDisplayDamage(const AEODCharacterBase* HitInstigator,
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

FORCEINLINE bool ACombatManager::WasBlockSuccessful(const AActor* HitInstigator,
													const AActor* HitActor,
													const bool bLineHitResultFound,
													const FHitResult& LineHitResult)
{
	FVector HitActorForwardVector = HitActor->GetActorForwardVector();
	FVector HitNormal = LineHitResult.ImpactNormal;
	float Angle = CalculateAngleBetweenVectors(HitActorForwardVector, HitNormal);
	bool bResult = Angle < BlockDetectionAngle ? true : false;
	return bResult;
}

FORCEINLINE bool ACombatManager::GetCritChanceBoolean(const AEODCharacterBase* HitInstigator,
													  const AEODCharacterBase* HitCharacter,
													  const EDamageType& DamageType) const
{
	float CritRate = DamageType == EDamageType::Physical ? HitInstigator->StatsComp->GetPhysicalCritRate() : HitInstigator->StatsComp->GetMagickCritRate();
	bool bResult = CritRate >= FMath::RandRange(0.f, 100.f) ? true : false;
	return bResult;
}

FORCEINLINE float ACombatManager::GetBCAngle(AEODCharacterBase* HitCharacter, const FHitResult& LineHitResult)
{
	return CalculateAngleBetweenVectors(HitCharacter->GetActorForwardVector(), LineHitResult.ImpactNormal);
}

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
		ActualDamage = HitInstigator->StatsComp->GetPhysicalAttack();
		CritBonus = HitInstigator->StatsComp->GetPhysicalCritBonus();
		DamageReductionOnBlock = HitInstigator->StatsComp->GetPhysicalDamageReductionOnBlock();
		CritMultiplier = PhysicalCritMultiplier;
	}
	else if (SkillDamageInfo.DamageType == EDamageType::Magickal)
	{
		ActualDamage = HitInstigator->StatsComp->GetMagickAttack();
		CritBonus = HitInstigator->StatsComp->GetMagickCritBonus();
		DamageReductionOnBlock = HitInstigator->StatsComp->GetMagickDamageReductionOnBlock();
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

bool ACombatManager::GetLineHitResult(const AActor* HitInstigator, const AActor* HitTarget, FHitResult& OutHitResult) const
{
	TArray<FHitResult> LineHitResults;
	FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(HitInstigator);
	FVector LineStart = HitInstigator->GetActorLocation();
	float LineEndZ = 0.f;
	if (HitTarget->GetActorLocation().Z < LineEndZ)
	{
		LineEndZ = HitTarget->GetActorLocation().Z;
	}
	else
	{
		LineEndZ = LineStart.Z;
	}
	FVector LineEnd = FVector(HitTarget->GetActorLocation().X, HitTarget->GetActorLocation().Y, LineEndZ);
	GetWorld()->LineTraceMultiByChannel(LineHitResults, LineStart, LineEnd, COLLISION_COMBAT, Params);

	bool bResult = false;
	for (FHitResult& LineHitResult : LineHitResults)
	{
		if (LineHitResult.Actor.Get() == HitTarget)
		{
			OutHitResult = LineHitResult;
			bResult = true;
			break;
		}
	}

	return bResult;
}

void ACombatManager::ProcessActorAttack(AActor* HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
}

void ACombatManager::ProcessCharacterAttack(AEODCharacterBase* HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults)
{
	const FSkillDamageInfo SkillDamageInfo = HitInstigator->GetCurrentActiveSkillDamageInfo();

	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.Actor.Get();
		// Do not process if hit actor is not valid
		if (!IsValid(HitActor))
		{
			continue;
		}

		AEODCharacterBase* HitCharacter = Cast<AEODCharacterBase>(HitActor);
		if (HitCharacter)
		{
			CharacterToCharacterAttack(HitInstigator, HitCharacter, SkillDamageInfo, HitResult);
		}
		else
		{
			CharacterToActorAttack(HitInstigator, HitActor, SkillDamageInfo, HitResult);
		}
	}
}

void ACombatManager::CharacterToCharacterAttack(AEODCharacterBase* HitInstigator,
												AEODCharacterBase* HitCharacter,
												const FSkillDamageInfo& SkillDamageInfo,
												const FHitResult& HitResult)
{
	check(HitInstigator);
	check(HitCharacter);

	if (!SkillDamageInfo.bUndodgable && HitCharacter->IsDodgingDamage())
	{
		// DisplayStatusEffectMessage(FString("Dodge"));
		HitCharacter->OnSuccessfulDodge(HitInstigator);
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
			HitCharacter->OnSuccessfulBlock(HitInstigator);
			HitInstigator->OnAttackDeflected(HitCharacter, SkillDamageInfo.bIgnoresBlock);
		}
	}

	float ActualDamage = GetActualDamage(HitInstigator, HitCharacter, SkillDamageInfo, bCritHit, bAttackBlocked);
	NativeDisplayDamage(HitInstigator, HitCharacter, LineHitResult, ActualDamage, bCritHit);

	// @todo make camera shake interesting
	PlayCameraShake(ECameraShakeType::Medium, LineHitResult.ImpactPoint);

	if (bAttackBlocked)
	{
		return;
	}

	HitCharacter->SetOffTargetSwitch();

	bool bCCEApplied = ApplyCrowdControlEffects(HitInstigator, HitCharacter, SkillDamageInfo, LineHitResult, BCAngle);
	// Rest of the function definition is inside BP_CharacterToCharacterAttack for now
	BP_CharacterToCharacterAttack(HitInstigator, HitCharacter, SkillDamageInfo, HitResult, LineHitResult);
}

void ACombatManager::CharacterToActorAttack(AEODCharacterBase* HitInstigator, AActor* HitActor,
											const FSkillDamageInfo& SkillDamageInfo,
											const FHitResult& HitResult)
{
}

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
		bCCEApplied = HitCharacter->Flinch(BCAngle);
		break;
	case ECrowdControlEffect::Interrupt:
		bCCEApplied = HitCharacter->Interrupt(BCAngle);
		break;
	case ECrowdControlEffect::KnockedDown:
		bCCEApplied = HitCharacter->Knockdown(SkillDamageInfo.CrowdControlEffectDuration);
		break;
	case ECrowdControlEffect::KnockedBack:
		bCCEApplied = HitCharacter->Knockback(SkillDamageInfo.CrowdControlEffectDuration, -(LineHitResult.ImpactNormal));
		break;
	case ECrowdControlEffect::Stunned:
		bCCEApplied = HitCharacter->Stun(SkillDamageInfo.CrowdControlEffectDuration);
		break;
	case ECrowdControlEffect::Crystalized:
		bCCEApplied = HitCharacter->Freeze(SkillDamageInfo.CrowdControlEffectDuration);
		break;
	default:
		break;
	}

	return bCCEApplied;
}
