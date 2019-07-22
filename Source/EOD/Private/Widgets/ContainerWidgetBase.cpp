// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "ContainerWidgetBase.h"

UContainerWidgetBase::UContainerWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UContainerWidgetBase::Initialize()
{
	if (Super::Initialize())
	{
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

void UContainerWidgetBase::SetIcon(UTexture* NewIcon)
{
}

void UContainerWidgetBase::SetSubText(int32 InCurrentValue, int32 InMaxValue)
{
}

void UContainerWidgetBase::SetCooldown(float InCooldown)
{
}
