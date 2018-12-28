// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CapsuleCollision.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UAnimNotify_CapsuleCollision : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	/** Position of collision capsule's center */
	UPROPERTY(EditAnywhere, Category = CapsuleInfo)
	FVector Center;
	
	/** Rotation of collision capsule */
	UPROPERTY(EditAnywhere, Category = CapsuleInfo)
	FRotator Rotation;
	
	/** Radius of collision capsule */
	UPROPERTY(EditAnywhere, Category = CapsuleInfo)
	float Radius;
	
	/** Half height of collision capsule */
	UPROPERTY(EditAnywhere, Category = CapsuleInfo)
	float HalfHeight;
	
	
};
