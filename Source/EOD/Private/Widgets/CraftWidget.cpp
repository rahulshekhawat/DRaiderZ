// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "CraftWidget.h"

UCraftWidget::UCraftWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UCraftWidget::Initialize()
{
	if (Super::Initialize() &&
		LeftPanel &&
		RightPanel &&
		ItemsList &&
		CraftButton)
	{
		return true;
	}

	return false;
}

void UCraftWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCraftWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
