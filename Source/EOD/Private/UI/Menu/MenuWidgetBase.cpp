// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "MenuWidgetBase.h"

UMenuWidgetBase::UMenuWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UMenuWidgetBase::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UMenuWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMenuWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMenuWidgetBase::AddMenuToScreen_Implementation()
{
	AddToViewport();
}

void UMenuWidgetBase::RemoveMenuFromScreen_Implementation()
{
	RemoveFromParent();
}
