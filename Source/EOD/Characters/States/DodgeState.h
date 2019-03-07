// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/States/CharacterStateBase.h"
#include "DodgeState.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UDodgeState : public UCharacterStateBase
{
	GENERATED_BODY()

public:
	UDodgeState(const FObjectInitializer& ObjectInitializer);

	/** Returns true if character can enter this state right now */
	virtual bool CanEnterState();

	/** Event called when character enters this state */
	virtual void OnEnterState();


};
