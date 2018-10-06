// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "CombatManager.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API ACombatManager : public AInfo
{
	GENERATED_BODY()
	
public:

	ACombatManager(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void OnMeleeHit(const AActor* Instigator, const bool bHit, const TArray<FHitResult>& HitResults);
	
	// @todo OnRangedHit
	
};
