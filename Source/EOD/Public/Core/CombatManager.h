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

	ACombatManager(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	/** Called when an actor attacks another actor */
	void OnMeleeAttack(AActor* HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults);

	//~ @todo OnRangedHit

	/** Returns angle between two vectors */
	FORCEINLINE float CalculateAngleBetweenVectors(FVector Vec1, FVector Vec2);

	void NativeDisplayDamage(const AEODCharacterBase* HitInstigator,
				 			 const AEODCharacterBase* HitCharacter,
							 const FHitResult& LineHitResult,
							 const float ActualDamage,
				 			 const bool bCriticalHit);

	/** Displays damage numbers on player screen */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = WidgetText)
	void DisplayDamageMessage(const FString& Message, const FLinearColor& MessageColor, const FVector& Location);

	/** Displays status effect text on player screen */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = WidgetText)
	void DisplayStatusEffectMessage(const FString& Message, const FLinearColor& MessageColor, const FVector& Location);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = WidgetText)
	void PlayCameraShake(ECameraShakeType CameraShakeType, const FVector& EpiCenter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = CombatEvent)
	void SpawnHitSFX(const FVector& HitLocation, const FVector& HitNormal);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = CombatEvent)
	void SpawnBloodSpurt(const FVector& HitLocation, const FVector& HitNormal);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = CombatEvent)
	void PlayHitSound(const AEODCharacterBase* HitInstigator, const FVector& HitLocation, const bool bCriticalHit);

	UFUNCTION(BlueprintImplementableEvent, Category = CombatEvent, meta = (DisplayName = "Character To Character Attack"))
	void BP_CharacterToCharacterAttack(AEODCharacterBase* HitInstigator,
									   AEODCharacterBase* HitCharacter,
									   const FSkillDamageInfo& SkillDamageInfo,
									   const FHitResult& AttackHitResult,
									   const FHitResult& LineHitResult);

protected:

	/** The maximum angle to which an incoming damage can be blocked */
	UPROPERTY(EditDefaultsOnly, Category = Combat, BlueprintReadOnly)
	float BlockDetectionAngle;

	UPROPERTY(EditDefaultsOnly, Category = Combat, BlueprintReadOnly)
	float PhysicalCritMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = Combat, BlueprintReadOnly)
	float MagickalCritMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = Colors, BlueprintReadOnly)
	FLinearColor BuffTextColor;

	UPROPERTY(EditDefaultsOnly, Category = Colors, BlueprintReadOnly)
	FLinearColor DebuffTextColor;

	UPROPERTY(EditDefaultsOnly, Category = Colors, BlueprintReadOnly)
	FLinearColor DodgeTextColor;

	UPROPERTY(EditDefaultsOnly, Category = Colors, BlueprintReadOnly)
	FLinearColor PlayerDamagedTextColor;

	UPROPERTY(EditDefaultsOnly, Category = Colors, BlueprintReadOnly, meta = (DisplayName = "NPC Damaged Text Color"))
	FLinearColor NPCNormalDamagedTextColor;

	UPROPERTY(EditDefaultsOnly, Category = Colors, BlueprintReadOnly, meta = (DisplayName = "NPC Critically Damaged Text Color"))
	FLinearColor NPCCritDamagedTextColor;

private:

	/** Determines whether the hit actor succesfully blocked an attack based on the positions of itself and the attacking actor */
	inline bool WasBlockSuccessful(
		const AActor* HitInstigator,
		const AActor* HitActor,
		const bool bLineHitResultFound,
		const FHitResult& LineHitResult);

	/**
	 * Generates a random boolean based on HitInstigator's crit rate and HitCharacter's crit resistance,
	 * that could be used to determine if the attack was a critical attacl
	 */
	inline bool GetCritChanceBoolean(
		const AEODCharacterBase* HitInstigator,
		const AEODCharacterBase* HitCharacter,
		const EDamageType& DamageType) const;

	FORCEINLINE float GetBCAngle(AEODCharacterBase* HitCharacter, const FHitResult& LineHitResult);

	float GetActualDamage(
		const AEODCharacterBase* HitInstigator,
		const AEODCharacterBase* HitCharacter,
		const FSkillDamageInfo& SkillDamageInfo,
		const bool bCriticalHit,
		const bool bAttackBlocked);

	/**
	 * Calculates and returns the actual damage that should be inflicted by HitInstigator on HitCharacter
	 * @param HitInstigator The character that initiated the attack
	 * @param HitCharacter The character that got damage by the attack
	 * @param SkillUsed The skill that HitInstigator used to attack
	 * @param bCriticalHit True if the hit was critical
	 * @param bAttackBlocked True if the HitCharacter blocked the incoming attack from HitInstigator
	 * @return Returns the actual damage that should be inflicted on HitCharacter
	 */
	float GetActualDamage(
		const AEODCharacterBase* HitInstigator,
		const AEODCharacterBase* HitCharacter,
		const FSkillTableRow* SkillUsed,
		const bool bCriticalHit,
		const bool bAttackBlocked);

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

	/** Processes an attack from an Actor */
	void ProcessActorAttack(AActor* HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults);

	/** Processes an attack from an EODCharacterBase */
	void ProcessCharacterAttack(AEODCharacterBase* HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults);

	/**
	 * Handles an attack from one character to another character
	 * @param HitInstigator The character that initiated the collision sweep
	 * @param HitCharacter The character that got hit by collision sweep
	 * @param SkillUsed The skill that HitInstigator used to initiate the collision sweep
	 * @param bOutHitCharacterReceivedDamage Outs true if HitCharacter received any form of damage (i.e., true if HitCharacter didn't dodge or was not an enemy of HitInstigator). OutDamageInflicted may be zero even if bOutHitCharacterReceivedDamage is true.
	 * @param OutDamageInflicted Actual damage inflicted on HitCharacter
	 */
	void CharacterToCharacterAttack(
		AEODCharacterBase* HitInstigator,
		AEODCharacterBase* HitCharacter,
		const FSkillTableRow* SkillUsed,
		const FHitResult& HitResult,
		bool& bOutHitCharacterReceivedDamage,
		float& OutDamageInflicted);

	/**
	 * Handles an attack from one character to another character
	 * @param HitInstigator The character that initiated the collision sweep
	 * @param HitActor The actor that got hit by collision sweep
	 * @param SkillUsed The skill that HitInstigator used to initiate the collision sweep
	 * @param bOutHitActorReceivedDamage Outs true if HitActor received any form of damage (i.e., true if HitActor didn't dodge or was not an enemy of HitInstigator). OutDamageInflicted may be zero even if bOutHitActorReceivedDamage is true.
	 * @param OutDamageInflicted Actual damage inflicted on HitCharacter
	 */
	void CharacterToActorAttack(
		AEODCharacterBase* HitInstigator,
		AActor* HitActor,
		const FSkillTableRow* SkillUsed,
		const FHitResult& HitResult,
		bool& bOutHitActorReceivedDamage,
		float& OutDamageInflicted);

	/** Handles an attack from one character to another character */
	/*
	void CharacterToCharacterAttack(
		AEODCharacterBase* HitInstigator,
		AEODCharacterBase* HitCharacter,
		const FSkillDamageInfo& SkillDamageInfo,
		const FHitResult& HitResult);
	*/

	/** Handles an attack from a character to an actor */
	/*
	void CharacterToActorAttack(
		AEODCharacterBase* HitInstigator,
		AActor* HitActor,
		const FSkillDamageInfo& SkillDamageInfo,
		const FHitResult& HitResult);
	*/

	bool ApplyCrowdControlEffects(
		AEODCharacterBase* HitInstigator,
		AEODCharacterBase* HitCharacter,
		const FSkillDamageInfo& SkillDamageInfo,
		const FHitResult& LineHitResult,
		const float BCAngle);

	FORCEINLINE FSkillDamageInfo GetSkillDamageInfoFromSkill(FSkillTableRow* Skill);

	bool AreEnemies(AEODCharacterBase* CharOne, AEODCharacterBase* CharTwo);

};

FORCEINLINE float ACombatManager::CalculateAngleBetweenVectors(FVector Vec1, FVector Vec2)
{
	FVector NormalizedVec1 = Vec1.GetSafeNormal();
	FVector NormalizedVec2 = Vec2.GetSafeNormal();
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(NormalizedVec1, NormalizedVec2)));

	return Angle;
}

inline bool ACombatManager::WasBlockSuccessful(
	const AActor* HitInstigator,
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

inline bool ACombatManager::GetCritChanceBoolean(
	const AEODCharacterBase* HitInstigator,
	const AEODCharacterBase* HitCharacter,
	const EDamageType& DamageType) const
{
	float CritRate = DamageType == EDamageType::Physical ? HitInstigator->GetCharacterStatsComponent()->GetPhysicalCritRate() : HitInstigator->GetCharacterStatsComponent()->GetMagickCritRate();
	bool bResult = CritRate >= FMath::RandRange(0.f, 100.f) ? true : false;
	return bResult;
}

FORCEINLINE float ACombatManager::GetBCAngle(AEODCharacterBase* HitCharacter, const FHitResult& LineHitResult)
{
	return CalculateAngleBetweenVectors(HitCharacter->GetActorForwardVector(), LineHitResult.ImpactNormal);
}

FORCEINLINE FSkillDamageInfo ACombatManager::GetSkillDamageInfoFromSkill(FSkillTableRow* Skill)
{
	check(Skill);
	FSkillDamageInfo SkillDamageInfo;
	SkillDamageInfo.bIgnoresBlock = Skill->bIgnoresBlock;
	SkillDamageInfo.bUnblockable = Skill->bUnblockable;
	SkillDamageInfo.bUndodgable = Skill->bUndodgable;
	SkillDamageInfo.CrowdControlEffect = Skill->CrowdControlEffect;
	SkillDamageInfo.CrowdControlEffectDuration = Skill->CrowdControlEffectDuration;
	SkillDamageInfo.DamagePercent = Skill->DamagePercent;
	SkillDamageInfo.DamageType = Skill->DamageType;
	return SkillDamageInfo;
}
