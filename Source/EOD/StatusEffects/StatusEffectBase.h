// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "UObject/NoExportTypes.h"
#include "StatusEffectBase.generated.h"

class AEODCharacterBase;

/** This USTRUCT is just a wrapper around TWeakObjectPtr<AEODCharacterBase> so that it can be passed as a parameter for UFUNCTION */
USTRUCT()
struct EOD_API FBaseCharacter_WeakObjPtrWrapper
{
	GENERATED_USTRUCT_BODY()

public:

	TWeakObjectPtr<AEODCharacterBase> RecipientCharacter;

};

/** FStatusInfo holds the required info required by the CharacterToStatusInfoMap */
USTRUCT()
struct EOD_API FStatusInfo
{
	GENERATED_USTRUCT_BODY()

public:

	int CurrentStackLevel;

	float TotalElapsedTime;

	FTimerHandle* TimerHandle;

	FTimerDelegate TimerDelegate;

	FStatusInfo()
	{
		CurrentStackLevel = 0;
		TotalElapsedTime = 0.f;
		TimerHandle = nullptr;
	}
};

/**
 * The base abstract class for status effects
 * 
 * @note The status effect class is not analogous to in-game status effects. Think of this class as the status effects manager class, which
 * has the ability to manage the behavior and state of one particular type of status effect for a particular owner. For example, an object
 * of BleedStatusEffect class will simply manage the status effects of 'bleed' type that are triggered by its owner.
 * 
 * Initialize() must be called right after any of the status effect class is instantiated. Failing to do so will throw a runtime error.
 * Initialize() would set up any prerequisites for the class. Similarly, Deinitialize() must be called right before the status effect's
 * object is destroyed.
 * 
 * The status effect object will persist in memory as long as the instigator persists. e.g., if a weapon with elemental enchant has been
 * equipped by player, the elemental status effect object will persist in memory as long as the weapon is equipped by player and must be
 * destroyed when the weapon is unequipped.
 */
UCLASS()
class EOD_API UStatusEffectBase : public UObject
{
	GENERATED_BODY()
	
public:

	/** Initializes booleans to defaults */
	UStatusEffectBase(const FObjectInitializer& ObjectInitializer);
	
	/**
	 * Called to initialize a status effect on a character.
	 * @param Owner The character that owns the status effect
	 * @param Instigator The actor that initiated the status effect. Can be nullptr. For a weapon with elemental enchant, the Instigator would be the weapon.
	 */
	virtual void Initialize(class AEODCharacterBase* Owner, class AActor* Instigator);
	
	/** Called to deinitialize this status effect on a character */
	virtual void Deinitialize();

	/** Called when an event that triggers this status effect occurs */
	virtual void OnTriggerEvent(TArray<TWeakObjectPtr<AEODCharacterBase>>& RecipientCharacters);

	/** Called for recipient character to deactivate this status effect, i.e., using a potion to stop bleed effect */
	virtual void RequestDeactivation(AEODCharacterBase* Character);
	
	/** Status effect name that will be visible to player inside game */
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	FString InGameName;
	
	/** The status effect description that will be displayed to player on hovering over the status effect icon */
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	FString TooltipDescription;

	/**
	 * The detailed in-game description of this status effect which may or may not be made available for player
	 * This can be null, in which case TooltipDescription will be used wherever needed.
	 */
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	FString DetailedDescription;

	/** Icon associated with this status effect */
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	class UTexture* Icon;
	
	/** Particle system associated with this status effect */
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	class UParticleSystem* ParticleSystem;

	/** Character bone that the particle effect should attach to */	
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	FName ParticleEffectAttachBone;
	
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	float ActivationChance;
	
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	float TickInterval;

	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	float StatusEffectDuration;
	
	// @todo Add a boolean to follow particle effect attach bone if needed
	
	AEODCharacterBase* GetOwningCharacter() const;

	AActor* GetInstigator() const;

	void SetOwningCharacter(AEODCharacterBase* NewCharacter);

	void SetInstigator(AActor* NewInstigator);

	virtual TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>* GetCharacterToStatusInfoMap() PURE_VIRTUAL(UStatusEffect::OnStatusEffectTick, return nullptr; );

protected:

	/** Map of characters affected by this status effect */
	// static TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> CharacterToStatusInfoMap;

	/**
	 * Determines if this status effect should reset on reactivation.
	 * If this is set to false, stacking will be disabled by default and StackLimit will be ignored
	 */
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	uint32 bResetsOnReactivation : 1;

	/** True if the status effect triggers on Owner receiving damage */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnReceivingHit : 1;

	/** True if the status effect triggers on Owner successfully landing an attack on enemy */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnSuccessfulHit : 1;

	/** True if the status effect triggers on Owner successfully landing a magick attack on enemy */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnSuccessfulMagickAttack : 1;

	/** True if the status effect triggers on Owner successfully landing a physical attack on enemy */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnSuccessfulPhysicalAttack : 1;

	/** True if the status effect triggers if owner kills an enemy */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnKillingEnemy : 1;

	/** True if the status effect triggers on Owner failing to land an attack on enemy */
	// UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	// uint32 bTriggersOnUnsuccessfulHit : 1;

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

	/** True if the status effect triggers on successfully attacking an enemy with a particular skill */
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnSuccessfulSkillAttack : 1;

	//~ @note Redundant property but it could be useful in certain situations
	UPROPERTY(EditDefaultsOnly, Category = ActivationCondition)
	uint32 bTriggersOnInitialization : 1;
	
	/** Number of times this status effect can stack. Only applicable if ReactivationCondition is set to EStatusEffectReactivationCondition::Stack */
	UPROPERTY(EditDefaultsOnly, Category = Reactivation)
	int32 StackLimit;

	// @todo add buffs/debuffs that activate on getting hit by another spell, buff, etc.
	
	/** Called to activate this status effect on a recipient character */
	virtual void ActivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter);
	
	/** Called to deactivate this status effect on a recipient character */
	virtual void DeactivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter);

	/** Called to process the ticking of this status effect. Must be overridden in inherited classes */
	UFUNCTION()
	virtual void OnStatusEffectTick(FBaseCharacter_WeakObjPtrWrapper& WrappedRecipientCharacter) PURE_VIRTUAL(UStatusEffect::OnStatusEffectTick, );

private:

	UPROPERTY(Transient)
	AEODCharacterBase* OwningCharacter;

	UPROPERTY(Transient)
	AActor* Instigator;
	
	// @todo Add flags to determine allies and enemies (for buff and debuff effects)
	// @note better to handle allies from ABaseCharacter class and add/use a function like TArray<ABaseCharacter*> GetAllies();
	// int AllyFlag;
	// @todo add boolean to check if this status effects deactives on death
	// @todo add boolean to determine whether the status effect is currently active or not
	
	
	
};
