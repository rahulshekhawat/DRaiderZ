// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_EODTrail.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class EOD_API UAnimNotifyState_EODTrail : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = TrailInfo)
	FString Socket_SwordTrailStart;

	UPROPERTY(EditDefaultsOnly, Category = TrailInfo)
	FString Socket_SwordTrailEnd;
	
	UPROPERTY(EditDefaultsOnly, Category = TrailInfo)
	UParticleSystem* PSWeaponTrail;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
