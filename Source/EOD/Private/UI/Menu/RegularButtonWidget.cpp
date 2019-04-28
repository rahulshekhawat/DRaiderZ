// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "RegularButtonWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

URegularButtonWidget::URegularButtonWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TextToDisplay = FText::FromString(FString("Regular Buttom"));
}

bool URegularButtonWidget::Initialize()
{
	if (Super::Initialize() &&
		DisplayText &&
		PrimaryButton)
	{
		DisplayText->SetText(TextToDisplay);
		PrimaryButton->OnClicked.AddDynamic(this, &URegularButtonWidget::OnPrimaryButtonClicked);
		PrimaryButton->OnHovered.AddDynamic(this, &URegularButtonWidget::OnPrimaryButtonHovered);
		PrimaryButton->OnUnhovered.AddDynamic(this, &URegularButtonWidget::OnPrimaryButtonUnhovered);
		return true;
	}

	return false;
}

void URegularButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (DisplayText)
	{
		// DisplayText->SetText(TextToDisplay);
	}
}

void URegularButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URegularButtonWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void URegularButtonWidget::LockButton(bool bInLocked)
{
	if (bIsLocked != bInLocked)
	{
		bIsLocked = bInLocked;
		if (bIsLocked)
		{
			RootBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
		}
		else
		{
			PrimaryButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
		}
	}
}

void URegularButtonWidget::OnPrimaryButtonClicked()
{
	OnClicked.Broadcast();
}

void URegularButtonWidget::OnPrimaryButtonHovered()
{
	OnHovered.Broadcast();
	StartHovered();
}

void URegularButtonWidget::OnPrimaryButtonUnhovered()
{
	OnUnhovered.Broadcast();
	StartUnhovered();
}

void URegularButtonWidget::StartHovered_Implementation()
{
}

void URegularButtonWidget::StartUnhovered_Implementation()
{
}
