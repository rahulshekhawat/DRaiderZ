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
#include "ContainerWidget.h"

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
	SkillPointsInfoWidget = SkillTreeWidget ? SkillTreeWidget->GetSkillPointsInfoWidget() : nullptr;

	UEODGameInstance* GI = Cast<UEODGameInstance>(PC->GetGameInstance());
	UPlayerSaveGame* SaveGame = GI ? GI->GetCurrentPlayerSaveGameObject() : nullptr;

	if (SaveGame)
	{
		SetSkillPointsAllocationInfo(SaveGame->SkillPointsAllocationInfo);
		SkillTreeSlotsSaveData = SaveGame->SkillTreeSlotsSaveData;
	}
	else
	{
		SetSkillPointsAllocationInfo(SkillPointsAllocationInfo);
	}

	if (SkillPointsAllocationInfo.AvailableSkillPoints + SkillPointsAllocationInfo.UsedSkillPoints < SkillPointsUnlockedByDefault)
	{
		SetAvailableSkillPoints(SkillPointsUnlockedByDefault - SkillPointsAllocationInfo.UsedSkillPoints);
	}

	if (SkillTreeWidget)
	{
		SkillTreeWidget->InitializeSkillTreeLayout(this, SkillTreeLayoutTable, SkillTreeSlotsSaveData);
	}
}

bool USkillTreeComponent::AttemptPointAllocationToSlot(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo)
{
	FSkillTreeSlot* SkillTreeSlot = SkillSlotInfo;
	if (SkillTreeSlot == nullptr)
	{
		FString ContextString = FString("USkillTreeComponent::AttemptPointAllocationToSlot()");
		SkillTreeSlot = SkillTreeLayoutTable->FindRow<FSkillTreeSlot>(SkillGroup, ContextString);
	}	

	if (!CanAllocatePointToSlot(SkillGroup, SkillTreeSlot))
	{
		return false;
	}

	switch (SkillTreeSlot->Vocation)
	{
	case EVocations::Assassin:
		ModifyAllocatedPointsAssassin(1);
		break;
	case EVocations::Berserker:
		ModifyAllocatedPointsBerserker(1);
		break;
	case EVocations::Cleric:
		ModifyAllocatedPointsCleric(1);
		break;
	case EVocations::Defender:
		ModifyAllocatedPointsDefender(1);
		break;
	case EVocations::Sorcerer:
		ModifyAllocatedPointsSorcerer(1);
		break;
	default:
		break;
	}

	ModifyAvailableSkillPoints(-1);
	ModifyUsedSkillPoints(1);

	ModifySkillSlotUpgrade(SkillGroup, 1);

	return true;
}

bool USkillTreeComponent::CanAllocatePointToSlot(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo)
{
	if (IsSkillAvailable(SkillGroup, SkillSlotInfo) && IsAnySkillPointAvailable())
	{
		return true;
	}

	return false;
}

bool USkillTreeComponent::IsAnySkillPointAllocatedToSlot(FName SkillGroup)
{
	if (SkillTreeSlotsSaveData.Contains(SkillGroup))
	{
		FSkillTreeSlotSaveData& SaveData = SkillTreeSlotsSaveData[SkillGroup];
		if (SaveData.CurrentUpgrade > 0)
		{
			return true;
		}
	}

	return false;
}

bool USkillTreeComponent::IsSkillAvailable(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo)
{
	if (SkillGroup == NAME_None)
	{
		return false;
	}

	FSkillTreeSlot* SkillTreeSlot = SkillSlotInfo;
	if (SkillTreeSlot == nullptr && SkillTreeLayoutTable)
	{
		FString ContextString = FString("USkillTreeComponent::CanAllocatePointToSlot()");
		SkillTreeSlot = SkillTreeLayoutTable->FindRow<FSkillTreeSlot>(SkillGroup, ContextString);
	}

	// If skil tree slot was not found
	if (SkillTreeSlot == nullptr)
	{
		return false;
	}

	int32 CurrentSkillGroupUpgrade = SkillTreeSlotsSaveData.Contains(SkillGroup) ? SkillTreeSlotsSaveData[SkillGroup].CurrentUpgrade : 0;
	// If the skill upgrade is already maxxed out
	if (CurrentSkillGroupUpgrade == SkillTreeSlot->MaxUpgrades)
	{
		return false;
	}

	int32 UsedSkillPointsRequired = SkillTreeSlot->MinimumPointsToUnlock + CurrentSkillGroupUpgrade * SkillTreeSlot->UpgradePointsGap;
	if (SkillPointsAllocationInfo.UsedSkillPoints < UsedSkillPointsRequired)
	{
		return false;
	}

	if (SkillTreeSlot->SkillRequiredToUnlock != NAME_None)
	{
		int32 UnlockSkillUpgradeLevel = SkillTreeSlotsSaveData.Contains(SkillTreeSlot->SkillRequiredToUnlock) ? SkillTreeSlotsSaveData[SkillTreeSlot->SkillRequiredToUnlock].CurrentUpgrade : 0;
		if (UnlockSkillUpgradeLevel == 0)
		{
			return false;
		}
	}

	return true;
}

ESkillSlotStatus USkillTreeComponent::GetSkillSlotStatus(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo)
{
	return ESkillSlotStatus();
}

void USkillTreeComponent::SetAllocatedPointsAssassin(int32 Value)
{
	SkillPointsAllocationInfo.AssassinPoints = Value;
	if (SkillPointsInfoWidget)
	{
		SkillPointsInfoWidget->UpdateAssassinPointsText(Value);
	}
}

void USkillTreeComponent::SetAllocatedPointsBerserker(int32 Value)
{
	SkillPointsAllocationInfo.BerserkerPoints = Value;
	if (SkillPointsInfoWidget)
	{
		SkillPointsInfoWidget->UpdateBerserkerPointsText(Value);
	}
}

void USkillTreeComponent::SetAllocatedPointsCleric(int32 Value)
{
	SkillPointsAllocationInfo.ClericPoints = Value;
	if (SkillPointsInfoWidget)
	{
		SkillPointsInfoWidget->UpdateClericPointsText(Value);
	}
}

void USkillTreeComponent::SetAllocatedPointsDefender(int32 Value)
{
	SkillPointsAllocationInfo.DefenderPoints = Value;
	if (SkillPointsInfoWidget)
	{
		SkillPointsInfoWidget->UpdateDefenderPointsText(Value);
	}
}

void USkillTreeComponent::SetAllocatedPointsSorcerer(int32 Value)
{
	SkillPointsAllocationInfo.SorcererPoints = Value;
	if (SkillPointsInfoWidget)
	{
		SkillPointsInfoWidget->UpdateSorcererPointsText(Value);
	}
}

void USkillTreeComponent::SetAvailableSkillPoints(int32 Value)
{
	SkillPointsAllocationInfo.AvailableSkillPoints = Value;
	if (SkillPointsInfoWidget)
	{
		SkillPointsInfoWidget->UpdateAvailableSkillPointsText(Value);
	}
}

void USkillTreeComponent::SetUsedSkillPoints(int32 Value)
{
	SkillPointsAllocationInfo.UsedSkillPoints = Value;
	if (SkillPointsInfoWidget)
	{
		SkillPointsInfoWidget->UpdateUsedSkillPointsText(Value);
	}
}

void USkillTreeComponent::ModifySkillSlotUpgrade(FName SkillGroup, int32 Value)
{
	if (SkillTreeSlotsSaveData.Contains(SkillGroup))
	{
		FSkillTreeSlotSaveData& SaveData = SkillTreeSlotsSaveData[SkillGroup];
		SetSkillSlotUpgrade(SkillGroup, SaveData.CurrentUpgrade + Value);
	}
	else
	{
		FSkillTreeSlotSaveData SaveData;
		SaveData.CurrentUpgrade += Value;
		SkillTreeSlotsSaveData.Add(SkillGroup, SaveData);
		SetSkillSlotUpgrade(SkillGroup, SaveData.CurrentUpgrade);
	}
}

void USkillTreeComponent::SetSkillSlotUpgrade(FName SkillGroup, int32 Value)
{
	check(SkillTreeSlotsSaveData.Contains(SkillGroup));
	FSkillTreeSlotSaveData& SaveData = SkillTreeSlotsSaveData[SkillGroup];
	SaveData.CurrentUpgrade = Value;

	UContainerWidget* SlotWidget = SkillTreeWidget ? SkillTreeWidget->GetSkillSlotForSkillGroup(SkillGroup) : nullptr;
	if (SlotWidget)
	{
		SlotWidget->SetCurrentValue(Value);
	}

	//~ @todo store new upgrade info to save game?
}
