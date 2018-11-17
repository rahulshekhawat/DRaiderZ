// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "NPCBase.h"

ANPCBase::ANPCBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ANPCBase::OnInteract_Implementation(const AEODCharacterBase* Character)
{
}

void ANPCBase::EnableCustomDepth_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
}

void ANPCBase::DisableCustomDepth_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
}
