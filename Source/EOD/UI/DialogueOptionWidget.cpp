// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "DialogueOptionWidget.h"

UDialogueOptionWidget::UDialogueOptionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDialogueOptionWidget::Initialize()
{
	if (Super::Initialize() && OptionButton && OptionText)
	{
		return true;
	}

	return false;
}

void UDialogueOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDialogueOptionWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
