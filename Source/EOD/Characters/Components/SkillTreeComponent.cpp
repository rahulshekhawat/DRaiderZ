// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeComponent.h"
#include "EODCharacterBase.h"
#include "EODPlayerController.h"
#include "DynamicSkillTreeWidget.h"
#include "GameplaySkillBase.h"
#include "EODGameInstance.h"
#include "PlayerSaveGame.h"


USkillTreeComponent::USkillTreeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(false);
	// SetIsReplicated(true);
}

void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeSkills();
	CreateAndInitializeSkillTreeWidget();
	InitializeSkillTreeSlots();
	
}

void USkillTreeComponent::ToggleSkillTreeUI()
{
	if (SkillTreeWidget && SkillTreeWidget->IsVisible())
	{
		SkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (SkillTreeWidget && !SkillTreeWidget->IsVisible())
	{
		SkillTreeWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void USkillTreeComponent::CreateAndInitializeSkillTreeWidget()
{
	AEODPlayerController* PC = Cast<AEODPlayerController>(GetOuter());
	if (!SkillTreeWidget && SkillTreeWidgetClass.Get() && PC && PC->IsLocalController())
	{
		SkillTreeWidget = CreateWidget<UDynamicSkillTreeWidget>(PC, SkillTreeWidgetClass);
		if (SkillTreeWidget)
		{
			SkillTreeWidget->AddToViewport();
		}

		//~ @todo add skill tree widget to hud widget

		if (SkillTreeWidget)
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
		SkillTreeWidget->InitializeSkillTreeSlots(PlayerSkillsMap);
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
			Skill->CurrentUpgrade = SaveGame->SkillTreeSlotsSaveData[SkillGroup].CurrentUpgrade;
		}
		PlayerSkillsMap.Add(SkillGroup, Skill);
	}
}
