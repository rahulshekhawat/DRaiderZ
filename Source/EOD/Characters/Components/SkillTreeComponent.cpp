// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeComponent.h"
#include "EODCharacterBase.h"
#include "EODPlayerController.h"
#include "GameplaySkillBase.h"
#include "EODGameInstance.h"
#include "PlayerSaveGame.h"
#include "DynamicHUDWidget.h"
#include "DynamicSkillTreeWidget.h"

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
	AEODPlayerController* PC = Cast<AEODPlayerController>(GetOuter());
	UDynamicHUDWidget* HUDWidget = PC ? PC->GetHUDWidget() : nullptr;
	SkillTreeWidget = HUDWidget ? HUDWidget->GetSkillTreeWidget() : nullptr;

	UEODGameInstance* GI = Cast<UEODGameInstance>(PC->GetGameInstance());
	UPlayerSaveGame* SaveGame = GI ? GI->GetCurrentPlayerSaveGameObject() : nullptr;

	if (SkillTreeWidget && SaveGame)
	{
		SkillPointsInfoWidget = SkillTreeWidget->GetSkillPointsInfoWidget();
		if (SaveGame)
		{
			SkillTreeWidget->InitializeSkillTreeLayout(SkillTreeLayoutTable, SaveGame->SkillTreeSlotsSaveData);
		}
		else
		{
			SkillTreeWidget->InitializeSkillTreeLayout(SkillTreeLayoutTable);
		}
	}
}

void USkillTreeComponent::InitializeSkills()
{
	if (!SkillTreeLayoutTable)
	{
		return;
	}

	AEODPlayerController* PC = Cast<AEODPlayerController>(GetOwner());
	if (PC)
	{
		PrintToScreen(this, PC->GetName(), 10.f);
	}

	AEODCharacterBase* SkillOwner = PC ? Cast<AEODCharacterBase>(PC->GetPawn()) : nullptr;
	if (SkillOwner)
	{
	}
	else
	{
		return;
	}

	// check(SkillOwner);

	UEODGameInstance* GI = Cast<UEODGameInstance>(SkillOwner->GetGameInstance());
	UPlayerSaveGame* SaveGame = GI ? GI->GetCurrentPlayerSaveGameObject() : nullptr;

	FString ContextString = FString("USkillTreeComponent::InitializeSkills()");
	TArray<FName> RowNames = SkillTreeLayoutTable->GetRowNames();
	for (FName RowName : RowNames)
	{
		FSkillTreeSlot* SkillTreeSlot = SkillTreeLayoutTable->FindRow<FSkillTreeSlot>(RowName, ContextString);
		if (SkillTreeSlot && SkillTreeSlot->PlayerSkill.Get())
		{
			Internal_LoadSkill(RowName, SkillTreeSlot->PlayerSkill, SkillOwner, SaveGame);
		}
	}
}

void USkillTreeComponent::InitializeSkillTreeSlots()
{
	if (SkillTreeWidget)
	{
		// SkillTreeWidget->InitializeSkillTreeSlots(PlayerSkillsMap);
	}
}

void USkillTreeComponent::Internal_LoadSkill(const FName& SkillGroup,
	const TSubclassOf<UGameplaySkillBase>& SkillClass,
	AEODCharacterBase* const SkillOwner,
	UPlayerSaveGame* const SaveGame)
{
	if (!SkillOwner)
	{
		return;
	}

	UGameplaySkillBase* Skill = NewObject<UGameplaySkillBase>(SkillOwner, SkillClass, SkillGroup, RF_Transient);
	if (Skill)
	{
		Skill->InitSkill(SkillOwner, nullptr);
		if (SaveGame && SaveGame->SkillTreeSlotsSaveData.Contains(SkillGroup))
		{
			Skill->SetCurrentUpgrade(SaveGame->SkillTreeSlotsSaveData[SkillGroup].CurrentUpgrade);
		}
		PlayerSkillsMap.Add(SkillGroup, Skill);
	}
}
