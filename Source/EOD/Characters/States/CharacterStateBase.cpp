// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CharacterStateBase.h"

UCharacterStateBase::UCharacterStateBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bAutoFinish = false;

}

void UCharacterStateBase::InitState(AEODCharacterBase* NewOwner)
{
	StateOwner = NewOwner;
}

bool UCharacterStateBase::CanEnterState()
{
	return false;
}

bool UCharacterStateBase::CanLeaveState()
{
	return false;
}

bool UCharacterStateBase::CanInterruptState()
{
	return false;
}

bool UCharacterStateBase::IsStateInterruptible() const
{
	return false;
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

void UCharacterStateBase::UpdateState(float DeltaTime)
{
}
