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

	TArray<FName> Keys;
	EODSaveGame->SkillBarLayout.GetKeys(Keys);

	for (FName Key : Keys)
	{
		int32 Position = EODSaveGame->SkillBarLayout[Key];
		UEODItemContainer* SkillButton = GetSkillButtonAtIndex(Position);
		if (!SkillButton)
		{
			continue;
		}


		FSkillTableRow* Skill = UCharacterLibrary::GetPlayerSkill(Key, FString("USkillBarWidget::LoadSkillBarLayout(), looking for player skill"));
		if (!Skill)
		{
#if MESSAGE_LOGGING_ENABLED
			FString Message = FString("Couldn't find skill : ") + Key.ToString();
			UKismetSystemLibrary::PrintString(this, Message);
#endif // MESSAGE_LOGGING_ENABLED
			continue;
		}

		SkillButton->EODItemInfo.ItemID = Key;
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

	TMap<FName, int32> SkillBarLayout;
	for (int i = 1; i <= 20; i++)
	{
		FName SkillID = GetSkillAtIndex(i);
		if (SkillID != NAME_None)
		{
			SkillBarLayout.Add(SkillID, i);
		}
	}

	EODSaveGame->SkillBarLayout = SkillBarLayout;
	UGameplayStatics::SaveGameToSlot(EODSaveGame, GameSingleton->CurrentSaveSlotName, GameSingleton->UserIndex);
}
