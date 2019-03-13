// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeComponent.h"
#include "EODPlayerController.h"
#include "DynamicSkillTreeWidget.h"


USkillTreeComponent::USkillTreeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(false);
}

void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateAndInitializeSkillTreeWidget();
}

void USkillTreeComponent::ToggleSkillTreeUI()
{
	if (SkillTreeWidget && SkillTreeWidget->IsVisible())
	{
		SkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (SkillTreeWidget && !SkillTreeWidget->IsVisible())
	{
		SkillTreeWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void USkillTreeComponent::CreateAndInitializeSkillTreeWidget()
{
	AEODPlayerController* PC = Cast<AEODPlayerController>(GetOuter());
	if (!SkillTreeWidget && SkillTreeWidgetClass.Get() && PC && PC->IsLocalController())
	{
		SkillTreeWidget = CreateWidget<UDynamicSkillTreeWidget>(PC, SkillTreeWidgetClass);
		if (SkillTreeWidget)
		{
			SkillTreeWidget->AddToViewport();
		}
	}
}
