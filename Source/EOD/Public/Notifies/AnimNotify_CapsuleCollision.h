// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatLibrary.h"
#include "AnimNotify_RaidCollision.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CapsuleCollision.generated.h"

/** A struct to hold capsule information */
USTRUCT(BlueprintType)
struct FEODCapsule
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Center;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Radius;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HalfHeight;

	FORCEINLINE bool operator==(const FEODCapsule& Other) const
	{
		return (this->Center == Other.Center && this->Rotation == Other.Rotation && this->Radius == Other.Radius && this->HalfHeight == Other.HalfHeight);
	}

	FORCEINLINE bool operator!=(const FEODCapsule& Other) const
	{
		return this->Center != Other.Center || this->Rotation != Other.Rotation || this->Radius != Other.Radius || this->HalfHeight != Other.HalfHeight;
	}
};

/**
 * 
 */
UCLASS()
class EOD_API UAnimNotify_CapsuleCollision : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	void InitializeFromRaidCapsules(const TArray<FRaidCapsule>& RaidCapsules);
	bool HasRaidCapsules(const TArray<FRaidCapsule>& RaidCapsules);

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CapsuleInfo)
	TArray<FEODCapsule> CollisionCapsules;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillInfo)
	FCollisionSkillInfo SkillInfo;

};
