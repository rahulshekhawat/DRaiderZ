// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "FrameRateSubWidget.h"
#include "ScrollButtonWidget.h"

UFrameRateSubWidget::UFrameRateSubWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UFrameRateSubWidget::Initialize()
{
	if (Super::Initialize() &&
		F30 &&
		F60 &&
		F90 &&
		F120 &&
		Unlimited)
	{
		return true;
	}

	return false;
}

void UFrameRateSubWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFrameRateSubWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UFrameRateSubWidget::SetParentOptionsWidget(UVideoOptionsWidget* NewParent)
{
	ParentOptionsWidget = NewParent;
}

void UFrameRateSubWidget::HandleF30Clicked()
{
	OnFrameRateSelected.Broadcast(30.f);
}

void UFrameRateSubWidget::HandleF60Clicked()
{
	OnFrameRateSelected.Broadcast(60.f);
}

void UFrameRateSubWidget::HandleF90Clicked()
{
	OnFrameRateSelected.Broadcast(90.f);
}

void UFrameRateSubWidget::HandleF120Clicked()
{
	OnFrameRateSelected.Broadcast(120.f);
}

void UFrameRateSubWidget::HandleUnlimitedClicked()
{
	OnFrameRateSelected.Broadcast(0.f);
}
