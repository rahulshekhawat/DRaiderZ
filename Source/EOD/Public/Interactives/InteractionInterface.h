// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class UUserWidget;
class AEODCharacterBase;

UENUM(BlueprintType)
enum class EInteractionResult : uint8
{
	InteractionRequestFailed,
	InteractionUpdated, // And it's not over yet
	InteractionCancelled,
	InteractionExitedByPlayer,
	InteractionFinished
};

UENUM(BlueprintType)
enum class EInteractionCancelType : uint8
{
	ManualCancel,
	AutoCancel
};


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

	/** This event is called when a (player) character starts interacting with the interactive actor */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InGameInteraction)
	EInteractionResult OnInteractionStart(AEODCharacterBase* Character);
	virtual EInteractionResult OnInteractionStart_Implementation(AEODCharacterBase* Character);

	/** This event is called when a (player) character wants to update the current interaction with the interactive actor */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InGameInteraction)
	EInteractionResult OnInteractionUpdate(AEODCharacterBase* Character);
	virtual EInteractionResult OnInteractionUpdate_Implementation(AEODCharacterBase* Character);

	/** This event is called when a (player) character cancels interaction with the interactive actor */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InGameInteraction)
	void OnInteractionCancel(AEODCharacterBase* Character, EInteractionCancelType CancelType);
	virtual void OnInteractionCancel_Implementation(AEODCharacterBase* Character, EInteractionCancelType CancelType);

	/** This event is called when a (player) character begins overlap with the underlying actor */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InGameInteraction)
	void OnBeginOverlap(AEODCharacterBase* Character);
	virtual void OnBeginOverlap_Implementation(AEODCharacterBase* Character);
	
	/** This event is called when a (player) character ends overlap with the underlying actor */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InGameInteraction)
	void OnEndOverlap(AEODCharacterBase* Character);
	virtual void OnEndOverlap_Implementation(AEODCharacterBase* Character);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InGameInteraction)
	void OnGainFocus(AEODCharacterBase* Character);
	virtual void OnGainFocus_Implementation(AEODCharacterBase* Character);
		
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InGameInteraction)
	void OnLoseFocus(AEODCharacterBase* Character);
	virtual void OnLoseFocus_Implementation(AEODCharacterBase* Character);

	/** This event is called to highlight interactive actor's meshes */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InGameInteraction)
	void EnableCustomDepth();
	virtual void EnableCustomDepth_Implementation();

	/** This event is called to disable highlight on interactive actor's meshes */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InGameInteraction)
	void DisableCustomDepth();
	virtual void DisableCustomDepth_Implementation();

};
