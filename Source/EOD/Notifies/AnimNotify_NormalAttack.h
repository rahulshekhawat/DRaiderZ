// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_NormalAttack.generated.h"

/**
 * Notify called at the beginning of a normal attack animation.
 * This is used to set current active skill for owning player and set off smooth rotation.
 */
UCLASS()
class EOD_API UAnimNotify_NormalAttack : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RequiredInfo)
	FName SectionName;
	
};
