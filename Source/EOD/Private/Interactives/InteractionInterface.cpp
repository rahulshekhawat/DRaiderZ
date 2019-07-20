// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "InteractionInterface.h"
#include "EODCharacterBase.h"

UInteractionInterface::UInteractionInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

EInteractionResult IInteractionInterface::OnInteractionStart_Implementation(AEODCharacterBase* Character)
{
	if (Character)
	{
		Character->SetCharacterStateInfo(ECharacterState::Interacting, 0, true);
		Character->SetCharacterStateAllowsMovement(false);
		Character->SetCharacterStateAllowsRotation(false);

		return EInteractionResult::InteractionUpdated;
	}

	return EInteractionResult::InteractionRequestFailed;
}

EInteractionResult IInteractionInterface::OnInteractionUpdate_Implementation(AEODCharacterBase* Character)
{
	// empty defintion
	return EInteractionResult();
}

void IInteractionInterface::OnInteractionCancel_Implementation(AEODCharacterBase* Character, EInteractionCancelType CancelType)
{
	if (Character && Character->IsInteracting())
	{
		Character->ResetState();
	}
}

void IInteractionInterface::OnInteractionFinish_Implementation(AEODCharacterBase* Character)
{
}

void IInteractionInterface::OnBeginOverlap_Implementation(AEODCharacterBase* Character)
{
}

void IInteractionInterface::OnEndOverlap_Implementation(AEODCharacterBase* Character)
{
	// empty defintion
}

void IInteractionInterface::OnGainFocus_Implementation(AEODCharacterBase* Character)
{
	// empty defintion
}

void IInteractionInterface::OnLoseFocus_Implementation(AEODCharacterBase* Character)
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
