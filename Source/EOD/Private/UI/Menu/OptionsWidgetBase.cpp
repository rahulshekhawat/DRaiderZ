// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "OptionsWidgetBase.h"

UOptionsWidgetBase::UOptionsWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UOptionsWidgetBase::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UOptionsWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UOptionsWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UOptionsWidgetBase::CloseDownOptions()
{
}
