// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatLibrary.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SphereCollision.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UAnimNotify_SphereCollision : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
	/** Position of collision sphere's center */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SphereInfo)
	FVector Center;
	
	/** Radius of collision sphere */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SphereInfo)
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillInfo)
	FCollisionSkillInfo SkillInfo;
	
};
