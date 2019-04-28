// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ActionConfirmationWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

UActionConfirmationWidget::UActionConfirmationWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConfirmationText = FText::FromString(FString("Do you want to apply and save these changes?"));
}

bool UActionConfirmationWidget::Initialize()
{
	if (Super::Initialize() &&
		ConfirmationTextBlock &&
		YesButton &&
		NoButton)
	{
		ConfirmationTextBlock->SetText(ConfirmationText);

		YesButton->OnClicked.AddDynamic(this, &UActionConfirmationWidget::HandleYesButtonClicked);
		NoButton->OnClicked.AddDynamic(this, &UActionConfirmationWidget::HandleNoButtonClicked);

		return true;
	}
	return false;
}

void UActionConfirmationWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UActionConfirmationWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UActionConfirmationWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UActionConfirmationWidget::HandleYesButtonClicked()
{
	OnYesButtonClicked.Broadcast();
	RemoveFromParent();
}

void UActionConfirmationWidget::HandleNoButtonClicked()
{
	OnNoButtonClicked.Broadcast();
	RemoveFromParent();
}
