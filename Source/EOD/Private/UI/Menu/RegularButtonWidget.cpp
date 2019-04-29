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
		DisplayText->SetText(TextToDisplay);
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
	check(PrimaryButton);
	if (bIsLocked == bInLocked)
	{
		return;
	}

	bIsLocked = bInLocked;
	bool bIsHovered = PrimaryButton->IsHovered();
	if (bIsHovered)
	{
		if (bIsLocked)
		{
			RootBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
			PrimaryButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		else
		{
			RootBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			PrimaryButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
	}
	else
	{
		if (bIsLocked)
		{
			RootBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
			PrimaryButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		else
		{
			RootBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
			PrimaryButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
			PlayUnhoveredAnimation();
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
	PlayHoveredAnimation();
}

void URegularButtonWidget::OnPrimaryButtonUnhovered()
{
	OnUnhovered.Broadcast();
	PlayUnhoveredAnimation();
}

void URegularButtonWidget::PlayHoveredAnimation_Implementation()
{
}

void URegularButtonWidget::PlayUnhoveredAnimation_Implementation()
{
}
