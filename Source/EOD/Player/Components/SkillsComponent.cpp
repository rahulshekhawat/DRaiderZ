// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillsComponent.h"
#include "UI/HUDWidget.h"
#include "UI/SkillTreeWidget.h"
#include "Core/EODPreprocessors.h"
#include "Core/EODSaveGame.h"
#include "Core/GameSingleton.h"
#include "Player/PlayerCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


USkillsComponent::USkillsComponent(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillsComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeComponentWidgets();
}

void USkillsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

inline USkillBarWidget * USkillsComponent::BP_GetSkillBarWidget() const
{
	return GetSkillBarWidget();
}

inline USkillTreeWidget * USkillsComponent::BP_GetSkillTreeWidget() const
{
	return GetSkillTreeWidget();
}

void USkillsComponent::ToggleSkillTreeUI()
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

void USkillsComponent::InitializeComponentWidgets()
{
	APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetOwner());
	if (!(OwningPlayer && OwningPlayer->Controller && OwningPlayer->Controller->IsLocalPlayerController() && OwningPlayer->GetHUDWidget()))
	{
		return;
	}

	if (SkillTreeWidgetClass.Get())
	{
		SkillTreeWidget = CreateWidget<USkillTreeWidget>(OwningPlayer->GetGameInstance(), SkillTreeWidgetClass);
		OwningPlayer->GetHUDWidget()->AddSkillTreeWidget(SkillTreeWidget);
		SkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (SkillBarWidgetClass.Get())
	{
		SkillBarWidget = CreateWidget<USkillBarWidget>(OwningPlayer->GetGameInstance(), SkillBarWidgetClass);
		OwningPlayer->GetHUDWidget()->AddSkillBarWidget(SkillBarWidget);
	}
}

bool USkillsComponent::CanUseSkill(const int32 SkillSlotIndex)
{
	return false;
}

FName USkillsComponent::GetSkillIDFromSkillSlot(const int32 SkillSlotIndex)
{
	FName SkillID = NAME_None;

	if (SkillBarWidget)
	{
		SkillID = SkillBarWidget->GetSkillAtIndex(SkillSlotIndex);
		FString SkillIDString = SkillID.ToString();
		FString SkillGroup = SkillIDString.Mid(2, 3);

#if MESSAGE_LOGGING_ENABLED
		UKismetSystemLibrary::PrintString(this, SkillGroup);
#endif // MESSAGE_LOGGING_ENABLED

	}

	return SkillID;
}

void USkillsComponent::OnSkillUsed(const int32 SkillSlotIndex, FName SkillID, const FSkillTableRow* Skill)
{
}
