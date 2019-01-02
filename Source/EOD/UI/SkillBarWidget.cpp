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
		Skill_1 &&
		Skill_2 &&
		Skill_3 &&
		Skill_4 &&
		Skill_5 &&
		Skill_6 &&
		Skill_7 &&
		Skill_8 &&
		Skill_9 &&
		Skill_10 &&
		Skill_11 &&
		Skill_12 &&
		Skill_13 &&
		Skill_14 &&
		Skill_15 &&
		Skill_16 &&
		Skill_17 &&
		Skill_18 &&
		Skill_19 &&
		Skill_20))
	{
		InitiateSkillContainer(Skill_1, 1);
		InitiateSkillContainer(Skill_2, 2);
		InitiateSkillContainer(Skill_3, 3);
		InitiateSkillContainer(Skill_4, 4);
		InitiateSkillContainer(Skill_5, 5);
		InitiateSkillContainer(Skill_6, 6);
		InitiateSkillContainer(Skill_7, 7);
		InitiateSkillContainer(Skill_8, 8);
		InitiateSkillContainer(Skill_9, 9);
		InitiateSkillContainer(Skill_10, 10);
		InitiateSkillContainer(Skill_11, 11);
		InitiateSkillContainer(Skill_12, 12);
		InitiateSkillContainer(Skill_13, 13);
		InitiateSkillContainer(Skill_14, 14);
		InitiateSkillContainer(Skill_15, 15);
		InitiateSkillContainer(Skill_16, 16);
		InitiateSkillContainer(Skill_17, 17);
		InitiateSkillContainer(Skill_18, 18);
		InitiateSkillContainer(Skill_19, 19);
		InitiateSkillContainer(Skill_20, 20);
		return false;
	}

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

void USkillBarWidget::LoadSkillBarLayout()
{
	/*
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
		UEODItemContainer* SkillButton = GetSkillButtonAtIndex(Position);
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
	*/
}

void USkillBarWidget::InitiateSkillContainer(UEODItemContainer* Container, int32 SkillBarIndex)
{
	if (IsValid(Container))
	{
		ContainersList.Add(Container);
		IndexToContainerMap.Add(SkillBarIndex, Container);
	}
}

bool USkillBarWidget::IsSkillGroupInCooldown(const FString& SkillGroup) const
{
	UEODItemContainer* SkillContainer = nullptr;
	for (int i = 1; i <= 20; i++)
	{
		if (SkillGroup == GetSkillGroupAtIndex(i))
		{
			SkillContainer = GetSkillButtonAtIndex(i);
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
	UEODItemContainer* SkillContainer = GetSkillButtonAtIndex(SkillIndex);
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
			SkillContainer = GetSkillButtonAtIndex(i);
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
	if (!IsValid(STContainer) || !IsValid(ToContainer) || !ContainersList.Contains(ToContainer) || !(STContainer->SkillGroup == FString("")))
	{
		return false;
	}

	FString SkillGroup = STContainer->SkillGroup;



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
	TArray<int32> NewLayoutKeys;
	for (int32& Key : NewLayoutKeys)
	{

	}
}
