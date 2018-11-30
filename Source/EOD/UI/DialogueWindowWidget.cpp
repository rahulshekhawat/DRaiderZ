// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "DialogueWindowWidget.h"

UDialogueWindowWidget::UDialogueWindowWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultDialogueText = NSLOCTEXT("eod-namespace", "eng-us", "DEFAULT TEXT\n\n<IF YOU ARE READING THIS THEN ACTUAL DIALOGUE TEXT IS MISSING>");

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
