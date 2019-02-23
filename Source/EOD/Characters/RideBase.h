// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/EODCharacterBase.h"
#include "RideBase.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API ARideBase : public AEODCharacterBase
{
	GENERATED_BODY()
	
public:
	ARideBase(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Updates character state every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called when the Pawn is being restarted (usually by being possessed by a Controller). Called on both server and owning client. */
	virtual void Restart() override;

	////////////////////////////////////////////////////////////////////////////////
	// Ride System
public:

protected:
	/** Determines whether this ride can fly */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ride System")
	uint32 bCanFly : 1;

private:


	////////////////////////////////////////////////////////////////////////////////
	// Movement
public:
	/** Move character forward/backward */
	virtual void MoveForward(const float Value) override;

	/** Move character left/right */
	virtual void MoveRight(const float Value) override;

protected:
	/** Updates the rotation of character */
	virtual void UpdateRotation(float DeltaTime) override;


};
