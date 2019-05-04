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
#include "PlayerStatsComponent.h"

#include "TimerManager.h"
#include "UnrealNetwork.h"
#include "Components/Image.h"
#include "Components/AudioComponent.h"
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
		ReleaseSkill(SkillIndex, Skill, SkillChargeDuration);
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

	check(CompOwner);
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
		UGameplaySkillBase* GameplaySkill = NewObject<UGameplaySkillBase>(this, Row->SkillClass, Key, RF_Transient);
		check(GameplaySkill);
		GameplaySkill->InitSkill(CompOwner, CompOwner->Controller);
		GameplaySkill->SetSkillIndex(SkillIndex);

		SkillIndexToSkillMap.Add(GameplaySkill->GetSkillIndex(), GameplaySkill);
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
	uint8 SkillIndex = SkillGroupToSkillIndexMap.Contains(SkillGroup) ? SkillGroupToSkillIndexMap[SkillGroup] : 0;
	if (SkillIndex == 0 || !SkillIndexToSkillMap.Contains(SkillIndex))
	{
		return;
	}

	if (SkillBarMap.Contains(SkillBarIndex))
	{
		SkillBarMap[SkillBarIndex] = SkillIndex;
	}
	else
	{
		SkillBarMap.Add(SkillBarIndex, SkillIndex);
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

void UPlayerSkillsComponent::OnSkillRemovedFromSkillBar(uint8 SkillBarIndex, FName SkillGroup)
{
	uint8 SkillIndex = SkillGroupToSkillIndexMap.Contains(SkillGroup) ? SkillGroupToSkillIndexMap[SkillGroup] : 0;
	if (SkillIndex == 0 || !SkillIndexToSkillMap.Contains(SkillIndex))
	{
		return;
	}

	if (SkillBarMap.Contains(SkillBarIndex))
	{
		SkillBarMap.Remove(SkillBarIndex);
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

	check(PlayerSkill);
	check(CharOwner);

	// if (!PlayerSkill || !CharOwner) { return; }

	bool bIsLocalPlayerController = CharOwner->Controller && CharOwner->Controller->IsLocalPlayerController();
	if (bIsLocalPlayerController)
	{
		if (PlayerSkill->CanTriggerSkill())
		{
			// If this skill is a chain skill, it may have been activated before the previous skill finished.
			// We need to finish/cancel previous skills before using this
			CancelAllActiveSkills();

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
				ActivateChainSkill(PlayerSkill);
			}

			ActivePrecedingChainSkillGroup = LastUsedSkillGroup = PlayerSkill->GetSkillGroup();
			LastUsedSkillIndex = SkillIndex;
			ActiveSkills.Add(PlayerSkill);
			SetCanUseChainSkill(false);
		}
		else
		{
			APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(CharOwner);
			if (PlayerChar && (PlayerChar->IsIdleOrMoving() || PlayerChar->IsNormalAttacking() || PlayerChar->IsBlocking()))
			{
				UActiveSkillBase* ActiveSkill = Cast<UActiveSkillBase>(PlayerSkill);
				if (ActiveSkill)
				{
					const FActiveSkillLevelUpInfo LevelUpInfo = ActiveSkill->GetCurrentSkillLevelupInfo();
					AEODPlayerController* PC = Cast<AEODPlayerController>(CharOwner->Controller);
					UPlayerStatsComponent* StatsComponent = PC->GetStatsComponent();
					if (StatsComponent)
					{
						if (StatsComponent->GetCurrentStamina() < LevelUpInfo.StaminaCost)
						{
							PlayerChar->PlaySystemSound(PlayerChar->SystemSounds.NotEnoughStamina);
						}
						else if (StatsComponent->GetCurrentMana() < LevelUpInfo.ManaCost)
						{
							PlayerChar->PlaySystemSound(PlayerChar->SystemSounds.NotEnoughEnergy);
						}
						else
						{
							PlayerChar->PlaySystemSound(PlayerChar->SystemSounds.SkillNotAvailable);
						}
					}
				}
				else
				{
					PlayerChar->PlaySystemSound(PlayerChar->SystemSounds.SkillNotAvailable);
				}
			}
		}
	}
	else
	{
		CancelAllActiveSkills();

		PlayerSkill->TriggerSkill();
		LastUsedSkillGroup = PlayerSkill->GetSkillGroup();
		LastUsedSkillIndex = SkillIndex;
		ActiveSkills.Add(PlayerSkill);
	}

	//~ @todo Modify the way attack info is stored
	if (!(CharOwner->Role < ROLE_Authority))
	{
		UActiveSkillBase* _Skill = Cast<UActiveSkillBase>(PlayerSkill);
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

	UPlayerSkillBase* PlayerSkill = Skill ? Cast<UPlayerSkillBase>(Skill) : Cast<UPlayerSkillBase>(SkillIndexToSkillMap[ActualSkillIndex]);

	check(PlayerSkill);
	check(CharOwner);

	// if (!Skill || !CharOwner) { return; }

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

			StopChargingSkill();
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
	Super::CancelAllActiveSkills();
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

void UPlayerSkillsComponent::ActivateChainSkill(UGameplaySkillBase* CurrentSkill)
{
	UPlayerSkillBase* PlayerSkill = Cast<UPlayerSkillBase>(CurrentSkill);
	UWorld* World = GetWorld();
	if (!PlayerSkill || !World)
	{
		return;
	}

	if (PlayerSkill->GetSupersedingSkillGroup() == NAME_None)
	{
		return;
	}

	uint8 SupersedingSkillIndex = GetSkillIndexForSkillGroup(PlayerSkill->GetSupersedingSkillGroup());
	UPlayerSkillBase* SupersedingSkill = SkillIndexToSkillMap.Contains(SupersedingSkillIndex) ? Cast<UPlayerSkillBase>(SkillIndexToSkillMap[SupersedingSkillIndex]) : nullptr;

	if (SupersedingSkill)
	{
		SupersedingChainSkillGroup = TPair<uint8, uint8>(LastPressedSkillKey, SupersedingSkillIndex);
		float SkillDuration = PlayerSkill->GetSkillDuration();
		float ChainSkillActivationWindow = SkillDuration + ChainSkillResetDelay;
		World->GetTimerManager().SetTimer(ChainSkillTimerHandle, this, &UPlayerSkillsComponent::ResetChainSkill, ChainSkillActivationWindow, false);

		SupersedingSkill->OnActivatedAsChainSkill();
	}
}

void UPlayerSkillsComponent::OnPlayerWeaponChanged()
{
	AEODPlayerController* PC = GetCharacterOwner() ? Cast<AEODPlayerController>(GetCharacterOwner()->Controller) : nullptr;
	UDynamicSkillBarWidget* SkillBarWidget = PC ? PC->GetSkillBarWidget() : nullptr;

	if (!SkillBarWidget)
	{
		return;
	}

	TArray<uint8> Keys;
	SkillBarMap.GetKeys(Keys);

	for (uint8 Key : Keys)
	{
		uint8 SkillKey = SkillBarMap[Key];
		if (SkillIndexToSkillMap.Contains(SkillKey))
		{
			UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(SkillIndexToSkillMap[SkillKey]);
			UContainerWidget* Cont = SkillBarWidget->GetContainerAtIndex(Key);
			if (Skill && Cont)
			{
				if (Skill->CanPlayerActivateThisSkill())
				{
					Cont->ItemImage->SetIsEnabled(true);
					Cont->SetCanBeClicked(true);
				}
				else
				{
					Cont->ItemImage->SetIsEnabled(false);
					Cont->SetCanBeClicked(false);
				}
			}
		}
	}
}

void UPlayerSkillsComponent::ResetChainSkill()
{
	if (SupersedingChainSkillGroup.Value != 0)
	{
		check(SkillIndexToSkillMap.Contains(SupersedingChainSkillGroup.Value));
		UPlayerSkillBase* PlayerSkill = Cast<UPlayerSkillBase>(SkillIndexToSkillMap[SupersedingChainSkillGroup.Value]);
		PlayerSkill->OnDeactivatedAsChainSkill();
	}

	Super::ResetChainSkill();
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
