// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Effects/GameplayEffectBase.h"
#include "MovementBuff.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UMovementBuff : public UGameplayEffectBase
{
	GENERATED_BODY()

public:

	UMovementBuff(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//	Gameplay Effect Interface
	// --------------------------------------

	virtual void ActivateEffect_Implementation(int32 ActivationLevel = 1) override;
	virtual void DeactivateEffect_Implementation() override;
	virtual void UpdateEffect_Implementation(float DeltaTime) override;

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	int32 ExtraEffectDurationPerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	int32 MaxUpgradeLevel;

	FTimerHandle MovementEndTimerHandle;

};
