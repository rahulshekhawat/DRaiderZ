// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ContainerWidget.h"
#include "EODGlobalNames.h"
#include "EOD.h"
#include "InventoryWidget.h"
#include "DynamicSkillTreeWidget.h"
#include "ContainerDragDropOperation.h"
#include "DynamicSkillBarWidget.h"
#include "PlayerSkillsComponent.h"
#include "PlayerSkillBase.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"

UContainerWidget::UContainerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ContainerType = EContainerType::None;
	bDisplaySubTextAsRatio = false;
	bSubTextVisible = true;
}

bool UContainerWidget::Initialize()
{
	if (Super::Initialize() && SubText && CooldownText && ItemImage)
	{
		SubText->SetVisibility(ESlateVisibility::Hidden);
		CooldownText->SetVisibility(ESlateVisibility::Hidden);

		Internal_InitializeContainer();
		return true;
	}
	return false;
}

void UContainerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UContainerWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UContainerWidget::InitializeWithParent(UUserWidget* ParentWidget)
{
	check(ParentWidget); // ParentWidget shouldn't be NULL
	
	ContainerParentWidget = ParentWidget;
	if (ParentWidget->IsA(UDynamicSkillTreeWidget::StaticClass()))
	{
		ContainerType = EContainerType::SkillTree;
		bDisplaySubTextAsRatio = true;
	}
	else if (ParentWidget->IsA(UDynamicSkillBarWidget::StaticClass()))
	{
		ContainerType = EContainerType::SkillBar;
	}
}

void UContainerWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	OnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UContainerWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// Cannot drop anything on skill tree
	if (this->ContainerType == EContainerType::SkillTree)
	{
		return false;
	}

	UContainerDragDropOperation* ContOperation = Cast<UContainerDragDropOperation>(InOperation);
	if (!ContOperation || !ContOperation->DraggedContainerWidget)
	{
		return false;
	}

	bool bResult = false;
	// Cannot drop anything from skill tree to inventory
	// Cannot drop anything from inventory to skill bar (It could be implemented in future)
	// Cannot drop anything from skill bar to inventory
	if (ContOperation->DraggedContainerWidget->ContainerType == EContainerType::SkillTree &&
		this->ContainerType == EContainerType::SkillBar)
	{
		UDynamicSkillBarWidget* SkillBarWidget = Cast<UDynamicSkillBarWidget>(ContainerParentWidget);
		if (SkillBarWidget)
		{
			bResult = SkillBarWidget->OnContainerDropped(ContOperation->DraggedContainerWidget, this);
		}
	}
	else if (ContOperation->DraggedContainerWidget->ContainerType == EContainerType::SkillBar &&
		this->ContainerType == EContainerType::SkillBar)
	{
		UDynamicSkillBarWidget* SkillBarWidget = Cast<UDynamicSkillBarWidget>(ContainerParentWidget);
		if (SkillBarWidget)
		{
			bResult = SkillBarWidget->OnContainersSwapped(ContOperation->DraggedContainerWidget, this);
		}
	}
	else if (ContOperation->DraggedContainerWidget->ContainerType == EContainerType::Inventory &&
		this->ContainerType == EContainerType::Inventory)
	{
		UInventoryWidget* InventoryWidget = Cast<UInventoryWidget>(ContainerParentWidget);
		//~ @todo inventory container drop event
	}

	return bResult;
}

void UContainerWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (bCanBeClicked && EmptyBorderMID)
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, HoveredBorderColor);
	}
}

void UContainerWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (EmptyBorderMID)
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, NormalBorderColor);
	}
}

FReply UContainerWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bool bLMBDown = InMouseEvent.IsMouseButtonDown(FKey(KeyboardKeysNames::LeftMouseButton));
	bool bRMBDown = InMouseEvent.IsMouseButtonDown(FKey(KeyboardKeysNames::RightMouseButton));

	FReply Reply = FReply::Handled();
	if (bLMBDown && bCanBeClicked && EmptyBorderMID)
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, PressedBorderColor);
		OnClicked.Broadcast(this, ContainerParentWidget);
	}
	else if (bRMBDown && bCanBeDragged && ContainerData.Icon != nullptr)
	{
		TSharedPtr<SWidget> SlateWidgetDetectingDrag = this->GetCachedWidget();
		if (SlateWidgetDetectingDrag.IsValid())
		{
			Reply = Reply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), InMouseEvent.GetEffectingButton());
		}
	}
	
	return Reply;
}

FReply UContainerWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bCanBeClicked && EmptyBorderMID)
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, HoveredBorderColor);
	}
	return FReply::Handled();
}

void UContainerWidget::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);

	if (ItemImage)
	{
		ItemImage->SetIsEnabled(bInIsEnabled);
	}
}

void UContainerWidget::SetCanBeClicked(bool bValue)
{
	bCanBeClicked = bValue;

	if (!bCanBeClicked && EmptyBorderMID)
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, NormalBorderColor);
	}
	//~ @todo Update container's border material
}

void UContainerWidget::SetCanBeDragged(bool bValue)
{
	bCanBeDragged = bValue;
	//~ @todo Update container's border material
}

void UContainerWidget::SetIcon(UTexture* NewIcon)
{
	if (ItemImage)
	{
		// If the NewIcon is a valid texture
		if (NewIcon)
		{
			FSlateBrush SlateBrush;
			SlateBrush.ImageSize = FVector2D(52.0, 52.0);
			SlateBrush.DrawAs = ESlateBrushDrawType::Image;
			SlateBrush.ImageType = ESlateBrushImageType::FullColor;
			SlateBrush.SetResourceObject(NewIcon);
			ItemImage->SetBrush(SlateBrush);
		}
		// If the NewIcon is NULL, we simply remove the previously set image inside ItemImage
		else
		{
			FSlateBrush SlateBrush;
			SlateBrush.ImageSize = FVector2D(52.0, 52.0);
			SlateBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
			SlateBrush.ImageType = ESlateBrushImageType::NoImage;
			ItemImage->SetBrush(SlateBrush);
		}

		ContainerData.Icon = NewIcon;
	}
}

void UContainerWidget::SetInGameName(const FString& NewInGameName)
{
	ToolTipText = FText::FromString(NewInGameName);
}

void UContainerWidget::SetDescription(const FString& NewDescription)
{
}

void UContainerWidget::SetItemType(EEODItemType EODItemType)
{
	ContainerData.EODItemType = EODItemType;
}

void UContainerWidget::SetCurrentValue(int32 NewValue)
{
	ContainerData.CurrentValue = NewValue;
	check(ContainerData.CurrentValue <= ContainerData.MaxValue);
	if (bSubTextVisible)
	{
		FString NewSubTextString;
		if (bDisplaySubTextAsRatio)
		{
			NewSubTextString = FString::FromInt(NewValue) + FString("/") + FString::FromInt(ContainerData.MaxValue);
		}
		else
		{
			NewSubTextString = FString::FromInt(NewValue);
		}

		FText NewText = FText::FromString(NewSubTextString);
		SubText->SetText(NewText);
	}
}

void UContainerWidget::SetMaxValue(int32 NewValue)
{
	ContainerData.MaxValue = NewValue;
	if (bDisplaySubTextAsRatio && SubText)
	{
		SubText->SetVisibility(ESlateVisibility::Visible);
		FString NewSubTextString = FString::FromInt(ContainerData.CurrentValue) + FString("/") + FString::FromInt(NewValue);
		FText NewText = FText::FromString(NewSubTextString);
		SubText->SetText(NewText);
	}
}

void UContainerWidget::SetContainerData(const FContainerData& NewData)
{
	SetItemID(NewData.ItemID);
	SetItemGroup(NewData.ItemGroup);
	SetIcon(NewData.Icon);
	SetInGameName(NewData.InGameName);
	SetDescription(NewData.Description);
	SetItemType(NewData.EODItemType);
	SetMaxValue(NewData.MaxValue);
	SetCurrentValue(NewData.CurrentValue);
}

void UContainerWidget::EnableCooldownText()
{
	if (ItemImage && CooldownText)
	{
		bIsInCooldown = true;
		ItemImage->SetIsEnabled(false);
		SetCanBeClicked(false);
		CooldownText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UContainerWidget::DisableCooldownText()
{
	if (ItemImage && CooldownText)
	{
		bIsInCooldown = false;
		ItemImage->SetIsEnabled(true);
		SetCanBeClicked(true);
		CooldownText->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ContainerType == EContainerType::SkillBar)
	{
		UDynamicSkillBarWidget* SkillBarWidget = Cast<UDynamicSkillBarWidget>(ContainerParentWidget);
		if (SkillBarWidget)
		{
			UPlayerSkillsComponent* SkillsComp = SkillBarWidget->GetOwnerSkillComponent();
			if (SkillsComp)
			{
				uint8 SkillBarIndex = SkillBarWidget->GetIndexOfSkillContainer(this);
				const TMap<uint8, uint8>& SkillBarMap = SkillsComp->GetSkillBarMap();
				const TMap<uint8, UGameplaySkillBase*>& SkillsMap = SkillsComp->GetSkillsMap();
				
				uint8 SkillIndex = SkillBarMap.Contains(SkillBarIndex) ? SkillBarMap[SkillBarIndex] : 0;
				UPlayerSkillBase* Skill = SkillsMap.Contains(SkillIndex) ? Cast<UPlayerSkillBase>(SkillsMap[SkillIndex]) : nullptr;

				if (Skill)
				{
					bool bCanActivate = Skill->CanPlayerActivateThisSkill();
					if (bCanActivate)
					{
						ItemImage->SetIsEnabled(true);
						SetCanBeClicked(true);
					}
					else
					{
						ItemImage->SetIsEnabled(false);
						SetCanBeClicked(false);
					}
				}
			}
		}
	}
}

void UContainerWidget::SetItemID(FName NewID)
{
	ContainerData.ItemID = NewID;
}

void UContainerWidget::SetItemGroup(FName NewItemGroup)
{
	ContainerData.ItemGroup = NewItemGroup;
}

void UContainerWidget::Internal_InitializeContainer()
{
	if (BorderImage)
	{
		if (!EmptyBorderMID)
		{
			EmptyBorderMID = BorderImage->GetDynamicMaterial();
		}

		if (EmptyBorderMID)
		{
			EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, NormalBorderColor);
		}
	}

	// Re-initiate container data with existing container data
	SetContainerData(ContainerData);

	SetCanBeClicked(true);
	SetCanBeDragged(true);
	SetIsEnabled(true);

	if (SubText)
	{
		if (bSubTextVisible)
		{
			SubText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			SubText->SetVisibility(ESlateVisibility::Hidden);
		}
	}	
}

void UContainerWidget::UpdateCooldown(float CooldownTimeRemaining)
{
	if (CooldownTimeRemaining <= 0.f)
	{
		if (bIsInCooldown)
		{
			DisableCooldownText();
		}
	}
	else
	{
		if (!bIsInCooldown)
		{
			EnableCooldownText();
		}

		if (CooldownText)
		{
			CooldownText->SetText(FText::FromString(FString::FromInt(CooldownTimeRemaining)));
			CooldownRemaining = CooldownTimeRemaining;
		}
	}
}
