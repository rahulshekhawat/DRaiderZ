// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/GameplaySkillBase.h"
#include "ActiveSkillBase.generated.h"

/**
 * Base class for all active skills
 */
UCLASS()
class EOD_API UActiveSkillBase : public UGameplaySkillBase
{
	GENERATED_BODY()

public:
	UActiveSkillBase(const FObjectInitializer& ObjectInitializer);


protected:

	// --------------------------------------
	//	Constants : Default values that are not supposed to be modified
	// --------------------------------------

	/** Animation montage containing skill animation for male version of character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Base Information")
	TSoftObjectPtr<UAnimMontage> MaleAnimationMontage;

	/** Animation montage containing skill animation for female version of character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Base Information")
	TSoftObjectPtr<UAnimMontage> FemaleAnimationMontage;
	
	/**
	 * A map of weapons supported by this skill to their animation montages for male version of character
	 * @note If the character doesn't have any specified gender, then this will the default variable to access animation from
	 * @note This only applies to characters that can equip a custom weapon
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Base Information")
	TMap<EWeaponType, TSoftObjectPtr<UAnimMontage>> WeaponToMaleAnimationMontageMap;

	/**
	 * A map of weapons supported by this skill to their animation montages for female version of character
	 * @note This only applies to characters that can equip a custom weapon
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Base Information")
	TMap<EWeaponType, TSoftObjectPtr<UAnimMontage>> WeaponToFemaleAnimationMontageMap;

	/** Starting section of skill animation inside animation montages */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Base Information")
	FName AnimationStartSectionName;
	
	/**
	 * Weapons that this skill can be used with.
	 * @note This only applies to characters that can equip a custom weapon
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Base Information", meta = (Bitmask, BitmaskEnum = "EWeaponType"))
	uint8 SupportedWeapons;

	/** Type of damage inflicted by this skill if this skill can inflict damage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	EDamageType DamageType;

	/** Describes the effect of this skill. This will be used by AI for determining most suitable skill to use */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	ESkillEffect SkillEffect;

	/** Stamina cost for using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 StaminaCost;

	/** Mana cost for using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 ManaCost;

	/** Determines whether this skill can be used while moving */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bAllowsMovement;

	/** Determines whether this skill has different animations to be used while walking in different directions */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	// bool bHasDirectionalAnimations;

	/** Determines whether this skill is a passive skill or an active skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bPassiveSkill;

	/** Determines whether this skill can be used while character has been hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bCanBeUsedWhileHit;

	/** Skills, any of which MUST be used before using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	TArray<FString> PrecedingSkillGroups;

	/** Skill that can be used after using this skill (skill chaining) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FString SupersedingSkillGroup;

	/**
	 * SkillID for skill that comes on upgrading this skill.
	 * This will be NAME_None if the skill can't be upgraded any further
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName UpgradeSkillID;

	/**
	 * SkillID for skill from which you can upgrade to this skill.
	 * This will be NAME_None for a base skill
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName DowngradeSkillID;

	/** Current upgrade level of this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 CurrentUpgradeLevel;

	/** Maximum upgrades available for this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 MaxUpgrades;

	/** The status effect required to use this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	FName RequiredStatusID;

	/** The duration before which the same skill can be used again */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float Cooldown;

	/** The duration of loop section if the skill has loop section */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float LoopDuration;

	/** Damage in percentage of player's magickal or physical attack that will be inflicted on enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 DamagePercent;

	/** Determines if this skill can be blocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bUnblockable;

	/** Determines if this skill can be dodged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bUndodgable;

	/** Determines if the 'skill deflected' animation should play on getting blocked. Only applicable if this skill can be blocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bIgnoresBlock;

	/** Crowd control effect on hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	ECrowdControlEffect CrowdControlEffect;

	/** The duration for which the crowd control effect should last (if applicable) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float CrowdControlEffectDuration;

	/** The camera shake type that should play when this skill hits an enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	ECameraShakeType CameraShakeOnHit;

	/** Immunities from crowd control effects granted on using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills, meta = (Bitmask, BitmaskEnum = "ECrowdControlEffect"))
	uint8 CrowdControlImmunities;

	/** Status effect that this skill triggers */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	TSubclassOf<class UStatusEffectBase> StatusEffect;

	
};
