// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"

#include "Engine/StreamableManager.h"
#include "Gameplay/Skills/GameplaySkillBase.h"
#include "ActiveSkillBase.generated.h"

class UStatusEffectBase;


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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Effects")
	TSoftClassPtr<UStatusEffectBase> StatusEffectSoftClass;


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
		StatusEffectSoftClass = NULL;
	}

};

/**
 * Base class for all active skills
 */
UCLASS()
class EOD_API UActiveSkillBase : public UGameplaySkillBase
{
	GENERATED_BODY()

public:
	UActiveSkillBase(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//	Gameplay Skill Interface
	// --------------------------------------

	/** Initialize this skill. Intended to be called immediately after the skill object is created */
	virtual void InitSkill(AEODCharacterBase* Instigator, AController* Owner) override;

	/** Reinitialize this skill. Intended to be called when the skill owner changes weapons */
	virtual void Reinitialize() override;

	/** Cancel this skill */
	virtual void CancelSkill() override;

	/** Returns true if this skill can be cancelled */
	virtual bool CanCancelSkill() const override;

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
	
	/**
	 * Weapons that this skill can be used with.
	 * @note This only applies to characters that can equip a custom weapon
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information", meta = (Bitmask, BitmaskEnum = "EWeaponType"))
	uint8 SupportedWeapons;

	/** Type of damage inflicted by this skill if this skill can inflict damage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information")
	EDamageType DamageType;

	/** Describes the effect of this skill. This will be used by AI for determining most suitable skill to use */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information")
	ESkillEffect SkillEffect;

	/** Skills, any of which MUST be used before using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	TArray<FName> PrecedingSkillGroups;

	/** Skill that can be used after using this skill (skill chaining) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	FName SupersedingSkillGroup;

	/** The status effect required to use this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	FGameplayTag RequiredOwnerGameplayTag;

	/** The camera to play when this skill hits an enemy */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Effects")
	TSoftClassPtr<UCameraShake> CameraShakeOnHitSoftClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Level Up Info")
	TArray<FActiveSkillLevelUpInfo> SkillLevelUpsInfo;

protected:

	UPROPERTY(Transient)
	TMap<EWeaponType, UAnimMontage*> SkillAnimations;

	TSharedPtr<FStreamableHandle> AnimationsHandle;

	void LoadFemaleAnimations();

	void LoadMaleAnimations();
	
	void OnFemaleAnimationsLoaded();

	void OnMaleAnimationsLoaded();

	/** Returns true if this skill can be used with the given weapon type */
	bool IsWeaponTypeSupported(EWeaponType WeaponType) const;

	//~ @todo
	/** Returns true if this skill is currently in cooldown */
	bool IsSkillInCooldown() const;

};
