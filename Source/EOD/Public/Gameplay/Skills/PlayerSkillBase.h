// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/GameplaySkillBase.h"
#include "PlayerSkillBase.generated.h"

/**
 * The base class of skills that can be used by player, added to skill bar, upgraded by player, etc.
 */
UCLASS()
class EOD_API UPlayerSkillBase : public UGameplaySkillBase
{
	GENERATED_BODY()
	
public:

	UPlayerSkillBase(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//  Gameplay Skill Interface
	// --------------------------------------

	/** Event called when player changes it's equipped weapon */
	virtual void OnWeaponChange(EWeaponType NewWeaponType, EWeaponType OldWeaponType);

	/** Event called when this skill is placed in SupersedingChainSkillGroup */
	virtual void OnActivatedAsChainSkill();

	/** Event called when this skill is removed from SupersedingChainSkillGroup */
	virtual void OnDeactivatedAsChainSkill();

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	FORCEINLINE int32 GetMaxUpgradeLevel() const { return MaxUpgrades; }

protected:

	/** Maximum upgrades available for this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	int32 MaxUpgrades;

public:

	// --------------------------------------
	//  Skill State
	// --------------------------------------

	/** Returns true if this skill is currently unlocked on skill tree */
	inline bool IsUnlocked() const { return CurrentUpgrade > 0; }

	/** Returns true if this skill is currently in cooldown */
	inline bool IsSkillInCooldown() const { return CooldownRemaining > 0.f; }

	FORCEINLINE int32 GetCurrentUpgrade() const { return CurrentUpgrade; }

	//~ @todo add check to make sure current upgrade is not more than maxupgrade
	inline void SetCurrentUpgrade(int32 Value) { CurrentUpgrade = Value; }

protected:

	int32 CurrentUpgrade;

	UPROPERTY()
	FTimerHandle CooldownTimerHandle;

	float CooldownRemaining;

	UFUNCTION()
	virtual void StartCooldown();

	UFUNCTION()
	virtual void FinishCooldown();

	UFUNCTION()
	virtual void CancelCooldown();

	UFUNCTION()
	virtual void UpdateCooldown();

};
