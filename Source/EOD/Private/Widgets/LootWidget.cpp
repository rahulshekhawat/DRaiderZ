// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "LootWidget.h"
#include "LootableInterface.h"
#include "InventoryItemBase.h"
#include "ItemInfoWidget.h"
#include "EODPlayerController.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"

ULootWidget::ULootWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool ULootWidget::Initialize()
{
	if (Super::Initialize() &&
		ItemsList &&
		AcquireButton &&
		AcquireAllButton)
	{
		AcquireButton->OnClicked.AddUniqueDynamic(this, &ULootWidget::OnAcquireButtonClicked);
		AcquireAllButton->OnClicked.AddUniqueDynamic(this, &ULootWidget::OnAcquireAllButtonClicked);

		return true;
	}

	return false;
}

void ULootWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::PlaySound2D(this, UIUpSound);
}

void ULootWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UGameplayStatics::PlaySound2D(this, UIDownSound);
}

bool ULootWidget::InitLootWidget(UObject* InLootSource)
{
	ILootableInterface* LootInterface = Cast<ILootableInterface>(InLootSource);
	if (LootInterface)
	{
		TArray<FGeneratedLootInfo> LootInfoArray = LootInterface->Execute_GetGeneratedLootInfo(InLootSource);
		if (LootInfoArray.Num() <= 0)
		{
			return false;
		}

		SetLootSource(InLootSource);
		GenerateItemList(LootInfoArray);

		return true;
	}

	return false;
}

void ULootWidget::OnAllLootPicked()
{
	AEODPlayerController* PC = GetOwningPlayer<AEODPlayerController>();
	check(PC);

	ILootableInterface* LootInterface = Cast<ILootableInterface>(GetLootSource());
	if (LootInterface)
	{
		LootInterface->Execute_OnAllLootPicked(GetLootSource(), PC);
	}
	else
	{
		PC->RemoveLootWidget(GetLootSource());
	}
}

void ULootWidget::OnLootCancelled()
{
	AEODPlayerController* PC = GetOwningPlayer<AEODPlayerController>();
	check(PC);

	ILootableInterface* LootInterface = Cast<ILootableInterface>(GetLootSource());
	if (LootInterface)
	{
		LootInterface->Execute_OnLootCancelled(GetLootSource(), PC);
	}
	else
	{
		PC->RemoveLootWidget(GetLootSource());
	}
}

void ULootWidget::GenerateItemList(const TArray<FGeneratedLootInfo>& LootInfoArray)
{
	check(ItemInfoWidgetClass.Get());
	APlayerController* PC = GetOwningPlayer();
	for (const FGeneratedLootInfo LootInfo : LootInfoArray)
	{
		if (LootInfo.ItemClass.Get())
		{
			UInventoryItemBase const* const InvItem = Cast<UInventoryItemBase>(LootInfo.ItemClass.Get()->GetDefaultObject());
			check(InvItem);

			UItemInfoWidget* InfoWidget = CreateWidget<UItemInfoWidget>(PC, ItemInfoWidgetClass.Get());
			check(InfoWidget);

			InfoWidget->SetIcon(InvItem->Icon);
			InfoWidget->SetItemName(InvItem->InGameName.ToString());
			
			FString SubTextString = TEXT("x") + FString::FromInt(LootInfo.ItemCount);
			InfoWidget->SetItemSubText(SubTextString);

			ItemsList->AddChild(InfoWidget);
			InfoWidget->OnPressed.AddUniqueDynamic(this, &ULootWidget::OnLootItemPressed);

			LootItemsMap.Add(InfoWidget, LootInfo);
		}
	}

	SelectFirstLootItem();
}

void ULootWidget::SelectFirstLootItem()
{
	if (LootItemsMap.Num() > 0)
	{
		TArray<UItemInfoWidget*> Keys;
		LootItemsMap.GetKeys(Keys);

		UItemInfoWidget* Widget = Keys[0];
		check(Widget);

		SelectLootItem(Widget);
		Widget->SetSelected(true);
	}
}

void ULootWidget::SelectLootItem(UItemInfoWidget* InWidget)
{
	for (TPair<UItemInfoWidget*, FGeneratedLootInfo>& LootItemPair : LootItemsMap)
	{
		check(LootItemPair.Key);
		LootItemPair.Key->SetSelected(false);
	}

	if (InWidget)
	{
		InWidget->SetSelected(true);
	}
}

void ULootWidget::PickAllLoot()
{
	TArray<UItemInfoWidget*> Keys;
	LootItemsMap.GetKeys(Keys);

	for (UItemInfoWidget* Key : Keys)
	{
		AcquireLootFromWidget(Key);
	}
}

void ULootWidget::AcquireLootFromWidget(UItemInfoWidget* InWidget)
{
	bool bWidgetWasSelected = false;
	if (InWidget && LootItemsMap.Contains(InWidget))
	{
		bWidgetWasSelected = InWidget->IsSelected();
		FGeneratedLootInfo LootInfo = LootItemsMap[InWidget];
		AEODPlayerController* EODPC = Cast<AEODPlayerController>(GetOwningPlayer());
		check(EODPC);
		ILootableInterface* LootInterface = Cast<ILootableInterface>(GetLootSource());
		check(LootInterface);

		LootInterface->Execute_AcquireLoot(GetLootSource(), LootInfo, EODPC);

		InWidget->RemoveFromParent();
		LootItemsMap.Remove(InWidget);
	}

	if (bWidgetWasSelected)
	{
		SelectFirstLootItem();
	}

	if (LootItemsMap.Num() == 0)
	{
		OnAllLootPicked();
	}
}

void ULootWidget::BP_SetLootSource(UObject* InLootSource)
{
	LootSource = InLootSource;
}

UObject* ULootWidget::BP_GetLootSource() const
{
	return LootSource.Get();
}

void ULootWidget::OnLootItemPressed(UItemInfoWidget* PressedWidget)
{
	SelectLootItem(PressedWidget);
}

void ULootWidget::OnAcquireButtonClicked()
{
	TArray<UItemInfoWidget*> Keys;
	LootItemsMap.GetKeys(Keys);

	UItemInfoWidget* SelectedWidget = nullptr;
	for (UItemInfoWidget* Key : Keys)
	{
		if (Key->IsSelected())
		{
			SelectedWidget = Key;
			break;
		}
	}

	if (SelectedWidget)
	{
		AcquireLootFromWidget(SelectedWidget);
	}
}

void ULootWidget::OnAcquireAllButtonClicked()
{
	PickAllLoot();
}
