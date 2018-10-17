// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/CharacterLibrary.h"
#include "Camera/CameraShake.h"
#include "GameFramework/Info.h"
#include "CombatManager.generated.h"

UENUM(BlueprintType)
enum class ECameraShakeType : uint8
{
	Weak,
	Medium,
	Strong
};

class AActor;
class UCameraShake;
class AEODCharacterBase;
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

	/** Displays damage numbers on player screen */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = WidgetText)
	void DisplayDamageMessage(const FString& Message, const FLinearColor& MessageColor, const FVector& Location);

	/** Displays status effect text on player screen */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = WidgetText)
	void DisplayStatusEffectMessage(const FString& Message, const FLinearColor& MessageColor, const FVector& Location);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = WidgetText)
	void PlayCameraShake(ECameraShakeType CameraShakeType, const FVector& EpiCenter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = WidgetText)
	void PlayCameraShakeFromClass(TSubclassOf<UCameraShake> CameraShakeClass, const FVector& EpiCenter);

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
	FORCEINLINE bool WasBlockSuccessful(AActor* HitInstigator, AActor* HitActor, bool bLineHitResultFound, const FHitResult& LineHitResult);

	/**
	 * Generates a random boolean based on HitInstigator's crit rate and HitCharacter's crit resistance,
	 * that could be used to determine if the attack was a critical attacl
	 */
	FORCEINLINE bool GetCritChanceBoolean(const AEODCharacterBase* HitInstigator,
								  		  const AEODCharacterBase* HitCharacter,
										  const EDamageType& DamageType) const;

	float GetActualDamage(const AEODCharacterBase* HitInstigator,
						  const AEODCharacterBase* HitCharacter,
						  const FSkillDamageInfo& SkillDamageInfo,
						  const bool bCriticalHit,
						  const bool bAttackBlocked);

	/**
	 * Does a line trace from HitInstigator to HitTarget
	 * @param HitInstigator The actor that initiated the line trace
	 * @param HitTarget The supposed target that we want to do line trace to
	 * @param OutHitResult Outs the hit result of line trace
	 * @return Returns true if line trace actually hit the HitTarget
	 */
	bool GetLineHitResult(const AActor* HitInstigator, const AActor* HitTarget, FHitResult& OutHitResult) const;

	/** Processes an attack from an AActor */
	void ProcessActorAttack(AActor* HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults);

	/** Processes an attack from an AEODCharacterBase */
	void ProcessCharacterAttack(AEODCharacterBase* HitInstigator, const bool bHit, const TArray<FHitResult>& HitResults);

	/** Handles an attack from one character to another character */
	void CharacterToCharacterAttack(AEODCharacterBase* HitInstigator,
									AEODCharacterBase* HitCharacter,
									const FSkillDamageInfo& SkillDamageInfo,
									const FHitResult& HitResult);

	/** Handles an attack from a character to an actor */
	void CharacterToActorAttack(AEODCharacterBase* HitInstigator,
								AActor* HitActor,
								const FSkillDamageInfo& SkillDamageInfo,
								const FHitResult& HitResult);


};
