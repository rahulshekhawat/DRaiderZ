// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "DialogueWindowWidget.h"

UDialogueWindowWidget::UDialogueWindowWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDialogueWindowWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UDialogueWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDialogueWindowWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
