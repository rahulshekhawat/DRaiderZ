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

	/** Bottom of the capsule */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ShapeInfo)
	FVector Bottom;

	/** Top of the capsule */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ShapeInfo)
	FVector Top;

	/** Capsule radius */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ShapeInfo)
	float Radius;

	FORCEINLINE bool operator == (const FRaidCapsule& Other) const
	{
		return (this->Bottom == Other.Bottom && this->Top == Other.Top && this->Radius == Other.Radius);
	}

};

/**
 * An anim notify class to handle collisions of RaiderZ format
 */
UCLASS()
class EOD_API UAnimNotify_RaidCollision : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	/** Capsules that will be used for doing collision tests */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CollisionInfo)
	TArray<FRaidCapsule> CollisionCapsules;	
	
};
