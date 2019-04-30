// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ResolutionButtonWidget.h"

UResolutionButtonWidget::UResolutionButtonWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UResolutionButtonWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UResolutionButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UResolutionButtonWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UResolutionButtonWidget::OnMainButtonClicked()
{
	Super::OnMainButtonClicked();
	OnClicked_ResolutionPayload.Broadcast(ButtonResolution);
}
