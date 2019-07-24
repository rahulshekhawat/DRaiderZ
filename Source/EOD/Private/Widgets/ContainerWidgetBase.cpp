// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ContainerWidgetBase.h"
#include "TooltipWidget.h"
#include "DragVisualWidget.h"

#include "WidgetBlueprintLibrary.h"

UContainerWidgetBase::UContainerWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCooldownEnabled = false;
}

bool UContainerWidgetBase::Initialize()
{
	if (Super::Initialize() &&
		RootBorder &&
		MainButton &&
		ItemImage &&
		SubText &&
		CooldownText)
	{
		InitializeTooltipWidget();

		SubText->SetVisibility(ESlateVisibility::Hidden);
		CooldownText->SetVisibility(ESlateVisibility::Hidden);

		MainButton->OnClicked.AddUniqueDynamic(this, &UContainerWidgetBase::MainButtonClicked);

		return true;
	}

	return false;
}

void UContainerWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UContainerWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UContainerWidgetBase::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UClass* DragWidgetClass = DragVisualClass.Get();
	check(DragWidgetClass);
	UDragDropOperation* DragDropOp = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	if (DragDropOp)
	{
		UDragVisualWidget* DragVisualWidget = CreateWidget<UDragVisualWidget>(GetOwningPlayer(), DragWidgetClass);
		DragVisualWidget->DragIcon = Cast<UTexture>(this->ItemImage->Brush.GetResourceObject());

		DragDropOp->DefaultDragVisual = DragVisualWidget;
		DragDropOp->Payload = GetDataObj();
		DragDropOp->Pivot = EDragPivot::CenterCenter;
		DragDropOp->Offset = FVector2D(0.f, 0.f);
	}
}

bool UContainerWidgetBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

void UContainerWidgetBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
}

void UContainerWidgetBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

FReply UContainerWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UContainerWidgetBase::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UContainerWidgetBase::MainButtonClicked()
{
	OnClicked.Broadcast(this);
}

void UContainerWidgetBase::PostManualConstruction(UUserWidget* InParentWidget, UObject* InDataObj)
{

	// check(ParentWidget); // ParentWidget shouldn't be NULL

	/*
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
	*/
}

void UContainerWidgetBase::ResetContainer()
{
}

void UContainerWidgetBase::SetDataObj(UObject* InDataObj)
{
}

bool UContainerWidgetBase::IsContainerEmpty() const
{
	return (DataObj != nullptr);
}

void UContainerWidgetBase::EnableContainer()
{
	check(RootBorder);
	RootBorder->SetContentColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
}

void UContainerWidgetBase::DisableContainer()
{
	check(RootBorder);
	RootBorder->SetContentColorAndOpacity(FLinearColor(0.15f, 0.15f, 0.15f, 1.f));
}

void UContainerWidgetBase::EnableCooldown()
{
	check(CooldownText);
	CooldownText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	check(ItemImage);
	ItemImage->SetColorAndOpacity(FLinearColor(0.15f, 0.15f, 0.15f, 1.f));

	bCooldownEnabled = true;
}

void UContainerWidgetBase::DisableCooldown()
{
	check(CooldownText);
	CooldownText->SetVisibility(ESlateVisibility::Hidden);

	check(ItemImage);
	ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

	bCooldownEnabled = false;
}

void UContainerWidgetBase::SetCooldownValue(float InValue)
{
	check(CooldownText);
	CooldownText->SetText(FText::FromString(FString::FromInt(InValue)));
}

void UContainerWidgetBase::SetIcon(UTexture* NewIcon)
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
}

void UContainerWidgetBase::SetSubText(int32 InCurrentValue, int32 InMaxValue)
{
}

void UContainerWidgetBase::SetCooldown(float InCooldown)
{
}

UWidget* UContainerWidgetBase::GetTooltipWidget()
{
	if (DataObj.Get())
	{
		return ToolTipWidget;
	}

	return nullptr;
}

void UContainerWidgetBase::InitializeTooltipWidget()
{
	if (TooltipWidgetClass.Get())
	{
		ToolTipWidget = CreateWidget<UUserWidget>(this, TooltipWidgetClass.Get());
	}

	if (!RootBorder->ToolTipWidgetDelegate.IsBoundToObject(this))
	{
		RootBorder->ToolTipWidgetDelegate.BindUFunction(this, TEXT("GetTooltipWidget"));
	}
}

void UContainerWidgetBase::UpdateTooltipWidget()
{
}
