// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "UObject/NoExportTypes.h"
#include "StatusEffect.generated.h"

class ABaseCharacter;

USTRUCT()
struct EOD_API FBaseCharacter_WeakObjPtrWrapper
{
	GENERATED_USTRUCT_BODY()

public:

	TWeakObjectPtr<ABaseCharacter> RecipientCharacter;

};

USTRUCT()
struct EOD_API FStatusTickInfo
{
	GENERATED_USTRUCT_BODY()

public:

	int CurrentStackLevel;

	float TotalElapsedTime;

	FTimerHandle* TimerHandle;

	FTimerDelegate TimerDelegate;

	TWeakObjectPtr<ABaseCharacter> RecipientCharacter;

};

/**
 * The base abstract class for status effects
 * 
 * @note The status effect class is not analogous to in-game status effects
 * Think of this class as the status effects manager class, which has the ability to manage the behavior and state of 
 * one particular type of status effect for a particular owner.
 * For example, an object of BleedStatusEffect class will simply manage the status effects of 'bleed' type that are triggered by its owner.
 * Keeping that in mind, the difference between OnInitialize and OnActivation is easy to understand.
 * OnInitiailize must be called right after the status effect class is instantiated.
 * For example, if a player equips a weapon that has the ability to cause bleeding in-game then an object of BleedStatusEffect
 * will be instantiated as soon as the player equips that weapon, and OnInitialize will be immediately called to set the player as owner
 * of that status effect, as well as to do any necessary preprocessing.
 * The 'OnActivation' function, however, will only be called whenever an event that could trigger the bleed status effect happens.
 * In the example above, if the player does a critical damage on enemy, `OnActivation` will be called. This function will do the necessary
 * chance calculation (50% chance of bleed on critical hit), and initiate the status effect on enemy character.
 * It also implies that the status effect object will persist in memory as long as the weapon to cause bleed will be equipped by character.
 * The status effect object shall be deinitialized and then destroyed when the weapon is unequipped.
 */
UCLASS(Abstract, Blueprintable)
class EOD_API UStatusEffect : public UObject
{
	GENERATED_BODY()
	
public:

	/** Initializes booleans for status effects */
	UStatusEffect();
	
	virtual void Initialize(class ABaseCharacter* Owner, class AActor* Instigator);

	virtual void Deinitialize();

	virtual void OnTriggerEvent(TArray<TWeakObjectPtr<ABaseCharacter>>& RecipientCharacters);

	virtual void RequestDeactivation(ABaseCharacter* Character);

	/**
	 * Called to initialize a status effect on a character.
	 * @param Owner The character that owns the status effect
	 * @param Instigator The actor that initiated the status effect. Can be nullptr. For a weapon with elemental enchant, the Instigator would be the weapon.
	 */
	// virtual void OnInitialize(class ABaseCharacter* Owner, class AActor* Instigator) PURE_VIRTUAL(UStatusEffect::OnInitialize, );

	/** Called to deinitialize this status effect on a character */
	// virtual void OnDeinitialize() PURE_VIRTUAL(UStatusEffect::OnDeinitialize, );

	/** Called when the status effect is activated */
	// virtual void OnActivation(TArray<TWeakObjectPtr<ABaseCharacter>> RecipientCharacters) PURE_VIRTUAL(UStatusEffect::OnActivation, );

	/** Called when the status effect is deactivated */
	// virtual void OnDeactivation() PURE_VIRTUAL(UStatusEffect::OnDeactivation, );
	
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
	
	// @todo Add a boolean to follow particle effect attach bone if needed
	
	ABaseCharacter* GetOwningCharacter() const;

	AActor* GetInstigator() const;

	void SetOwningCharacter(ABaseCharacter* NewCharacter);

	void SetInstigator(AActor* NewInstigator);

protected:

	/** Map of characters affected by this status effect */
	static TMap<TWeakObjectPtr<ABaseCharacter>, FStatusTickInfo> CharacterToStatusTickInfoMap;

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
	
	/** Number of times this status effect can stack. Only applicable if ReactivationCondition is set to EStatusEffectReactivationCondition::Stack */
	UPROPERTY(EditDefaultsOnly, Category = Reactivation)
	int32 StackLimit;

	// @todo add buffs/debuffs that activate on getting hit by another spell, buff, etc.
	
	virtual void ActivateStatusEffect(TWeakObjectPtr<ABaseCharacter>& RecipientCharacter);

	virtual void DeactivateStatusEffect(TWeakObjectPtr<ABaseCharacter>& RecipientCharacter);

	UFUNCTION()
	virtual void OnStatusEffectTick(FBaseCharacter_WeakObjPtrWrapper& StatusTickInfo) PURE_VIRTUAL(UStatusEffect::OnStatusEffectTick, );
	// virtual void OnStatusEffectTick(FStatusTickInfo& StatusTickInfo);
	// virtual void OnStatusEffectTick(FStatusTickInfo& StatusTickInfo) PURE_VIRTUAL(UStatusEffect::OnStatusEffectTick, );


private:

	UPROPERTY(Transient)
	ABaseCharacter* OwningCharacter;

	UPROPERTY(Transient)
	AActor* Instigator;
	
	// @todo Add flags to determine allies and enemies (for buff and debuff effects)
	// @note better to handle allies from ABaseCharacter class and add/use a function like TArray<ABaseCharacter*> GetAllies();
	// int AllyFlag;
	// @todo add boolean to check if this status effects deactives on death
	// @todo add boolean to determine whether the status effect is currently active or not
	
};
