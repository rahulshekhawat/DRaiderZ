// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/CharacterLibrary.h"
#include "Player/EODCharacterBase.h"

#include "Engine/StreamableManager.h"
#include "AICharacterBase.generated.h"

class UEODWidgetComponent;

/**
 * AICharacterBase is the base class for AI controlled characters
 */
UCLASS()
class EOD_API AAICharacterBase : public AEODCharacterBase
{
	GENERATED_BODY()
	
public:

	AAICharacterBase(const FObjectInitializer& ObjectInitializer);

	/** Initialize TArray<FSkill*> */
	virtual void PostInitializeComponents() override;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;

	/** Get aggro widget component */
	FORCEINLINE UEODWidgetComponent* GetAggroWidgetComp() const { return AggroWidgetComp; }

	/** Get health widget component */
	FORCEINLINE UEODWidgetComponent* GetHealthWidgetComp() const { return HealthWidgetComp; }

	/** Get aggro widget component */
	UFUNCTION(BlueprintPure, Category = WidgetComponent, meta = (DisplayName = "Get Aggro Widget Component"))
	UEODWidgetComponent* BP_GetAggroWidgetComp() const;

	/** Get health widget component */
	UFUNCTION(BlueprintPure, Category = WidgetComponent, meta = (DisplayName = "Get Health Widget Component"))
	UEODWidgetComponent* BP_GetHealthWidgetComp() const;

	/** Flinch this character (visual feedback) */
	virtual bool CCEFlinch_Implementation(const float BCAngle) override;

	/** Interrupt this character's current action */
	virtual bool CCEInterrupt_Implementation(const float BCAngle) override;

	/** Applies stun to this character */
	virtual bool CCEStun_Implementation(const float Duration) override;

	/** Removes 'stun' crowd control effect from this character */
	virtual void CCERemoveStun_Implementation() override;

	/** Freeze this character */
	virtual bool CCEFreeze_Implementation(const float Duration) override;

	/** Removes 'freeze' crowd control effect from this character */
	virtual void CCEUnfreeze_Implementation() override;
	
	/** Knockdown this character */
	virtual bool CCEKnockdown_Implementation(const float Duration) override;
	
	/** Removes 'knock-down' crowd control effect from this character */
	virtual void CCEEndKnockdown_Implementation() override;
	
	/** Knockback this character */
	virtual bool CCEKnockback_Implementation(const float Duration, const FVector& ImpulseDirection) override;

	/** Set whether character is in combat or not */
	virtual void SetInCombat(bool bValue) override;

	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	/** Use a skill and play it's animation */
	virtual bool UseSkill(FName SkillID) override;

	/**
	 * Determines and returns the status of a skill
	 * Returns EEODTaskStatus::Active if character is currently using the skill
	 * Returns EEODTaskStatus::Finished if character has finished using the skill
	 * Returns EEODTaskStatus::Aborted if the skill was aborted before completion
	 * Returns EEODTaskStatus::Inactive if the character is using or have used a different skill
	 */
	virtual EEODTaskStatus CheckSkillStatus(FName SkillID) override;

	/** [AI] Returns the melee attack skill that is more appropriate to use in current state against the given enemy */
	virtual FName GetMostWeightedMeleeSkillID(const AEODCharacterBase* TargetCharacter) const override;

private:

	/** Used to display floating aggro widget above AI character */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UEODWidgetComponent* AggroWidgetComp;

	/** Used to display floating health widget above AI character */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UEODWidgetComponent* HealthWidgetComp;

	/** In game name of this AI character. This may or may not differ for each instance of character */
	UPROPERTY(EditAnywhere, Category = BaseInfo, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString InGameName;

	/** In game description of this AI character. This may or may not differ for each instance of character */
	UPROPERTY(EditAnywhere, Category = BaseInfo, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString InGameDescription;

	/** Maximum speed an AI character can walk with inside combat */
	UPROPERTY(EditDefaultsOnly, Category = AIMovement, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxWalkSpeedInCombat;

	/** Maximum speed an AI character can walk with outside combat */
	UPROPERTY(EditDefaultsOnly, Category = AIMovement, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxWalkSpeedOutsideCombat;

	/** Animation montage containing animations for hit effects */
	UPROPERTY(EditDefaultsOnly, Category = Animations, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitEffectsAnimMontage;

	/** Animation montage containing animations for character flinch */
	UPROPERTY(EditDefaultsOnly, Category = Animations, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FlinchAnimMontage;

	/** Changes maximum walk speed of character based on whether character is engaged in combat or not */
	void UpdateMaxWalkSpeed();

	//~ @todo replace following boolean with just default crowd control immunities in stats component
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bHasFlinchAnimations;

	/** Cache for skills that can knock down enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> KnockDownSkills;

	/** Cache for skills that can knock back enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> KnockBackSkills;

	/** Cache for skills that can interrupt enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> InterruptSkills;

	/** Cache for skills that can stun enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> StunSkills;
	
	/** Cache for skills that can crystalize enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> CrystalizeSkills;

	/** Cache for skills that can flinch enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> FlinchSkills;

	/** Cache for ranged skills */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> RangedSkills;

	/** Cache for melee skills */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> MeleeSkills;

	/** Cache for self healing skills */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> SelfHealSkills;

	/** Cache for party healing skills */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> PartyHealSkills;

	/** Cache for skills that put a debuff on enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> DebuffSkills;

	/** Cache for self buff skills */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> SelfBuffSkills;

	/** Cache for party buff skills */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> PartyBuffSkills;

	/** A map of skills to their usability weight */
	// UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TMap<FName, int32> SkillPriorityMap;

	/** A map of skills to their usability weight */
	// UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TMap<FName, int32> SkillWeightMap;

	TSharedPtr<FStreamableHandle> SkillAnimationsStreamableHandle;

	void InitializeSkills();

};
