// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class UUserWidget;
class AEODCharacterBase;

/** An interface that must be implemented for all in-game interactive actors */
UINTERFACE(BlueprintType)
class EOD_API UInteractionInterface : public UInterface
{
	GENERATED_BODY()

public:

    UInteractionInterface(const FObjectInitializer& ObjectInitializer);

};

/** An interface that must be implemented for all in-game interactive actors */
class EOD_API IInteractionInterface
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  Interaction Interface
	// --------------------------------------

	/** This event is called when a (player) character interacts with the underlying actor */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InGameInteraction)
	void OnInteract(AEODCharacterBase* Character);
	virtual void OnInteract_Implementation(AEODCharacterBase* Character);

	/** This event is called to highlight interactive actor's meshes */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InGameInteraction)
	void EnableCustomDepth();
	virtual void EnableCustomDepth_Implementation();

	/** This event is called to disable highlight on interactive actor's meshes */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InGameInteraction)
	void DisableCustomDepth();
	virtual void DisableCustomDepth_Implementation();

};
