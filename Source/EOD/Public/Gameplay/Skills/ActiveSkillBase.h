// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"

#include "Engine/StreamableManager.h"
#include "Gameplay/Skills/PlayerSkillBase.h"
#include "ActiveSkillBase.generated.h"

class UGameplayEffectBase;


USTRUCT(BlueprintType)
struct EOD_API FActiveSkillLevelUpInfo
{
	GENERATED_USTRUCT_BODY()

	/** Stamina cost for using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Costs")
	int32 StaminaCost;

	/** Mana cost for using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Costs")
	int32 ManaCost;

	/** Damage in percentage of player's magickal or physical attack that will be inflicted on enemy */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information")
	int32 DamagePercent;

	/** Determines if this skill can be blocked */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information")
	bool bUnblockable;

	/** Determines if this skill can be dodged */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information")
	bool bUndodgable;

	/** Determines if the 'skill deflected' animation should play on getting blocked. Only applicable if this skill can be blocked */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information")
	bool bIgnoresBlock;

	/** Crowd control effect on hit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information")
	ECrowdControlEffect CrowdControlEffect;

	/** The duration for which the crowd control effect should last (if applicable) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information")
	float CrowdControlEffectDuration;

	/** Immunities from crowd control effects granted on using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information", meta = (Bitmask, BitmaskEnum = "ECrowdControlEffect"))
	uint8 CrowdControlImmunities;

	/** The duration before which the same skill can be used again */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information")
	float Cooldown;

	//~ @todo
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffectBase> GameplayEffectClass;
	// TSoftClassPtr<UGameplayEffectBase> GameplayEffectSoftClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	ESkillEventTriggerCondition GameplayEffectTriggerCondition;
	// EGameplayEffectActivationCondition GameplayEffectActivationCondition;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	EGameplayEffectAuthority GameplayEffectAuthority;

	FActiveSkillLevelUpInfo()
	{
		StaminaCost = 0;
		ManaCost = 0;
		DamagePercent = 0;
		bUnblockable = false;
		bUndodgable = false;
		bIgnoresBlock = false;
		CrowdControlEffect = ECrowdControlEffect::Flinch;
		CrowdControlEffectDuration = 0.f;
		CrowdControlImmunities = 0;
		Cooldown = 0.f;
		GameplayEffectClass = NULL;
		// GameplayEffectSoftClass = NULL;
		GameplayEffectTriggerCondition = ESkillEventTriggerCondition::TriggersOnSkillTrigger;
		// GameplayEffectActivationCondition = EGameplayEffectActivationCondition::ActivatesOnSkillTrigger;
		GameplayEffectAuthority = EGameplayEffectAuthority::ClientOwner;
	}
};

/**
 * Base class for all active skills
 */
UCLASS()
class EOD_API UActiveSkillBase : public UPlayerSkillBase
{
	GENERATED_BODY()

public:

	UActiveSkillBase(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//	Gameplay Skill Interface
	// --------------------------------------

	/** Initialize this skill. Intended to be called immediately after the skill object is created */
	virtual void InitSkill(AEODCharacterBase* Instigator, AController* Owner) override;

	/**
	 * Returns true if the skill owner has enough stats to commit this skill
	 * @note Intended to be called from server or client owner
	 */
	virtual bool CanCommitSkill() const;

	/**
	 * Commits this ability by deducting skill cost from player stats
	 * @note Intended to be called from server or client owner
	 */
	virtual void CommitSkill();

	virtual void ApplyRotation();

	virtual bool CanTriggerSkill() const;

	/** Trigger this skill, i.e., either instantly activate this skill or start charging this skill. */
	virtual void TriggerSkill() override;

	/** Returns true if this skill can be cancelled */
	virtual bool CanCancelSkill() const override;

	/** Cancel this skill */
	virtual void CancelSkill() override;

	virtual void FinishSkill() override;

	virtual void TriggerGameplayEffects() override;

	virtual TSharedPtr<FAttackInfo> GetAttackInfoPtr() override;

	inline FActiveSkillLevelUpInfo GetCurrentSkillLevelupInfo() const;

protected:

	virtual void StartCooldown() override;

	virtual void FinishCooldown() override;

	virtual void CancelCooldown() override;

	virtual void UpdateCooldown() override;

	// --------------------------------------
	//  Pseudo Constants : Default values that are not supposed to be modified
	// --------------------------------------

	/** Duration of this skill in case it doesn't have any animation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	float FailSafeDuration;

	/**
	 * A map of weapons supported by this skill to their animation montages for male version of character
	 * @note If the character doesn't have any specified gender, then this will the default variable to access animation from
	 * @note This only applies to characters that can equip a custom weapon
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TMap<EWeaponType, TSoftObjectPtr<UAnimMontage>> WeaponToMaleAnimationMontageMap;

	/**
	 * A map of weapons supported by this skill to their animation montages for female version of character
	 * @note This only applies to characters that can equip a custom weapon
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TMap<EWeaponType, TSoftObjectPtr<UAnimMontage>> WeaponToFemaleAnimationMontageMap;

	/** Starting section of skill animation inside animation montages */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FName AnimationStartSectionName;

	/** The camera to play when this skill hits an enemy */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Effects")
	TSoftClassPtr<UCameraShake> CameraShakeOnHitSoftClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Level Up Info")
	TArray<FActiveSkillLevelUpInfo> SkillLevelUpsInfo;

	// --------------------------------------
	//  Cache
	// --------------------------------------

	TSharedPtr<FAttackInfo> AttackInfoPtr;

	UPROPERTY(Transient)
	TMap<EWeaponType, UAnimMontage*> SkillAnimations;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	virtual void LoadFemaleAnimations();

	virtual void LoadMaleAnimations();
	
	UFUNCTION()
	virtual void OnFemaleAnimationsLoaded();

	UFUNCTION()
	virtual void OnMaleAnimationsLoaded();

	FTimerHandle SkillTimerHandle;

};

inline FActiveSkillLevelUpInfo UActiveSkillBase::GetCurrentSkillLevelupInfo() const
{
	if (SkillLevelUpsInfo.Num() > CurrentUpgrade)
	{
		return SkillLevelUpsInfo[CurrentUpgrade];
	}
	else if (SkillLevelUpsInfo.Num() > 0)
	{
		return SkillLevelUpsInfo[0];
	}
	else
	{
		return FActiveSkillLevelUpInfo();
	}
}
