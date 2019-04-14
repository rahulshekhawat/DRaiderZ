// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveSkillBase.h"
#include "DynamicSpellCastingSkill.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UDynamicSpellCastingSkill : public UActiveSkillBase
{
	GENERATED_BODY()

public:

	UDynamicSpellCastingSkill(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//	Gameplay Skill Interface
	// --------------------------------------

	/** Initialize this skill. Intended to be called immediately after the skill object is created */
	virtual void InitSkill(AEODCharacterBase* Instigator, AController* Owner) override;

	virtual bool CanTriggerSkill() const override;

	/** Trigger this skill, i.e., either instantly activate this skill or start charging this skill. */
	virtual void TriggerSkill() override;

	/** Returns true if this skill can be cancelled */
	virtual bool CanCancelSkill() const override;

	virtual void UpdateSkill(float DeltaTime) override;

	/** Cancel this skill */
	virtual void CancelSkill() override;

	virtual void FinishSkill() override;

protected:

	// --------------------------------------
	//	Constants : Default values that are not supposed to be modified
	// --------------------------------------

	/**
	 * A map of weapons supported by this skill to their animation montages for male version of character
	 * @note If the character doesn't have any specified gender, then this will the default variable to access animation from
	 * @note This only applies to characters that can equip a custom weapon
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TMap<EWeaponType, TSoftObjectPtr<UAnimMontage>> WeaponToMaleUpperSlotAnimationsMap;

	/**
	 * A map of weapons supported by this skill to their animation montages for female version of character
	 * @note This only applies to characters that can equip a custom weapon
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TMap<EWeaponType, TSoftObjectPtr<UAnimMontage>> WeaponToFemaleUpperSlotAnimationsMap;

	UPROPERTY(Transient)
	TMap<EWeaponType, UAnimMontage*> SkillUpperSlotAnimations;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	bool bMovableMontagePlaying;

	FTimerHandle SkillTimerHandle;

	virtual void LoadFemaleAnimations() override;

	virtual void LoadMaleAnimations() override;

	virtual void OnFemaleAnimationsLoaded() override;

	virtual void OnMaleAnimationsLoaded() override;

	void TransitionBetweenMontages(UAnimInstance* AnimInstance, UAnimMontage* TransitionFromMontage, UAnimMontage* TransitionToMontage);

};
