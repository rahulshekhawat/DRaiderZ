// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "VocationInfoWidget.h"

#include "Components/TextBlock.h"

UVocationInfoWidget::UVocationInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WeaponsList = FString("Warhammer,\nGreatsword");
	CharacteristicsList = FString("High Strength,\nHigh Physical Damage");
}

bool UVocationInfoWidget::Initialize()
{
	if (Super::Initialize() &&
		WeaponsListText &&
		CharacteristicsListText)
	{
		WeaponsListText->SetText(FText::FromString(WeaponsList));
		CharacteristicsListText->SetText(FText::FromString(CharacteristicsList));

		return true;
	}

	return false;
}

void UVocationInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVocationInfoWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
