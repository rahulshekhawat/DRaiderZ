// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "FloatingHealthBarWidget.h"

#include "Components/ProgressBar.h"

UFloatingHealthBarWidget::UFloatingHealthBarWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UFloatingHealthBarWidget::Initialize()
{
	if (Super::Initialize() && HealthBar)
	{
		return true;
	}

	return false;
}

void UFloatingHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFloatingHealthBarWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UFloatingHealthBarWidget::UpdateHealth(int32 BaseHealth, int32 MaxHealth, int32 CurrentHealth)
{
	check(HealthBar);
	if (MaxHealth > 0)
	{
		float Percent = float(CurrentHealth) / float(MaxHealth);
		HealthBar->SetPercent(Percent);
	}
	else
	{
		HealthBar->SetPercent(1.f);
	}
}
