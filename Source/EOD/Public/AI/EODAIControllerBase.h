// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EODAIControllerBase.generated.h"

class UAIStatsComponent;
class UBlackboardComponent;
class AAICharacterBase;

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

	virtual void SetPawn(APawn* InPawn) override;

	// --------------------------------------
	//  Components
	// --------------------------------------

	static const FName StatsComponentName;

	FORCEINLINE UAIStatsComponent* GetStatsComponent() const { return StatsComponent; }

protected:

	UPROPERTY(Category = Stats, VisibleAnywhere, BlueprintReadOnly)
	UAIStatsComponent* StatsComponent;

	// --------------------------------------
	//  Blackboard and Behaviour Tree
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	virtual void InitializeBlackboardValues(UBlackboardComponent* BlackboardComponent);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard Values")
	float AggroActivationRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard Values")
	float AggroAreaRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard Values")
	float MaxEnemyChaseRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard Values")
	float WanderRadius;

public:

	// --------------------------------------
	//  Pawn
	// --------------------------------------

	FORCEINLINE AAICharacterBase* GetAICharacter() const { return AICharacter; }

private:

	UPROPERTY(Transient)
	AAICharacterBase* AICharacter;

};
