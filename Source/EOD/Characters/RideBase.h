// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/EODCharacterBase.h"
#include "RideBase.generated.h"

class UAnimMontage;

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

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Updates character state every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called when the Pawn is being restarted (usually by being possessed by a Controller). Called on both server and owning client. */
	virtual void Restart() override;


	////////////////////////////////////////////////////////////////////////////////
	// Ride System
public:
	/** Reference to character currently mounting this rideable character */
	UPROPERTY(ReplicatedUsing = OnRep_MountedCharacter)
	AEODCharacterBase* MountedCharacter;

	static FName MountSocketName;

	void MountCharacter(AEODCharacterBase* Character);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ride System|Animations")
	UAnimMontage* MountedCharacter_IdealAnimation;		

protected:
	/** Determines whether this ride can fly */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ride System")
	uint32 bCanFly : 1;


	////////////////////////////////////////////////////////////////////////////////
	// Movement
public:
	/** Move character forward/backward */
	virtual void MoveForward(const float Value) override;

	/** Move character left/right */
	virtual void MoveRight(const float Value) override;


	////////////////////////////////////////////////////////////////////////////////
	// Update Character State
protected:
	/** Updates character rotation every frame */
	virtual void UpdateRotation(float DeltaTime) override;


	////////////////////////////////////////////////////////////////////////////////
	// Network
private:
	UFUNCTION()
	void OnRep_MountedCharacter(AEODCharacterBase* OldCharacter);
	

};
