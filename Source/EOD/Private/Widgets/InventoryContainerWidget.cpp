// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "InventoryContainerWidget.h"
#include "InventoryItemBase.h"
#include "EODPlayerController.h"

#include "Components/TextBlock.h"

UInventoryContainerWidget::UInventoryContainerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UInventoryContainerWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void UInventoryContainerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryContainerWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UInventoryContainerWidget::MainButtonClicked()
{
	Super::MainButtonClicked();

	UInventoryItemBase* InvItem = Cast<UInventoryItemBase>(GetDataObj());
	if (InvItem)
	{
		InvItem->OnClick(GetOwningPlayer<AEODPlayerController>());
	}
}

void UInventoryContainerWidget::SetDataObj(UObject* InDataObj)
{
	//~ @todo Unlink widget from old data obj

	UInventoryItemBase* InvItem = Cast<UInventoryItemBase>(InDataObj);
	if (InvItem)
	{
		DataObj = InvItem;

		SetIcon(InvItem->Icon);

		check(SubText);
		if (InvItem->bStackable)
		{
			SubText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			SubText->SetVisibility(ESlateVisibility::Hidden);
		}

		//~ @todo Linkwidget
	}
}

void UInventoryContainerWidget::SetItemCount(int32 InValue)
{
	check(SubText);
	SubText->SetText(FText::FromString(FString::FromInt(InValue)));
}
