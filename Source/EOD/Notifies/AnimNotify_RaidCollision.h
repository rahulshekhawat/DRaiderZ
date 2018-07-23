// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_RaidCollision.generated.h"

/** A struct to hold capsule information of RaiderZ format */
USTRUCT(BlueprintType)
struct FRaidCapsule
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere)
	FVector Bottom;
	
	UPROPERTY(EditAnywhere)
	FVector Top;
	
	UPROPERTY(EditAnywhere)
	float Radius;

	FORCEINLINE bool operator == (const FRaidCapsule& Other) const
	{
		return (this->Bottom == Other.Bottom && this->Top == Other.Top && this->Radius == Other.Radius);
	}

};

/**
 * An anim notify class to handle collisions of RaiderZ's format
 */
UCLASS()
class EOD_API UAnimNotify_RaidCollision : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	/** Capsules that will be used for doing collision tests */
	UPROPERTY(EditAnywhere, Category = CollisionInfo)
	TArray<FRaidCapsule> CollisionCapsules;	
	
};
