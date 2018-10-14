// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "DragVisualWidget.h"

UDragVisualWidget::UDragVisualWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDragVisualWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UDragVisualWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDragVisualWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
