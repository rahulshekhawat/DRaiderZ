// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "FooterButtonWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UFooterButtonWidget::UFooterButtonWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DisplayText = FText::FromString(FString("[ENTER] SELECT"));
}

bool UFooterButtonWidget::Initialize()
{
	if (Super::Initialize() &&
		DisplayTextBlock)
	{
		DisplayTextBlock->SetText(DisplayText);
		FooterButton->OnHovered.AddDynamic(this, &UFooterButtonWidget::HandleFootButtonHovered);
		FooterButton->OnUnhovered.AddDynamic(this, &UFooterButtonWidget::HandleFootButtonUnhovered);

		return true;
	}
	return false;
}

void UFooterButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFooterButtonWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UFooterButtonWidget::HandleFootButtonHovered()
{
}

void UFooterButtonWidget::HandleFootButtonUnhovered()
{
}
