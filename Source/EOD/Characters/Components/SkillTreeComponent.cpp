// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeComponent.h"
#include "EODCharacterBase.h"
#include "EODPlayerController.h"
#include "GameplaySkillBase.h"
#include "EODGameInstance.h"
#include "PlayerSaveGame.h"
#include "DynamicHUDWidget.h"
#include "DynamicSkillTreeWidget.h"
#include "SkillPointsInfoWidget.h"

USkillTreeComponent::USkillTreeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(false);
}

void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void USkillTreeComponent::InitializeSkillTreeWidget()
{
	//~ @todo modularize the logic for widgets and local skill data

	AEODPlayerController* PC = Cast<AEODPlayerController>(GetOuter());
	UDynamicHUDWidget* HUDWidget = PC ? PC->GetHUDWidget() : nullptr;
	SkillTreeWidget = HUDWidget ? HUDWidget->GetSkillTreeWidget() : nullptr;

	UEODGameInstance* GI = Cast<UEODGameInstance>(PC->GetGameInstance());
	UPlayerSaveGame* SaveGame = GI ? GI->GetCurrentPlayerSaveGameObject() : nullptr;

	if (!SkillTreeWidget || !SaveGame)
	{
		return;
	}

	if (SkillTreeWidget && SaveGame)
	{
		SkillPointsInfoWidget = SkillTreeWidget->GetSkillPointsInfoWidget();
		if (SaveGame)
		{
			SkillTreeWidget->InitializeSkillTreeLayout(this, SkillTreeLayoutTable, SaveGame->SkillTreeSlotsSaveData);
		}
		else
		{
			SkillTreeWidget->InitializeSkillTreeLayout(this, SkillTreeLayoutTable);
		}
	}

	SkillPointsInfoWidget = SkillTreeWidget->GetSkillPointsInfoWidget();
	if (!SkillPointsInfoWidget)
	{
		return;
	}

	SkillPointsAllocationInfo = SaveGame->SkillPointsAllocationInfo;
	if (SkillPointsAllocationInfo.AvailableSkillPoints + SkillPointsAllocationInfo.UsedSkillPoints < SkillPointsUnlockedByDefault)
	{
		SkillPointsAllocationInfo.AvailableSkillPoints = SkillPointsUnlockedByDefault - SkillPointsAllocationInfo.UsedSkillPoints;
	}

	SkillPointsInfoWidget->UpdateSkillPointAllocationText(SkillPointsAllocationInfo);

}

bool USkillTreeComponent::AttemptPointAllocationToSlot(FName SkillGroup)
{
	if (!CanAllocatePointToSlot(SkillGroup))
	{
		return false;
	}

	

	return true;
}

bool USkillTreeComponent::CanAllocatePointToSlot(FName SkillGroup)
{
	if (SkillPointsAllocationInfo.AvailableSkillPoints == 0 || SkillTreeLayoutTable == nullptr)
	{
		return false;
	}

	FString ContextString = FString("USkillTreeComponent::CanAllocatePointToSlot()");
	FSkillTreeSlot* SkillTreeSlot = SkillTreeLayoutTable->FindRow<FSkillTreeSlot>(SkillGroup, ContextString);
	
	// If skil tree slot was not found
	if (SkillTreeSlot == nullptr)
	{
		return false;
	}

	// SkillTreeSlot

	return true;
}
