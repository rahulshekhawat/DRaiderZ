// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatsComponentBase.h"
#include "AIStatsComponent.generated.h"

class AAICharacterBase;

/**
 * AIStatsComponent is used to manage stats of an AI controlled character
 */
UCLASS()
class EOD_API UAIStatsComponent : public UStatsComponentBase
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UAIStatsComponent(const FObjectInitializer& ObjectInitializer);

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	/** Dummy declaration. This component doesn't tick */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Primary Info")
	FName StatsRowName;

private:

	void LoadAIStats();
	
};
