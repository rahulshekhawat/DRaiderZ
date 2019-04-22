// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/GameplaySkillBase.h"
#include "AISkillBase.generated.h"

class UCameraShake;
class UGameplayEffectBase;

USTRUCT(BlueprintType, DisplayName = "AI Skill Info")
struct EOD_API FAISkillInfo
{
	GENERATED_USTRUCT_BODY()
		
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
	TSoftClassPtr<UGameplayEffectBase> GameplayEffectSoftClass;

	FAISkillInfo()
	{
		DamagePercent = 0.f;
		bUnblockable = false;
		bUndodgable = false;
		bIgnoresBlock = false;
		CrowdControlEffect = ECrowdControlEffect::Flinch;
		CrowdControlEffectDuration = 0.f;
		CrowdControlImmunities = 0;
		Cooldown = 0.f;
		GameplayEffectSoftClass = NULL;
	}
};

/**
 * 
 */
UCLASS()
class EOD_API UAISkillBase : public UGameplaySkillBase
{
	GENERATED_BODY()

public:

	UAISkillBase(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//	Gameplay Skill Interface
	// --------------------------------------

	virtual bool CanTriggerSkill() const override;

	/** Trigger this skill, i.e., either instantly activate this skill or start charging this skill. */
	virtual void TriggerSkill() override;

	virtual bool CanReleaseSkill() const override;

	virtual void FinishSkill() override;

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* SkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FName AnimationStartSectionName;

	/** The camera to play when this skill hits an enemy */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Effects")
	TSoftClassPtr<UCameraShake> CameraShakeOnHitSoftClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Info")
	FAISkillInfo SkillInfo;

	float SkillDuration;

};
