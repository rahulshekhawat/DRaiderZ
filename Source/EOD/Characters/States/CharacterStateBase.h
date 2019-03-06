// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterStateBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class EOD_API UCharacterStateBase : public UObject
{
	GENERATED_BODY()
	
public:
	UCharacterStateBase(const FObjectInitializer& ObjectInitializer);

	/** Returns true if character can enter this state right now */
	virtual bool CanEnterState();

	/** Returns true if the character can leave this state right now */
	virtual bool CanLeaveState();

	/** Returns true if this state can be interrupted right now */
	virtual bool CanInterruptState();

	/** Returns true if this state can be interrupted at all */
	virtual bool IsStateInterruptible() const;

	/** Event called when character enters this state */
	virtual void OnEnterState();

	/** Event called when character leaves this state */
	virtual void OnLeaveState();

	/** Event called when this state is interrupted */
	virtual void OnInterruptState();

	/** Update this state */
	virtual void UpdateState(float DeltaTime);

protected:
	/** Determines whether this state finishes automatically */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Behaviour")
	uint32 bAutoFinish : 1;

	/** Determines whether this state should force the transition from previous state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Behaviour")
	uint32 bForceStateChange : 1;

	/** Determines whether character can rotate while in this state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rotation")
	uint32 bAllowsRotation : 1;

	/** Determines whether this state uses controller rotation yaw */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rotation")
	uint32 bUseControllerRotationYaw : 1;

	/** Determines whether character can move while in this state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	uint32 bAllowsMovement : 1;




};
