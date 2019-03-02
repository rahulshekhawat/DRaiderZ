// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CharacterStateBase.h"

UCharacterStateBase::UCharacterStateBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bAutoFinish = false;

}

void UCharacterStateBase::OnEnterState()
{
	//~ empty
}

void UCharacterStateBase::OnLeaveState()
{
	//~ empty
}

void UCharacterStateBase::OnInterruptState()
{
	//~ empty
}
