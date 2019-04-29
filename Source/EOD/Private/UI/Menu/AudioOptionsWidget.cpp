// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "AudioOptionsWidget.h"

UAudioOptionsWidget::UAudioOptionsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UAudioOptionsWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UAudioOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAudioOptionsWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
