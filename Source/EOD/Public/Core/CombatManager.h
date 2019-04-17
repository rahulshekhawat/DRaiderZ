// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODCharacterBase.h"
#include "CharacterLibrary.h"

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
	void OnMeleeAttack(AActor* HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults);

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
	
	/** Displays damage numbers on player screen */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Utility")
	void DisplayDamageMessage(const FString& Message, const FLinearColor& MessageColor, const FVector& Location);

	/** Displays status effect text on player screen */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Utility")
	void DisplayStatusEffectMessage(const FString& Message, const FLinearColor& MessageColor, const FVector& Location);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Utility")
	void PlayCameraShake(ECameraShakeType CameraShakeType, const FVector& EpiCenter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Utility")
	void SpawnHitSFX(const FVector& HitLocation, const FVector& HitNormal);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Utility")
	void SpawnBloodSpurt(const FVector& HitLocation, const FVector& HitNormal);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Utility")
	void PlayHitSound(const AEODCharacterBase* HitInstigator, const FVector& HitLocation, const bool bCriticalHit);

protected:

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	float PhysicalCritMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	float MagickalCritMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	FLinearColor BuffTextColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	FLinearColor DebuffTextColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	FLinearColor DodgeTextColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	FLinearColor PlayerDamagedTextColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Colors, meta = (DisplayName = "NPC Damaged Text Color"))
	FLinearColor NPCNormalDamagedTextColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Colors, meta = (DisplayName = "NPC Critically Damaged Text Color"))
	FLinearColor NPCCritDamagedTextColor;

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
