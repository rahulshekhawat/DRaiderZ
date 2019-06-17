// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODCharacterBase.h"
#include "CharacterLibrary.h"
#include "CombatLibrary.h"

#include "Camera/CameraShake.h"
#include "GameFramework/Info.h"
#include "CombatManager.generated.h"

class AActor;
class UCameraShake;
class APlayerCharacter;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class EOD_API ACombatManager : public AInfo
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	ACombatManager(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// --------------------------------------
	//  Combat
	// --------------------------------------

	/** Called when an actor attacks another actor */
	void OnMeleeAttack(
		AActor* HitInstigator,
		const bool bHit,
		const TArray<FHitResult>& HitResults,
		const FCollisionSkillInfo& CollisionSkillInfo);

	TSharedPtr<FAttackResponse> ProcessAttack(
		AActor* HitInstigator,
		ICombatInterface* InstigatorCI,
		const TSharedPtr<FAttackInfo>& AttackInfoPtr,
		AActor* HitTarget,
		ICombatInterface* TargetCI,
		const FHitResult& HitResult);

	//~ @todo OnRangedHit

	bool AreEnemies(AEODCharacterBase* CharOne, AEODCharacterBase* CharTwo);

	// --------------------------------------
	//  Utility
	// --------------------------------------

	/** Returns angle between two vectors */
	inline float CalculateAngleBetweenVectors(FVector Vec1, FVector Vec2);

	/** Returns angle between HitCharacter's forward actor and LineHitResult's ImpactNormal */
	inline float GetBCAngle(AEODCharacterBase* HitCharacter, const FHitResult& LineHitResult);

	/**
	 * Does a line trace from HitInstigator to HitTarget
	 * @param HitInstigator The actor that initiated the line trace
	 * @param HitTarget The supposed target that we want to do line trace to
	 * @param OutHitResult Outs the hit result of line trace
	 * @param bOutLineHitResultFound True if line trace actually hit the HitTarget
	 */
	void GetLineHitResult(const AActor* HitInstigator, const AActor* HitTarget, FHitResult& OutHitResult, bool& bOutLineHitResultFound) const;

	/**
	 * Does a line trace from HitInstigator to HitTarget
	 * @param HitInstigator The actor that initiated the line trace
	 * @param HitTarget The supposed target that we want to do line trace to
	 * @param OutHitResult Outs the hit result of line trace
	 * @param bOutLineHitResultFound True if line trace actually hit the HitTarget
	 */
	void GetLineHitResult(const AActor* HitInstigator, const UPrimitiveComponent* HitComponent, FHitResult& OutHitResult, bool& bOutLineHitResultFound) const;

};

inline float ACombatManager::CalculateAngleBetweenVectors(FVector Vec1, FVector Vec2)
{
	FVector NormalizedVec1 = Vec1.GetSafeNormal();
	FVector NormalizedVec2 = Vec2.GetSafeNormal();
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(NormalizedVec1, NormalizedVec2)));

	return Angle;
}

inline float ACombatManager::GetBCAngle(AEODCharacterBase* HitCharacter, const FHitResult& LineHitResult)
{
	return CalculateAngleBetweenVectors(HitCharacter->GetActorForwardVector(), LineHitResult.ImpactNormal);
}
