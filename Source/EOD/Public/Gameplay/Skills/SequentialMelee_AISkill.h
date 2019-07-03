// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/AISkillBase.h"
#include "SequentialMelee_AISkill.generated.h"

USTRUCT(BlueprintType)
struct EOD_API FSequentialAnimation
{
	GENERATED_USTRUCT_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SectionName;

	FSequentialAnimation() :
		Animation(nullptr),
		SectionName(TEXT("Default"))
	{
	}

};


/**
 * 
 */
UCLASS()
class EOD_API USequentialMelee_AISkill : public UAISkillBase
{
	GENERATED_BODY()

public:

	USequentialMelee_AISkill(const FObjectInitializer& ObjectInitializer);

	/** Trigger this skill, i.e., either instantly activate this skill or start charging this skill. */
	virtual void TriggerSkill() override;

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TArray<FSequentialAnimation> SequentialAnimations;

protected:
	
	UFUNCTION()
	void PlaySequentialAnimation(int32 AnimIndex = 0);

};
