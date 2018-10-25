// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillBarWidget.h"
#include "UI/EODItemContainer.h"
#include "Core/EODSaveGame.h"
#include "Core/EODPreprocessors.h"
#include "Core/GameSingleton.h"
#include "Statics/CharacterLibrary.h"

#include "Button.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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
		return false;
	}

	LoadSkillBarLayout();

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
	UGameSingleton* GameSingleton = nullptr;
	if (GEngine)
	{
		GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
	}

	if (!GameSingleton)
	{
		return;
	}

	UEODSaveGame* EODSaveGame = Cast<UEODSaveGame>(UGameplayStatics::LoadGameFromSlot(GameSingleton->CurrentSaveSlotName, GameSingleton->UserIndex));
	if (!EODSaveGame)
	{
		return;
	}

	TArray<FString> SkillGroups;
	EODSaveGame->SkillBarLayout.GetKeys(SkillGroups);

	for (const FString& SkillGroup : SkillGroups)
	{
		int32 Position = EODSaveGame->SkillBarLayout[SkillGroup];
		UEODItemContainer* SkillButton = GetSkillButtonAtIndex(Position);
		if (!SkillButton)
		{
			continue;
		}

		FString SkillIDString = "F_" + SkillGroup + "_1";
		FName SkillID = FName(*SkillIDString);

		FString ContextString = "USkillBarWidget::LoadSkillBarLayout(), looking for player skill : " + SkillIDString;
		FSkillTableRow* Skill = UCharacterLibrary::GetPlayerSkill(SkillID, ContextString);
		if (!Skill)
		{
#if MESSAGE_LOGGING_ENABLED
			FString Message = FString("Couldn't find skill : ") + SkillIDString;
			UKismetSystemLibrary::PrintString(this, Message);
#endif // MESSAGE_LOGGING_ENABLED
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

void USkillBarWidget::SaveSkillBarLayout()
{
	UGameSingleton* GameSingleton = nullptr;
	if (GEngine)
	{
		GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
	}

	if (!GameSingleton)
	{
		return;
	}

	UEODSaveGame* EODSaveGame = Cast<UEODSaveGame>(UGameplayStatics::LoadGameFromSlot(GameSingleton->CurrentSaveSlotName, GameSingleton->UserIndex));
	if (!EODSaveGame)
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
