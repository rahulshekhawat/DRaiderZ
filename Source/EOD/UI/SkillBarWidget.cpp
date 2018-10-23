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

/*
void USkillBarWidget::PressSkillButton(uint32 SkillIndex)
{
	if (!OwningEODPlayer)
	{
		// @todo raise error
		return;
	}

	switch (SkillIndex)
	{
	case 1:
		OwningEODPlayer->StartSkill(Skill_1->EODItemInfo.ItemID);
		break;
	case 2:
		OwningEODPlayer->StartSkill(Skill_2->EODItemInfo.ItemID);
		break;
	case 3:
		OwningEODPlayer->StartSkill(Skill_3->EODItemInfo.ItemID);
		break;
	case 4:
		OwningEODPlayer->StartSkill(Skill_4->EODItemInfo.ItemID);
		break;
	case 5:
		OwningEODPlayer->StartSkill(Skill_5->EODItemInfo.ItemID);
		break;
	case 6:
		OwningEODPlayer->StartSkill(Skill_6->EODItemInfo.ItemID);
		break;
	case 7:
		OwningEODPlayer->StartSkill(Skill_7->EODItemInfo.ItemID);
		break;
	case 8:
		OwningEODPlayer->StartSkill(Skill_8->EODItemInfo.ItemID);
		break;
	case 9:
		OwningEODPlayer->StartSkill(Skill_9->EODItemInfo.ItemID);
		break;
	case 10:
		OwningEODPlayer->StartSkill(Skill_10->EODItemInfo.ItemID);
		break;
	case 11:
		OwningEODPlayer->StartSkill(Skill_11->EODItemInfo.ItemID);
		break;
	case 12:
		OwningEODPlayer->StartSkill(Skill_12->EODItemInfo.ItemID);
		break;
	case 13:
		OwningEODPlayer->StartSkill(Skill_13->EODItemInfo.ItemID);
		break;
	case 14:
		OwningEODPlayer->StartSkill(Skill_14->EODItemInfo.ItemID);
		break;
	case 15:
		OwningEODPlayer->StartSkill(Skill_15->EODItemInfo.ItemID);
		break;
	case 16:
		OwningEODPlayer->StartSkill(Skill_16->EODItemInfo.ItemID);
		break;
	case 17:
		OwningEODPlayer->StartSkill(Skill_17->EODItemInfo.ItemID);
		break;
	case 18:
		OwningEODPlayer->StartSkill(Skill_18->EODItemInfo.ItemID);
		break;
	case 19:
		OwningEODPlayer->StartSkill(Skill_19->EODItemInfo.ItemID);
		break;
	case 20:
		OwningEODPlayer->StartSkill(Skill_20->EODItemInfo.ItemID);
		break;
	default:
		break;
	}
}
*/

/*
void USkillBarWidget::ReleaseSkillButton(uint32 SkillIndex)
{
	if (!OwningEODPlayer)
	{
		// @todo raise error
		return;
	}

	switch (SkillIndex)
	{
	case 1:
		OwningEODPlayer->StopSkill(Skill_1->EODItemInfo.ItemID);
		break;
	case 2:
		OwningEODPlayer->StopSkill(Skill_2->EODItemInfo.ItemID);
		break;
	case 3:
		OwningEODPlayer->StopSkill(Skill_3->EODItemInfo.ItemID);
		break;
	case 4:
		OwningEODPlayer->StopSkill(Skill_4->EODItemInfo.ItemID);
		break;
	case 5:
		OwningEODPlayer->StopSkill(Skill_5->EODItemInfo.ItemID);
		break;
	case 6:
		OwningEODPlayer->StopSkill(Skill_6->EODItemInfo.ItemID);
		break;
	case 7:
		OwningEODPlayer->StopSkill(Skill_7->EODItemInfo.ItemID);
		break;
	case 8:
		OwningEODPlayer->StopSkill(Skill_8->EODItemInfo.ItemID);
		break;
	case 9:
		OwningEODPlayer->StopSkill(Skill_9->EODItemInfo.ItemID);
		break;
	case 10:
		OwningEODPlayer->StopSkill(Skill_10->EODItemInfo.ItemID);
		break;
	case 11:
		OwningEODPlayer->StopSkill(Skill_11->EODItemInfo.ItemID);
		break;
	case 12:
		OwningEODPlayer->StopSkill(Skill_12->EODItemInfo.ItemID);
		break;
	case 13:
		OwningEODPlayer->StopSkill(Skill_13->EODItemInfo.ItemID);
		break;
	case 14:
		OwningEODPlayer->StopSkill(Skill_14->EODItemInfo.ItemID);
		break;
	case 15:
		OwningEODPlayer->StopSkill(Skill_15->EODItemInfo.ItemID);
		break;
	case 16:
		OwningEODPlayer->StopSkill(Skill_16->EODItemInfo.ItemID);
		break;
	case 17:
		OwningEODPlayer->StopSkill(Skill_17->EODItemInfo.ItemID);
		break;
	case 18:
		OwningEODPlayer->StopSkill(Skill_18->EODItemInfo.ItemID);
		break;
	case 19:
		OwningEODPlayer->StopSkill(Skill_19->EODItemInfo.ItemID);
		break;
	case 20:
		OwningEODPlayer->StopSkill(Skill_20->EODItemInfo.ItemID);
		break;
	default:
		break;
	}
}
*/
