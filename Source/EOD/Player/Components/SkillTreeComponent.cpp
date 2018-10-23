// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeComponent.h"
#include "Player/PlayerCharacter.h"
#include "UI/SkillTreeWidget.h"
#include "UI/HUDWidget.h"

USkillTreeComponent::USkillTreeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(false);

	SkillGroupMap.Add(FString("BZ0"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ1"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ2"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ3"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ4"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ5"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ6"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ7"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ8"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ9"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ10"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ11"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ12"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ13"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ14"), FSkillGroup());
	SkillGroupMap.Add(FString("BZ15"), FSkillGroup());

}

void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();	

	InitializeComponentWidget();
}

void USkillTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

USkillTreeWidget * USkillTreeComponent::BP_GetSkillTreeWidget() const
{
	return GetSkillTreeWidget();
}

void USkillTreeComponent::InitializeComponentWidget()
{
	APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetOwner());
	if (!(OwningPlayer && OwningPlayer->IsLocallyControlled() && OwningPlayer->GetHUDWidget()))
	{
		return;
	}

	if (SkillTreeWidgetClass.Get())
	{
		SkillTreeWidget = CreateWidget<USkillTreeWidget>(OwningPlayer->GetGameInstance(), SkillTreeWidgetClass);
		OwningPlayer->GetHUDWidget()->AddSkillTreeWidget(SkillTreeWidget);
		SkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USkillTreeComponent::ToggleSkillTreeUI()
{
	if (SkillTreeWidget)
	{
		if (SkillTreeWidget->IsVisible())
		{
			SkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			SkillTreeWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
