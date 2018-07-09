// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatusEffect.generated.h"

class ABaseCharacter;

UENUM(BlueprintType)
enum class EStatusEffectReactivationCondition :uint8
{
	None,
	Reset,
	Stack
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class EOD_API UStatusEffect : public UObject
{
	GENERATED_BODY()
	
public:

	UStatusEffect();
	
	/**
	 * Called to initialize a status effect on a character.
	 * @param Owner The character that owns the status effect
	 * @param Initiator The actor that initiated the status effect. Can be nullptr. For elemental effects this would be a weapon.
	 */
	virtual void OnInitialize(class ABaseCharacter* Owner, class AActor* Initiator) PURE_VIRTUAL(UStatusEffect::OnInitialize, );

	/** Called to deinitialize this status effect on a character */
	virtual void OnDeinitialize() PURE_VIRTUAL(UStatusEffect::OnDeinitialize, );

	/** Called when the status effect is activated */
	virtual void OnActivation(TArray<TWeakObjectPtr<ABaseCharacter>> RecipientCharacters) PURE_VIRTUAL(UStatusEffect::OnActivation, );

	/** Called when the status effect is deactivated */
	virtual void OnDeactivation() PURE_VIRTUAL(UStatusEffect::OnDeactivation, );

	ABaseCharacter* GetOwningCharacter() const;

	void SetOwningCharacter(ABaseCharacter* NewCharacter);
	
	/** The name of this status effect that will be used inside game */
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	FString InGameName;
	
	/** In-game description of this status effect */
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	FString Description;

protected:

	/** True if the status effect triggers on Owner receiving damage */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnReceivingHit : 1;

	/** True if the status effect triggers on Owner successfully landing an attack on enemy */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnSuccessfulHit : 1;

	/** True if the status effect triggers on Owner failing to land an attack on enemy */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnUnsuccessfulHit : 1;

	/** True if the status effect triggers on Owner successfully landing a critical attack on enemy */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnCriticalHit : 1;

	/** True if the status effect triggers on Owner successfully dodging an incoming enemy attack */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnSuccessfulDodge : 1;

	/** True if the status effect triggers on Owner successfully blocking an incoming enemy attack */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnSuccessfulBlock : 1;

	/** True if the status effect triggers on Owner regenerating full health */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnFullHealth : 1;

	/** True if the status effect triggers on Owner hitting 'low health' mark */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnLowHealth : 1;

	/** True if the status effect triggers on Owner entering combat */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnEnteringCombat : 1;

	/** True if the status effect triggers on Owner leaving combat */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnLeavingCombat : 1;
	
	/** True if the status effect triggers on using a particular skill */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnUsingSkill : 1;

	//~ @note Redundant property but it could be useful in certain situations
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnInitialization : 1;
	
	UPROPERTY(EditDefaultsOnly, Category = Reactivation)
	EStatusEffectReactivationCondition ReactivationCondition;
	
	/** Number of times this status effect can stack. Only applicable if ReactivationCondition is set to EStatusEffectReactivationCondition::Stack */
	UPROPERTY(EditDefaultsOnly, Category = Reactivation)
	int32 StackLimit;

	// @todo add buffs/debuffs that activate on getting hit by another spell, buff, etc.
	
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	class UTexture* Icon;
	
	/** Particle system associated with this status effect */
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	class UParticleSystem* ParticleSystem;

private:

	UPROPERTY(Transient)
	ABaseCharacter* OwningCharacter;

	
	// @todo Add flags to determine allies and enemies (for buff and debuff effects)
	// @note better to handle allies from ABaseCharacter class and add/use a function like TArray<ABaseCharacter*> GetAllies();
	// int AllyFlag;
	// @todo add boolean to check if this status effects deactives on death
	// @todo add boolean to determine whether the status effect is currently active or not
	
	
};
