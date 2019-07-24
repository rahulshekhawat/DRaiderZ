// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "StatusEffectWidget.h"

#include "Components/Image.h"
#include "Components/Border.h"
#include "Materials/MaterialInstanceDynamic.h"

UStatusEffectWidget::UStatusEffectWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UStatusEffectWidget::Initialize()
{
	if (Super::Initialize() &&
		RootBorder &&
		StatusEffectIcon)
	{
		RootMID = RootBorder->GetDynamicMaterial();
		RootMID->SetScalarParameterValue(TEXT("Alpha"), 1.f);

		return true;
	}

	return false;
}

void UStatusEffectWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStatusEffectWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UStatusEffectWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (RootMID)
	{
		float CurrentAlpha = 0.f;
		bool bParamFound = RootMID->GetScalarParameterValue(TEXT("Alpha"), CurrentAlpha);
		float DeltaAlpha = InDeltaTime / Duration;

		RootMID->SetScalarParameterValue(TEXT("Alpha"), (CurrentAlpha - DeltaAlpha));
	}	
}

void UStatusEffectWidget::SetDuration(float InValue)
{
	Duration = InValue;
}

void UStatusEffectWidget::SetIcon(UTexture* InIcon)
{
	if (InIcon)
	{
		FSlateBrush SlateBrush;
		SlateBrush.ImageSize = FVector2D(56.0, 56);
		SlateBrush.DrawAs = ESlateBrushDrawType::Image;
		SlateBrush.ImageType = ESlateBrushImageType::FullColor;
		SlateBrush.SetResourceObject(InIcon);
		StatusEffectIcon->SetBrush(SlateBrush);
	}
	// If the NewIcon is NULL, we simply remove the previously set image inside ItemImage
	else
	{
		FSlateBrush SlateBrush;
		SlateBrush.ImageSize = FVector2D(56.0, 56);
		SlateBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
		SlateBrush.ImageType = ESlateBrushImageType::NoImage;
		StatusEffectIcon->SetBrush(SlateBrush);
	}
}
