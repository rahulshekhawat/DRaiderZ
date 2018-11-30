// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InteractionInterface.h"

UInteractionInterface::UInteractionInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void IInteractionInterface::OnInteract_Implementation(const AEODCharacterBase* Character, UUserWidget* DialogueWidget)
{
	// empty defintion
}

void IInteractionInterface::EnableCustomDepth_Implementation()
{
	// empty defintion
}

void IInteractionInterface::DisableCustomDepth_Implementation()
{
	// empty defintion
}
