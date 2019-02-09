// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillBarWidget.h"
#include "EOD/UI/EODItemContainer.h"
#include "EOD/Core/EODSaveGame.h"
// #include "EOD/Core/EODGameInstance.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/Core/GameSingleton.h"
// #include "EOD/SaveSystem/PlayerSaveGame.h"
#include "EOD/Statics/CharacterLibrary.h"
#include "EOD/UI/SkillTreeItemContainer.h"

#include "Components/Button.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

USkillBarWidget::USkillBarWidget(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer)
{
}

bool USkillBarWidget::Initialize()
{
	if (!(Super::Initialize() &&
		SkillContainer_1 &&
		SkillContainer_2 &&
		SkillContainer_3 &&
		SkillContainer_4 &&
		SkillContainer_5 &&
		SkillContainer_6 &&
		SkillContainer_7 &&
		SkillContainer_8 &&
		SkillContainer_9 &&
		SkillContainer_10 &&
		SkillContainer_11 &&
		SkillContainer_12 &&
		SkillContainer_13 &&
		SkillContainer_14 &&
		SkillContainer_15 &&
		SkillContainer_16 &&
		SkillContainer_17 &&
		SkillContainer_18 &&
		SkillContainer_19 &&
		SkillContainer_20))
	{
		return false;
	}

	InitializeSkillContainer(SkillContainer_1, 1);
	InitializeSkillContainer(SkillContainer_2, 2);
	InitializeSkillContainer(SkillContainer_3, 3);
	InitializeSkillContainer(SkillContainer_4, 4);
	InitializeSkillContainer(SkillContainer_5, 5);
	InitializeSkillContainer(SkillContainer_6, 6);
	InitializeSkillContainer(SkillContainer_7, 7);
	InitializeSkillContainer(SkillContainer_8, 8);
	InitializeSkillContainer(SkillContainer_9, 9);
	InitializeSkillContainer(SkillContainer_10, 10);
	InitializeSkillContainer(SkillContainer_11, 11);
	InitializeSkillContainer(SkillContainer_12, 12);
	InitializeSkillContainer(SkillContainer_13, 13);
	InitializeSkillContainer(SkillContainer_14, 14);
	InitializeSkillContainer(SkillContainer_15, 15);
	InitializeSkillContainer(SkillContainer_16, 16);
	InitializeSkillContainer(SkillContainer_17, 17);
	InitializeSkillContainer(SkillContainer_18, 18);
	InitializeSkillContainer(SkillContainer_19, 19);
	InitializeSkillContainer(SkillContainer_20, 20);
	return true;
}

void USkillBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillBarWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

bool USkillBarWidget::IsSkillGroupInCooldown(const FString& SkillGroup) const
{
	UEODItemContainer* SkillContainer = nullptr;
	for (int i = 1; i <= 20; i++)
	{
		if (SkillGroup == GetSkillGroupAtIndex(i))
		{
			SkillContainer = GetSkillContainerAtIndex(i);
			break;
		}
	}

	if (IsValid(SkillContainer))
	{
		return SkillContainer->bInCooldown;
	}

	return false;
}

void USkillBarWidget::PutSkillOnCooldownTimer(int32 SkillIndex, float Duration, float Interval)
{
	UEODItemContainer* SkillContainer = GetSkillContainerAtIndex(SkillIndex);
	if (IsValid(SkillContainer))
	{
		SkillContainer->StartCooldown(Duration, Interval);
	}
}

void USkillBarWidget::PutSkillOnCooldownTimer(FString SkillGroup, float Duration, float Interval)
{
	UEODItemContainer* SkillContainer = nullptr;
	for (int i = 1; i <= 20; i++)
	{
		if (SkillGroup == GetSkillGroupAtIndex(i))
		{
			SkillContainer = GetSkillContainerAtIndex(i);
			break;
		}
	}

	if (IsValid(SkillContainer))
	{
		SkillContainer->StartCooldown(Duration, Interval);
	}
}


void USkillBarWidget::SaveSkillBarLayout()
{
	UGameSingleton* GameSingleton = IsValid(GEngine) ? Cast<UGameSingleton>(GEngine->GameSingleton) : nullptr;
	if (!IsValid(GameSingleton))
	{
		return;
	}

	UEODSaveGame* EODSaveGame = Cast<UEODSaveGame>(UGameplayStatics::LoadGameFromSlot(GameSingleton->CurrentSaveSlotName, GameSingleton->UserIndex));
	if (!IsValid(EODSaveGame))
	{
		return;
	}

	TMap<FString, int32> SkillBarLayout;
	for (int i = 1; i <= 20; i++)
	{
		FEODItemInfo EODItemInfo = GetEODItemInfoAtIndex(i);
		if (EODItemInfo.ItemID != NAME_None)
		{
			SkillBarLayout.Add(EODItemInfo.ItemGroup, i);
		}
	}

	EODSaveGame->SkillBarLayout = SkillBarLayout;
	UGameplayStatics::SaveGameToSlot(EODSaveGame, GameSingleton->CurrentSaveSlotName, GameSingleton->UserIndex);
}

bool USkillBarWidget::OnNewSkillDropped(UEODItemContainer* FromContainer, UEODItemContainer* ToContainer)
{
	USkillTreeItemContainer* STContainer = Cast<USkillTreeItemContainer>(FromContainer);
	if (!IsValid(STContainer) || !IsValid(ToContainer) || STContainer->SkillGroup == FString(""))
	{
		return false;
	}

	int32 ToContainerIndex = GetIndexOfSkillContainer(ToContainer);
	if (ToContainerIndex <= 0 || ToContainerIndex > 20)
	{
		return false;
	}

	OnNewSkillAdded.Broadcast(ToContainerIndex, STContainer->SkillGroup);

	// if (!IsValid(STContainer) || !IsValid(ToContainer) || !ContainersList.Contains(ToContainer) || !(STContainer->SkillGroup == FString("")))
	ToContainer->EODItemInfo = FromContainer->EODItemInfo;
	ToContainer->RefreshContainerVisuals();

	// FString SkillGroup = STContainer->SkillGroup;



	// STContainer->SkillGroup


	return true;

	/*
	this->EODItemInfo = Operation->DraggedEODItemWidget->EODItemInfo;
	this->RefreshContainerVisuals();

	USkillBarWidget* SkillBarWidget = Cast<USkillBarWidget>(ParentWidget);

	bResult = true;

	return false;
	*/
}

bool USkillBarWidget::OnSkillRemoved(UEODItemContainer* Container)
{
	return false;
}

bool USkillBarWidget::OnSkillsSwapped(UEODItemContainer* Container1, UEODItemContainer* Container2)
{
	/*
	FEODItemInfo TempEODItemInfo = Operation->DraggedEODItemWidget->EODItemInfo;

	Operation->DraggedEODItemWidget->EODItemInfo = this->EODItemInfo;
	this->EODItemInfo = TempEODItemInfo;

	Operation->DraggedEODItemWidget->RefreshContainerVisuals();
	this->RefreshContainerVisuals();

	bResult = true;
	*/
	return false;
}

void USkillBarWidget::UpdateSkillBarLayout(TMap<int32, FString>& NewLayout, bool bResize)
{
	TArray<int32> LayoutKeys;
	NewLayout.GetKeys(LayoutKeys);
	for (int32 Key : LayoutKeys)
	{
		UEODItemContainer* Container = GetSkillContainerAtIndex(Key);
		if (IsValid(Container))
		{
			// Container->EODItemInfo.
		}
	}
}

/*
void USkillBarWidget::LoadSkillBarLayout()
{
	UGameSingleton* GameSingleton = IsValid(GEngine) ? Cast<UGameSingleton>(GEngine->GameSingleton) : nullptr;
	if (!IsValid(GameSingleton))
	{
		return;
	}

	UEODSaveGame* EODSaveGame = Cast<UEODSaveGame>(UGameplayStatics::LoadGameFromSlot(GameSingleton->CurrentSaveSlotName, GameSingleton->UserIndex));
	if (!IsValid(EODSaveGame))
	{
		return;
	}

	TArray<FString> SkillGroups;
	EODSaveGame->SkillBarLayout.GetKeys(SkillGroups);

	for (const FString& SkillGroup : SkillGroups)
	{
		int32 Position = EODSaveGame->SkillBarLayout[SkillGroup];
		UEODItemContainer* SkillButton = GetSkillContainerAtIndex(Position);
		if (!IsValid(SkillButton))
		{
			continue;
		}

		FString SkillIDString = "F_" + SkillGroup + "_1";
		FName SkillID = FName(*SkillIDString);

		FString ContextString = "USkillBarWidget::LoadSkillBarLayout(), looking for player skill : " + SkillIDString;
		FSkillTableRow* Skill = UCharacterLibrary::GetPlayerSkill(SkillID, ContextString);
		if (!Skill)
		{
			FString Message = FString("Couldn't find skill : ") + SkillIDString;
			PrintEverywhere(this, Message);
			continue;
		}

		SkillButton->EODItemInfo.ItemID = SkillID;
		SkillButton->EODItemInfo.ItemGroup = SkillGroup;
		SkillButton->EODItemInfo.StackCount = 1;
		SkillButton->EODItemInfo.InGameName = Skill->InGameName;
		SkillButton->EODItemInfo.Icon = Skill->Icon;
		SkillButton->EODItemInfo.EODItemType = EEODItemType::ActiveSkill; // It MUST be an active skill
		SkillButton->EODItemInfo.Description = Skill->Description;

		SkillButton->RefreshContainerVisuals();
	}
}
*/
