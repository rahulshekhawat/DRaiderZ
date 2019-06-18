// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatLibrary.h"
#include "GameplaySkillBase.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information")
	FCCEffectInfo CCEffectInfo;

	/** Immunities from crowd control effects granted on using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information", meta = (Bitmask, BitmaskEnum = "ECrowdControlEffect"))
	uint8 AcquiredCCImmunities;

	/** The duration before which the same skill can be used again */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information")
	float Cooldown;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	FGameplayEffectInfo GameplayEffectInfo;

	FAISkillInfo() :
		DamagePercent(0.f),
		bUnblockable(false),
		bUndodgable(false),
		bIgnoresBlock(false),
		CCEffectInfo(),
		AcquiredCCImmunities(0),
		Cooldown(0.f),
		GameplayEffectInfo()
	{
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

	virtual TSharedPtr<FAttackInfo> GetAttackInfoPtr(int32 CollisionIndex = 1) override;

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* SkillMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FName AnimationStartSectionName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Effects")
	ECameraShakeType CamShakeType;

	/** The camera to play when this skill hits an enemy */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Effects")
	TSoftClassPtr<UCameraShake> CameraShakeOnHitSoftClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Info")
	FAISkillInfo SkillInfo;

	float SkillDuration;

};
