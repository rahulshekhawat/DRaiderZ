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
		FooterButton->OnClicked.AddDynamic(this, &UFooterButtonWidget::HandleFootButtonClicked);
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

void UFooterButtonWidget::HandleFootButtonClicked()
{
	OnClicked.Broadcast();
}

void UFooterButtonWidget::HandleFootButtonHovered()
{
	if (RootBorder)
	{
		RootBorder->SetContentColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UFooterButtonWidget::HandleFootButtonUnhovered()
{
	if (RootBorder)
	{
		RootBorder->SetContentColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
	}
}
