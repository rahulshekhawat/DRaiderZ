// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "NotificationWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

UNotificationWidget::UNotificationWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UNotificationWidget::Initialize()
{
	if (Super::Initialize() &&
		NotificationIcon &&
		NotificationText)
	{
		return true;
	}
	return false;
}

void UNotificationWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UNotificationWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UNotificationWidget::DeleteNotification_Implementation()
{
	this->RemoveFromParent();
}
