// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EODAIControllerBase.generated.h"

class UAIStatsComponent;

/**
 * 
 */
UCLASS()
class EOD_API AEODAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AEODAIControllerBase(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// --------------------------------------
	//  Components
	// --------------------------------------

	static const FName StatsComponentName;

	FORCEINLINE UAIStatsComponent* GetStatsComponent() const { return StatsComponent; }

private:

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAIStatsComponent* StatsComponent;




};
