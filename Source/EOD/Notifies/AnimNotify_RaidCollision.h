// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_RaidCollision.generated.h"

USTRUCT(BlueprintType)
struct FCapsuleInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere)
	FVector Bottom;
	
	UPROPERTY(EditAnywhere)
	FVector Top;
	
	UPROPERTY(EditAnywhere)
	float Radius;

	FORCEINLINE bool operator == (const FCapsuleInfo& Other) const
	{
		return (this->Bottom == Other.Bottom && this->Top == Other.Top && this->Radius == Other.Radius);
	}

};

/**
 * AnimNotify_RaidCollision performs capsule collisions using the capsules created from
 * RaiderZ collision files.
 */
UCLASS()
class EOD_API UAnimNotify_RaidCollision : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	/** Capsules that will be used for doing collision tests */
	UPROPERTY(EditAnywhere, Category = CollisionInfo)
	TArray<FCapsuleInfo> CollisionCapsules;	
	
};
