// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FootStepBase.generated.h"

class USoundBase;
class USoundAttenuation;

/**
 * 
 */
UCLASS(Abstract, Blueprintable, hidecategories = Object, collapsecategories)
class EOD_API UAnimNotify_FootStepBase : public UAnimNotify
{
	GENERATED_BODY()

public:

	// virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	/** Determines if this notify is called when character's left foot hits the ground */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FootNotify)
	bool bLeftFoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* DefaultLeftFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* DefaultRightFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* StoneLeftFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* StoneRightFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* ConcreteLeftFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* ConcreteRightFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* DirtLeftFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* DirtRightFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* MetalLeftFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* MetalRightFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* FoliageLeftFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* FoliageRightFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* SandLeftFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* SandRightFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* SnowLeftFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* SnowRightFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* WaterLeftFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* WaterRightFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* WoodLeftFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* WoodRightFootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundAttenuation* SoundAttenuation;

	UFUNCTION(BlueprintCallable, Category = FootNotify)
	void GetFootstepHitResults(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, FHitResult& OutHitResults);

	UFUNCTION(BlueprintCallable, Category = FootNotify)
	void PlayFootstepSound(const FHitResult& HitResult);

};
