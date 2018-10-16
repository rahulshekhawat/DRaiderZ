// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CombatManager.h"
#include "EODPreprocessors.h"
#include "Player/EODCharacterBase.h"
#include "Player/PlayerCharacter.h"
#include "Player/Components/StatsComponentBase.h"

#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

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

FORCEINLINE bool ACombatManager::WasBlockSuccessful(AActor * HitInstigator, AActor * HitActor, bool bLineHitResultFound, const FHitResult & LineHitResult)
{
	FVector HitActorForwardVector = HitInstigator->GetActorForwardVector();
	FVector HitNormal = LineHitResult.ImpactNormal;
	float Angle = CalculateAngleBetweenVectors(HitActorForwardVector, HitNormal);
	bool bResult = Angle < BlockDetectionAngle ? true : false;
	return bResult;
}

FORCEINLINE bool ACombatManager::GetCritChanceBoolean(const AEODCharacterBase * HitInstigator, const AEODCharacterBase * HitCharacter, const EDamageType& DamageType) const
{
	float CritRate = DamageType == EDamageType::Physical ? HitInstigator->StatsComp->GetPhysicalCritRate() : HitInstigator->StatsComp->GetMagickCritRate();
	bool bResult = CritRate >= FMath::RandRange(0.f, 100.f) ? true : false;
	return bResult;
}

float ACombatManager::GetActualDamage(const AEODCharacterBase * HitInstigator, const AEODCharacterBase * HitCharacter, const FSkillDamageInfo& SkillDamageInfo, const bool bCriticalHit, const bool bAttackBlocked)
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

	ActualDamage = ActualDamage * SkillDamageInfo.DamagePercent;

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

void ACombatManager::CharacterToCharacterAttack(AEODCharacterBase* HitInstigator, AEODCharacterBase* HitCharacter, const FSkillDamageInfo& SkillDamageInfo, const FHitResult& HitResult)
{
	check(HitInstigator);
	check(HitCharacter);

	if (!SkillDamageInfo.bUndodgable && HitCharacter->IsDodgingDamage())
	{
		DisplayDamageText(FString("Dodge"));
		HitCharacter->OnSuccessfulDodge(HitInstigator);
	}

	bool bLineHitResultFound = false;
	bool bCritHit = false;
	bool bAttackBlocked = false;

	FHitResult LineHitResult;
	bLineHitResultFound = GetLineHitResult(HitInstigator, HitCharacter, LineHitResult);
	bCritHit = GetCritChanceBoolean(HitInstigator, HitCharacter, SkillDamageInfo.DamageType);

	if (!SkillDamageInfo.bUnblockable && HitCharacter->IsBlockingDamage())
	{
		bAttackBlocked = WasBlockSuccessful(HitInstigator, HitCharacter, bLineHitResultFound, LineHitResult);
		if (bAttackBlocked)
		{
			HitCharacter->OnSuccessfulBlock(HitInstigator);
			HitInstigator->OnAttackBlocked(HitCharacter, SkillDamageInfo.bIgnoresBlock);
		}
	}

	float ActualDamage = GetActualDamage(HitInstigator, HitCharacter, SkillDamageInfo, bCritHit, bAttackBlocked);


}

void ACombatManager::CharacterToActorAttack(AEODCharacterBase* HitInstigator, AActor* HitActor, const FSkillDamageInfo& SkillDamageInfo, const FHitResult& HitResult)
{
}
