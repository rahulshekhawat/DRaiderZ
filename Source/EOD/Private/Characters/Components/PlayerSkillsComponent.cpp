// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "PlayerSkillsComponent.h"
#include "PlayerCharacter.h"
#include "EODGameInstance.h"
#include "PlayerSaveGame.h"
#include "GameplaySkillBase.h"
#include "ActiveSkillBase.h"
#include "DynamicSkillBarWidget.h"
#include "EODPlayerController.h"
#include "ContainerWidget.h"

#include "TimerManager.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericPlatformTime.h"

UPlayerSkillsComponent::UPlayerSkillsComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerSkillsComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeSkills();
}

void UPlayerSkillsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerSkillsComponent::OnPressingSkillKey(const int32 SkillKeyIndex)
{
	LastPressedSkillKey = SkillKeyIndex;

	uint8 SkillIndex = 0;
	UGameplaySkillBase* Skill = nullptr;
	if (SupersedingChainSkillGroup.Key == SkillKeyIndex)
	{
		SkillIndex = SupersedingChainSkillGroup.Value;
	}
	else if (SkillBarMap.Contains(SkillKeyIndex))
	{
		SkillIndex = SkillBarMap[SkillKeyIndex];
	}

	if (SkillIndexToSkillMap.Contains(SkillIndex))
	{
		Skill = SkillIndexToSkillMap[SkillIndex];
	}

	if (Skill)
	{
		TriggerSkill(SkillIndex, Skill);
	}
}

void UPlayerSkillsComponent::OnReleasingSkillKey(const int32 SkillKeyIndex)
{
	uint8 SkillIndex = 0;
	UGameplaySkillBase* Skill = nullptr;
	if (SkillBarMap.Contains(SkillKeyIndex))
	{
		SkillIndex = SkillBarMap[SkillKeyIndex];
	}

	if (SkillIndexToSkillMap.Contains(SkillIndex))
	{
		Skill = SkillIndexToSkillMap[SkillIndex];
	}

	if (Skill)
	{
		SkillIndex += 100; // Skill Index while releasing skill is offset by 100 for replication purposes
		ReleaseSkill(SkillIndex, Skill);
	}

	LastReleasedSkillKey = SkillKeyIndex;
}

void UPlayerSkillsComponent::InitializeSkills(AEODCharacterBase* CompOwner)
{
	// If skills have already been initialized
	if (SkillIndexToSkillMap.Num() > 0)
	{
		return;
	}

	if (!CompOwner)
	{
		// If CompOwner is invalid, try to initalize it from locally cached component owner
		CompOwner = GetCharacterOwner();
	}

	check(CompOwner)
	if (!SkillsDataTable)
	{
		return;
	}

	FString ContextString = FString("UPlayerSkillsComponent::InitializeSkills()");
	TArray<FName> Keys = SkillsDataTable->GetRowNames();
	uint8 SkillIndex = 1;
	for (FName Key : Keys)
	{
		FGameplaySkillTableRow* Row = SkillsDataTable->FindRow<FGameplaySkillTableRow>(Key, ContextString);
		check(Row);
		UGameplaySkillBase* GameplaySkill = NewObject<UGameplaySkillBase>(this, Row->PlayerSkill, Key, RF_Transient);
		check(GameplaySkill)
		GameplaySkill->InitSkill(CompOwner, CompOwner->Controller);
		GameplaySkill->SetSkillIndex(SkillIndex);

		SkillIndexToSkillMap.Add(SkillIndex, GameplaySkill);
		SkillGroupToSkillMap.Add(GameplaySkill->GetSkillGroup(), GameplaySkill);
		SkillGroupToSkillIndexMap.Add(GameplaySkill->GetSkillGroup(), GameplaySkill->GetSkillIndex());

		SkillIndex++;
	}

	UEODGameInstance* GI = Cast<UEODGameInstance>(CompOwner->GetGameInstance());
	UPlayerSaveGame* SaveGame = GI ? GI->GetCurrentPlayerSaveGameObject() : nullptr;
	if (SaveGame)
	{
		this->SkillBarMap = SaveGame->SkillBarMap;

		TMap<FName, FSkillTreeSlotSaveData> SkillTreeSlotsSaveData = SaveGame->SkillTreeSlotsSaveData;
		TArray<FName> SkillGroups;
		for (FName SkillGroup : SkillGroups)
		{
			FSkillTreeSlotSaveData SlotSaveData = SkillTreeSlotsSaveData[SkillGroup];
			UPlayerSkillBase* PlayerSkill = Cast<UPlayerSkillBase>(GetSkillForSkillGroup(SkillGroup));
			if (PlayerSkill)
			{
				PlayerSkill->SetCurrentUpgrade(SlotSaveData.CurrentUpgrade);
			}
		}		
	}
}

void UPlayerSkillsComponent::OnSkillAddedToSkillBar(uint8 SkillBarIndex, FName SkillGroup)
{
	TArray<uint8> Keys;
	SkillIndexToSkillMap.GetKeys(Keys);

	for (uint8 Key : Keys)
	{
		UGameplaySkillBase* Skill = SkillIndexToSkillMap[Key];
		if (Skill && Skill->GetSkillGroup() == SkillGroup)
		{
			if (SkillBarMap.Contains(SkillBarIndex))
			{
				SkillBarMap[SkillBarIndex] = Key;
			}
			else
			{
				SkillBarMap.Add(SkillBarIndex, Key);
			}

			break;
		}
	}

	AEODCharacterBase* CharOwner = GetCharacterOwner();
	UEODGameInstance* GI = CharOwner ? Cast<UEODGameInstance>(CharOwner->GetGameInstance()) : nullptr;
	UPlayerSaveGame* SaveGame = GI ? GI->GetCurrentPlayerSaveGameObject() : nullptr;
	if (SaveGame)
	{
		SaveGame->SkillBarMap = this->SkillBarMap;
		UGameplayStatics::SaveGameToSlot(SaveGame, GI->GetCurrentPlayerSaveGameName(), GI->PlayerIndex);
	}
}

void UPlayerSkillsComponent::TriggerSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
	AEODCharacterBase* CharOwner = GetCharacterOwner();

	check(SkillIndexToSkillMap.Contains(SkillIndex));
	UPlayerSkillBase* PlayerSkill = Skill ? Cast<UPlayerSkillBase>(Skill) : Cast<UPlayerSkillBase>(SkillIndexToSkillMap[SkillIndex]);

	if (!PlayerSkill || !CharOwner)
	{
		return;
	}

	bool bIsLocalPlayerController = CharOwner->Controller && CharOwner->Controller->IsLocalPlayerController();
	if (bIsLocalPlayerController)
	{
		if (PlayerSkill->CanTriggerSkill())
		{
			ResetChainSkill();

			PlayerSkill->TriggerSkill();
			if (CharOwner->Role < ROLE_Authority)
			{
				Server_TriggerSkill(SkillIndex);
			}

			if (PlayerSkill->bSkillCanBeCharged)
			{
				StartChargingSkill();
			}

			if (PlayerSkill->GetSupersedingSkillGroup() != NAME_None)
			{
				ActivateChainSkill(PlayerSkill->GetSupersedingSkillGroup());
			}

			ActivePrecedingChainSkillGroup = LastUsedSkillGroup = Skill->GetSkillGroup();
		}
	}
	else
	{
		PlayerSkill->TriggerSkill();
	}

	//~ @todo Modify the way attack info is stored
	if (!(CharOwner->Role < ROLE_Authority))
	{
		UActiveSkillBase* _Skill = Cast<UActiveSkillBase>(Skill);
		if (_Skill)
		{
			CharOwner->SetAttackInfoFromActiveSkill(_Skill);
		}
	}
}

void UPlayerSkillsComponent::ReleaseSkill(uint8 SkillIndex, UGameplaySkillBase* Skill, float ReleaseDelay)
{
	AEODCharacterBase* CharOwner = GetCharacterOwner();
	uint8 ActualSkillIndex = SkillIndex % 100;

	if (!Skill)
	{
		Skill = SkillIndexToSkillMap[ActualSkillIndex];
	}

	if (!Skill || !CharOwner)
	{
		return;
	}

	bool bIsLocalPlayerController = CharOwner->Controller && CharOwner->Controller->IsLocalPlayerController();
	if (bIsLocalPlayerController)
	{
		if (Skill->CanReleaseSkill())
		{
			FCharacterStateInfo StateInfo(ECharacterState::UsingActiveSkill, SkillIndex);
			StateInfo.NewReplicationIndex = CharOwner->CharacterStateInfo.NewReplicationIndex + 1;
			CharOwner->CharacterStateInfo = StateInfo;

			//~ @note Release delay is only relevant to server and client owner
			Skill->ReleaseSkill(ReleaseDelay);

			if (CharOwner->Role < ROLE_Authority)
			{
				//~ @note Release delay is only relevant to server and client owner
				Server_ReleaseSkill(SkillIndex, ReleaseDelay);
			}

			bSkillCharging = false;
			SkillChargeDuration = 0.f;
		}
	}
	else
	{
		//~ @note Release delay is only relevant to server and client owner
		Skill->ReleaseSkill(ReleaseDelay);
	}
}

void UPlayerSkillsComponent::CancelSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
}

void UPlayerSkillsComponent::CancelAllActiveSkills()
{
}

bool UPlayerSkillsComponent::CanUseAnySkill() const
{
	return false;
}

bool UPlayerSkillsComponent::CanUseSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
	return false;
}

void UPlayerSkillsComponent::UpdateSkillCooldown(FName SkillGroup, float RemainingCooldown)
{
	AEODCharacterBase* CharOwner = GetCharacterOwner();
	// If this skill component doesn't belong to a local player character
	if (!CharOwner || !CharOwner->Controller || !CharOwner->Controller->IsLocalPlayerController())
	{
		return;
	}

	TArray<UContainerWidget*> SkillWidgets = GetAllContainerWidgetsForSkill(SkillGroup);
	
	for (UContainerWidget* Widget : SkillWidgets)
	{
		if (Widget)
		{
			Widget->UpdateCooldown(RemainingCooldown);
		}
	}
}

void UPlayerSkillsComponent::UpdateSkillCooldown(uint8 SkillIndex, float RemainingCooldown)
{
}

TArray<UContainerWidget*> UPlayerSkillsComponent::GetAllContainerWidgetsForSkill(FName SkillGroup) const
{
	AEODPlayerController* PC = GetCharacterOwner() ? Cast<AEODPlayerController>(GetCharacterOwner()->Controller) : nullptr;
	UDynamicSkillBarWidget* SkillBarWidget = PC ? PC->GetSkillBarWidget() : nullptr;

	if (!SkillBarWidget)
	{
		return TArray<UContainerWidget*>();
	}

	int SkillIndex = 0;
	TArray<uint8> SkillsMapKeys;
	SkillIndexToSkillMap.GetKeys(SkillsMapKeys);

	for (uint8 Key : SkillsMapKeys)
	{
		UGameplaySkillBase* Skill = SkillIndexToSkillMap[Key];
		if (Skill && Skill->GetSkillGroup() == SkillGroup)
		{
			SkillIndex = Key;
			break;
		}
	}

	if (SkillIndex == 0)
	{
		return TArray<UContainerWidget*>();
	}

	TArray<uint8> SkillBarMapKeys;
	SkillBarMap.GetKeys(SkillBarMapKeys);

	TArray<uint8> SkillWidgetsIndices;
	for (uint8 Key : SkillBarMapKeys)
	{
		if (SkillBarMap[Key] == SkillIndex)
		{
			SkillWidgetsIndices.Add(Key);
		}
	}

	if (SkillWidgetsIndices.Num() == 0)
	{
		return TArray<UContainerWidget*>();
	}

	TArray<UContainerWidget*> SkillWidgets;

	for (uint8 Key : SkillWidgetsIndices)
	{
		UContainerWidget* Widget = SkillBarWidget->GetContainerAtIndex(Key);
		if (Widget)
		{
			SkillWidgets.Add(Widget);
		}
	}

	return SkillWidgets;
}

UGameplaySkillBase* UPlayerSkillsComponent::GetSkillForSkillGroup(FName SkillGroup) const
{
	TArray<uint8> SkillsMapKeys;
	SkillIndexToSkillMap.GetKeys(SkillsMapKeys);

	for (uint8 Key : SkillsMapKeys)
	{
		UGameplaySkillBase* Skill = SkillIndexToSkillMap[Key];
		if (Skill && Skill->GetSkillGroup() == SkillGroup)
		{
			return Skill;
		}
	}

	return nullptr;
}

uint8 UPlayerSkillsComponent::GetSkillIndexForSkillGroup(FName SkillGroup) const
{
	uint8 SkillIndex = 0;
	TArray<uint8> SkillsMapKeys;
	SkillIndexToSkillMap.GetKeys(SkillsMapKeys);

	for (uint8 Key : SkillsMapKeys)
	{
		UGameplaySkillBase* Skill = SkillIndexToSkillMap[Key];
		if (Skill && Skill->GetSkillGroup() == SkillGroup)
		{
			return Key;
		}
	}

	return SkillIndex;
}

void UPlayerSkillsComponent::ActivateChainSkill(FName SkillGroup)
{
	SkillGroupToSkillIndexMap[SkillGroup];
	uint8 SupersedingSkillIndex = GetSkillIndexForSkillGroup(SkillGroup);

	if (SupersedingSkillIndex != 0)
	{
		UPlayerSkillBase* PlayerSkill = Cast<UPlayerSkillBase>(SkillIndexToSkillMap[SupersedingSkillIndex]);
		UWorld* World = GetWorld();
		if (PlayerSkill && World)
		{
			SupersedingChainSkillGroup = TPair<uint8, uint8>(LastPressedSkillKey, SupersedingSkillIndex);
			float SkillDuration = PlayerSkill->GetSkillDuration();
			float ChainSkillActivationWindow = SkillDuration + ChainSkillResetDelay;
			World->GetTimerManager().SetTimer(ChainSkillTimerHandle, this, &UPlayerSkillsComponent::ResetChainSkill, ChainSkillActivationWindow, false);
		}
	}
}

void UPlayerSkillsComponent::ResetChainSkill()
{
}

void UPlayerSkillsComponent::Server_TriggerSkill_Implementation(uint8 SkillIndex)
{
	AEODCharacterBase* CharOwner = GetCharacterOwner();
	check(CharOwner);

	TriggerSkill(SkillIndex);
}

void UPlayerSkillsComponent::Server_ReleaseSkill_Implementation(uint8 SkillIndex, float ChargeDuration)
{
	AEODCharacterBase* CharOwner = GetCharacterOwner();
	check(CharOwner);

	// FCharacterStateInfo StateInfo(ECharacterState::UsingActiveSkill, SkillIndex);
	// StateInfo.NewReplicationIndex = CharOwner->CharacterStateInfo.NewReplicationIndex + 1;
	// CharOwner->CharacterStateInfo = StateInfo;

	SkillChargeDuration = SkillIndex;
	ReleaseSkill(SkillIndex);
}
