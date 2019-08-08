// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "PlayerSkillsComponent.h"
#include "PlayerCharacter.h"
#include "EODGameInstance.h"
#include "GameplaySkillBase.h"
#include "ActiveSkillBase.h"
#include "EODPlayerController.h"
#include "ContainerWidget.h"
#include "PlayerStatsComponent.h"
#include "GameplayEffectBase.h"
#include "HUDWidget.h"
#include "SkillTreeWidget.h"
#include "SkillBarWidget.h"
#include "SkillBarContainerWidget.h"

#include "Engine/World.h"
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

	// Do not call TriggerSkill if Skill is nullptr
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

bool UPlayerSkillsComponent::AddSkillToSkillBar(uint8 SkillBarIndex, FName SkillGroup)
{
	uint8 SkillIndex = SkillGroupToSkillIndexMap.Contains(SkillGroup) ? SkillGroupToSkillIndexMap[SkillGroup] : 0;
	if (SkillIndex == 0 || !SkillIndexToSkillMap.Contains(SkillIndex))
	{
		return false;
	}

	SkillBarMap.Add(SkillBarIndex, SkillIndex);
	return SaveSkillBarMap();
}

bool UPlayerSkillsComponent::RemoveSkillFromSkillBar(uint8 SkillBarIndex, FName SkillGroup)
{
	SkillBarMap.Remove(SkillBarIndex);
	return SaveSkillBarMap();
}

bool UPlayerSkillsComponent::SwapSkillsOnSkillBar(uint8 SBI1, FName SG1, uint8 SBI2, FName SG2)
{
	check(SBI1 != SBI2);

	uint8 SI1 = SkillGroupToSkillIndexMap.Contains(SG1) ? SkillGroupToSkillIndexMap[SG1] : 0;
	uint8 SI2 = SkillGroupToSkillIndexMap.Contains(SG2) ? SkillGroupToSkillIndexMap[SG2] : 0;
	if (SI1 == 0 || SI2 == 0 || !SkillIndexToSkillMap.Contains(SI1) || !SkillIndexToSkillMap.Contains(SI2))
	{
		return false;
	}

	SkillBarMap.Add(SBI1, SI2);
	SkillBarMap.Add(SBI2, SI1);

	return SaveSkillBarMap();
}

UPlayerSkillBase const* const UPlayerSkillsComponent::GetSkillAtSkillBarIndex(uint8 SkillBarIndex) const
{
	uint8 SkillIndex = SkillBarMap.Contains(SkillBarIndex) ? SkillBarMap[SkillBarIndex] : 0;
	if (SkillIndex == 0 || !SkillIndexToSkillMap.Contains(SkillIndex))
	{
		return nullptr;
	}

	return Cast<UPlayerSkillBase>(SkillIndexToSkillMap[SkillIndex]);
}

bool UPlayerSkillsComponent::SaveSkillBarMap()
{
	AEODCharacterBase* CharOwner = GetCharacterOwner();
	UEODGameInstance* GI = CharOwner ? Cast<UEODGameInstance>(CharOwner->GetGameInstance()) : nullptr;
	UPlayerSaveGame* SaveGame = GI ? GI->GetCurrentPlayerSaveGameObject() : nullptr;
	if (SaveGame)
	{
		SaveGame->SkillBarMap = this->SkillBarMap;
		UGameplayStatics::SaveGameToSlot(SaveGame, GI->GetCurrentPlayerSaveGameName(), GI->PlayerIndex);
		return true;
	}

	return false;
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
			if (CharOwner->IsBlocking())
			{
				CharOwner->StopBlockingAttacks();
			}

			// If this skill is a chain skill, it may have been activated before the previous skill finished.
			// We need to finish/cancel previous skills before using this
			//~ @todo replace cancel all active skills with something that force finishes all active skills
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

			OnSkillTriggered(SkillIndex, PlayerSkill->GetSkillGroup(), PlayerSkill);
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
						if (StatsComponent->Stamina.GetCurrentValue() < LevelUpInfo.StaminaCost)
						{
							PlayerChar->PlaySystemSound(PlayerChar->SystemSounds.NotEnoughStamina);
						}
						else if (StatsComponent->Mana.GetCurrentValue() < LevelUpInfo.ManaCost)
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

		OnSkillTriggered(SkillIndex, PlayerSkill->GetSkillGroup(), PlayerSkill);
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

			OnSkillReleased(SkillIndex, PlayerSkill->GetSkillGroup(), PlayerSkill);
		}
	}
	else
	{
		//~ @note Release delay is only relevant to server and client owner
		Skill->ReleaseSkill(ReleaseDelay);

		OnSkillReleased(SkillIndex, PlayerSkill->GetSkillGroup(), PlayerSkill);
	}
}

void UPlayerSkillsComponent::CancelSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
	Super::CancelSkill(SkillIndex, Skill);
}

void UPlayerSkillsComponent::CancelAllActiveSkills()
{
	Super::CancelAllActiveSkills();
}

bool UPlayerSkillsComponent::CanUseAnySkill() const
{
	return Super::CanUseAnySkill();
}

bool UPlayerSkillsComponent::CanUseSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
	return Super::CanUseSkill(SkillIndex, Skill);
}

void UPlayerSkillsComponent::UpdateSkillCooldown(FName SkillGroup, float RemainingCooldown)
{
	AEODCharacterBase* CharOwner = GetCharacterOwner();
	// If this skill component doesn't belong to a local player character
	if (!CharOwner || !CharOwner->Controller || !CharOwner->Controller->IsLocalPlayerController())
	{
		return;
	}

	TArray<UContainerWidgetBase*> SkillWidgets = GetAllContainerWidgetsForSkill(SkillGroup);
	for (UContainerWidgetBase* Widget : SkillWidgets)
	{
		check(Widget);
		Widget->SetCooldownValue(RemainingCooldown);
	}
}

void UPlayerSkillsComponent::UpdateSkillCooldown(uint8 SkillIndex, float RemainingCooldown)
{
	UGameplaySkillBase* Skill = SkillIndexToSkillMap.Contains(SkillIndex) ? SkillIndexToSkillMap[SkillIndex] : nullptr;
	if (Skill)
	{
		UpdateSkillCooldown(Skill->GetSkillGroup(), RemainingCooldown);
	}
}

void UPlayerSkillsComponent::AddGameplayEffect(UGameplayEffectBase* GameplayEffect)
{
	Super::AddGameplayEffect(GameplayEffect);
	if (GameplayEffect && GameplayEffect->Icon && HUDWidget)
	{
		HUDWidget->AddGameplayEffectUI(GameplayEffect);
	}
}

void UPlayerSkillsComponent::RemoveGameplayEffect(UGameplayEffectBase* GameplayEffect)
{
	Super::RemoveGameplayEffect(GameplayEffect);
	if (GameplayEffect && GameplayEffect->Icon && HUDWidget)
	{
		HUDWidget->RemoveGameplayEffectUI(GameplayEffect);
	}
}

TArray<uint8> UPlayerSkillsComponent::GetSkillBarIndicesOfSkillGroup(FName SkillGroup)
{
	uint8 SkillIndex = GetSkillIndexForSkillGroup(SkillGroup);
	
	TArray<uint8> Keys;
	SkillBarMap.GetKeys(Keys);

	TArray<uint8> SkillBarIndices;
	for (uint8 Key : Keys)
	{
		if (SkillBarMap[Key] == SkillIndex)
		{
			SkillBarIndices.AddUnique(Key);
		}
	}

	return SkillBarIndices;
}

TArray<UContainerWidgetBase*> UPlayerSkillsComponent::GetAllContainerWidgetsForSkill(FName SkillGroup, bool bFastSearch)
{
	if (bFastSearch)
	{
		UPlayerSkillBase* PlayerSkill = SkillGroupToSkillMap.Contains(SkillGroup) ? Cast<UPlayerSkillBase>(SkillGroupToSkillMap[SkillGroup]) : nullptr;
		if (PlayerSkill)
		{
			return PlayerSkill->GetRegisteredWidgets();
		}
	}
	else
	{
		check(SBWidget);
		TArray<uint8> SkillBarIndices = GetSkillBarIndicesOfSkillGroup(SkillGroup);

		TArray<UContainerWidgetBase*> ContainerWidgets;
		for (uint8 SkillBarIndex : SkillBarIndices)
		{
			UContainerWidgetBase* Cont = SBWidget->GetContainerAtIndex(SkillBarIndex);
			if (Cont)
			{
				ContainerWidgets.AddUnique(Cont);
			}
		}

		return ContainerWidgets;
	}

	return TArray<UContainerWidgetBase*>();
}

void UPlayerSkillsComponent::ActivateChainSkill(UGameplaySkillBase* CurrentSkill)
{
	UPlayerSkillBase* PlayerSkill = Cast<UPlayerSkillBase>(CurrentSkill);
	if (PlayerSkill == nullptr || PlayerSkill->GetSupersedingSkillGroup() == NAME_None)
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

		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(ChainSkillTimerHandle, this, &UPlayerSkillsComponent::ResetChainSkill, ChainSkillActivationWindow, false);

		SupersedingSkill->OnActivatedAsChainSkill();

		if (SupersedingSkill->ShouldUIActivate())
		{
			AEODCharacterBase* CharOwner = GetCharacterOwner();
			AEODPlayerController* PC = CharOwner ? Cast<AEODPlayerController>(CharOwner->Controller) : nullptr;
			if (PC)
			{
				FString ActionName = TEXT("Skill_") + FString::FromInt(LastPressedSkillKey);
				FName KeyName = PC->GetKeyNameForActionName(FName(*ActionName));
				if (KeyName == NAME_None)
				{
					PrintWarning(TEXT("Key name is invalid"));
				}
				else
				{
					PC->RegisterPopupWidget(this, KeyName.ToString(), SupersedingSkill->GetInGameSkillName(), SupersedingSkill->GetSkillIcon());
				}
			}
		}
	}
}

void UPlayerSkillsComponent::OnPlayerWeaponChanged()
{
	TArray<uint8> Keys;
	SkillBarMap.GetKeys(Keys);

	TArray<UPlayerSkillBase*> SkillsOnSkillBar;
	for (uint8 Key : Keys)
	{
		uint8 SkillKey = SkillBarMap[Key];
		UPlayerSkillBase* Skill = SkillBarMap.Contains(Key) ? Cast<UPlayerSkillBase>(SkillIndexToSkillMap[SkillKey]) : nullptr;
		if (Skill)
		{
			SkillsOnSkillBar.AddUnique(Skill);
		}
	}

	for (UPlayerSkillBase* Skill : SkillsOnSkillBar)
	{
		check(Skill);
		Skill->RefreshWidgets();
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

	AEODCharacterBase* CharOwner = GetCharacterOwner();
	AEODPlayerController* PC = CharOwner ? Cast<AEODPlayerController>(CharOwner->Controller) : nullptr;
	if (PC)
	{
		PC->UnregisterPopupWidget(this);
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

void UPlayerSkillsComponent::InitializeSkills(AEODCharacterBase* CompOwner)
{
	// If there are already skills in Skill Map
	if (SkillIndexToSkillMap.Num() > 0)
	{
		VerifySkillsInitializedCorrectly();
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

	FString ContextString = TEXT(__FUNCTION__);
	ContextString += TEXT(" - Looking for player skill class");

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
	
		if (GameplaySkill->GetSkillGroup() == NAME_None)
		{
			GameplaySkill->SetSkillGroup(Key);
		}
		else
		{
			check(GameplaySkill->GetSkillGroup() == Key);
		}

		SkillIndexToSkillMap.Add(GameplaySkill->GetSkillIndex(), GameplaySkill);
		SkillGroupToSkillMap.Add(GameplaySkill->GetSkillGroup(), GameplaySkill);
		SkillGroupToSkillIndexMap.Add(GameplaySkill->GetSkillGroup(), GameplaySkill->GetSkillIndex());

		SkillIndex++;
	}

	UnlockPlayerSkillsFromSaveGame(CompOwner);
}

void UPlayerSkillsComponent::VerifySkillsInitializedCorrectly()
{
	check(SkillsDataTable != nullptr);

	TArray<FName> Keys = SkillsDataTable->GetRowNames();

	int32 Num1 = SkillIndexToSkillMap.Num();
	int32 Num2 = SkillGroupToSkillMap.Num();
	int32 Num3 = SkillGroupToSkillIndexMap.Num();
	int32 KeyNum = Keys.Num();

	check(Num1 == Num2 && Num2 == Num3);
	check(Num1 == KeyNum);

	//~ @todo [optional] check the skills have been constructed correctly
}

void UPlayerSkillsComponent::UnlockPlayerSkillsFromSaveGame(AEODCharacterBase* CompOwner)
{
	check(CompOwner);

	UEODGameInstance* GI = Cast<UEODGameInstance>(CompOwner->GetGameInstance());
	UPlayerSaveGame* SaveGame = GI ? GI->GetCurrentPlayerSaveGameObject() : nullptr;
	if (SaveGame)
	{
		this->SkillBarMap = SaveGame->SkillBarMap;
		TMap<FName, FSkillTreeSlotSaveData> SkillTreeSlotsSaveData = SaveGame->SkillTreeSlotsSaveData;

		TArray<FName> SkillGroups;
		SkillTreeSlotsSaveData.GetKeys(SkillGroups);
		for (FName SkillGroup : SkillGroups)
		{
			FSkillTreeSlotSaveData& SlotSaveData = SkillTreeSlotsSaveData[SkillGroup];
			UPlayerSkillBase* PlayerSkill = Cast<UPlayerSkillBase>(GetSkillForSkillGroup(SkillGroup));
			if (PlayerSkill)
			{
				PlayerSkill->UnlockSkill(SlotSaveData.CurrentUpgrade);
			}
		}
	}
}

UPlayerSaveGame* UPlayerSkillsComponent::GetPlayerSaveGame() const
{
	UWorld* World = GetWorld();
	UEODGameInstance* EODGI = World ? Cast<UEODGameInstance>(World->GetGameInstance()) : nullptr;
	UPlayerSaveGame* SaveGame = EODGI ? EODGI->GetCurrentPlayerSaveGameObject() : nullptr;
	return SaveGame;
}

bool UPlayerSkillsComponent::IsAnySkillPointAvailable() const
{
	UPlayerSaveGame* SaveGame = GetPlayerSaveGame();
	if (SaveGame)
	{
		FSkillPointsAllocationInfo& SPAInfo = SaveGame->SkillPointsAllocationInfo;
		if (SPAInfo.AvailableSkillPoints > 0 || (SPAInfo.AvailableSkillPoints + SPAInfo.UsedSkillPoints) > SkillPointsUnlockedByDefault)
		{
			return true;
		}
	}
	return false;
}

bool UPlayerSkillsComponent::AttemptPointAllocationToSlot(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo)
{
	FSkillTreeSlot* SkillTreeSlot = SkillSlotInfo;
	if (SkillTreeSlot == nullptr)
	{
		FString ContextString = TEXT(__FUNCTION__);
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

bool UPlayerSkillsComponent::IsAnySkillPointAllocatedToSlot(FName SkillGroup)
{
	UPlayerSaveGame* SaveGame = GetPlayerSaveGame();
	if (SaveGame->SkillTreeSlotsSaveData.Contains(SkillGroup))
	{
		FSkillTreeSlotSaveData& SaveData = SaveGame->SkillTreeSlotsSaveData[SkillGroup];
		if (SaveData.CurrentUpgrade > 0)
		{
			return true;
		}
	}
	return false;
}

bool UPlayerSkillsComponent::IsSkillAvailable(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo)
{
	if (SkillGroup == NAME_None)
	{
		return false;
	}

	FSkillTreeSlot* SkillTreeSlot = SkillSlotInfo;
	if (SkillTreeSlot == nullptr && SkillTreeLayoutTable)
	{
		FString ContextString = TEXT(__FUNCTION__);
		SkillTreeSlot = SkillTreeLayoutTable->FindRow<FSkillTreeSlot>(SkillGroup, ContextString);
	}

	UPlayerSaveGame* SaveGame = GetPlayerSaveGame();

	// If skil tree slot was not found
	if (SkillTreeSlot == nullptr || SaveGame == nullptr)
	{
		return false;
	}

	int32 CurrentSkillGroupUpgrade = SaveGame->SkillTreeSlotsSaveData.Contains(SkillGroup) ? SaveGame->SkillTreeSlotsSaveData[SkillGroup].CurrentUpgrade : 0;
	// If the skill upgrade is already maxxed out
	if (CurrentSkillGroupUpgrade == SkillTreeSlot->MaxUpgrades)
	{
		return false;
	}

	int32 UsedSkillPointsRequired = SkillTreeSlot->MinimumPointsToUnlock + CurrentSkillGroupUpgrade * SkillTreeSlot->UpgradePointsGap;
	int32 AllocatedPoints = 0;
	switch (SkillTreeSlot->Vocation)
	{
	case EVocations::Assassin:
		AllocatedPoints = SaveGame->SkillPointsAllocationInfo.AssassinPoints;
		break;
	case EVocations::Berserker:
		AllocatedPoints = SaveGame->SkillPointsAllocationInfo.BerserkerPoints;
		break;
	case EVocations::Cleric:
		AllocatedPoints = SaveGame->SkillPointsAllocationInfo.ClericPoints;
		break;
	case EVocations::Defender:
		AllocatedPoints = SaveGame->SkillPointsAllocationInfo.DefenderPoints;
		break;
	case EVocations::Sorcerer:
		AllocatedPoints = SaveGame->SkillPointsAllocationInfo.SorcererPoints;
		break;
	default:
		break;
	}

	if (AllocatedPoints < UsedSkillPointsRequired)
	{
		return false;
	}

	if (SkillTreeSlot->SkillRequiredToUnlock != NAME_None)
	{
		int32 UnlockSkillUpgradeLevel = SaveGame->SkillTreeSlotsSaveData.Contains(SkillTreeSlot->SkillRequiredToUnlock) ? SaveGame->SkillTreeSlotsSaveData[SkillTreeSlot->SkillRequiredToUnlock].CurrentUpgrade : 0;
		if (UnlockSkillUpgradeLevel == 0)
		{
			return false;
		}
	}

	return true;
}

bool UPlayerSkillsComponent::CanAllocatePointToSlot(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo)
{
	if (IsSkillAvailable(SkillGroup, SkillSlotInfo) && IsAnySkillPointAvailable())
	{
		return true;
	}

	return false;
}

void UPlayerSkillsComponent::SetAllocatedPointsAssassin(int32 Value)
{
	UPlayerSaveGame* SaveGame = GetPlayerSaveGame();
	if (SaveGame == nullptr)
	{
		return;
	}

	SaveGame->SkillPointsAllocationInfo.AssassinPoints = Value;
	if (SPIWidget)
	{
		SPIWidget->UpdateAssassinPointsText(Value);
	}
}

void UPlayerSkillsComponent::SetAllocatedPointsBerserker(int32 Value)
{
	UPlayerSaveGame* SaveGame = GetPlayerSaveGame();
	if (SaveGame == nullptr)
	{
		return;
	}

	SaveGame->SkillPointsAllocationInfo.BerserkerPoints = Value;
	if (SPIWidget)
	{
		SPIWidget->UpdateBerserkerPointsText(Value);
	}
}

void UPlayerSkillsComponent::SetAllocatedPointsCleric(int32 Value)
{
	UPlayerSaveGame* SaveGame = GetPlayerSaveGame();
	if (SaveGame == nullptr)
	{
		return;
	}

	SaveGame->SkillPointsAllocationInfo.ClericPoints = Value;
	if (SPIWidget)
	{
		SPIWidget->UpdateClericPointsText(Value);
	}
}

void UPlayerSkillsComponent::SetAllocatedPointsDefender(int32 Value)
{
	UPlayerSaveGame* SaveGame = GetPlayerSaveGame();
	if (SaveGame == nullptr)
	{
		return;
	}

	SaveGame->SkillPointsAllocationInfo.DefenderPoints = Value;
	if (SPIWidget)
	{
		SPIWidget->UpdateDefenderPointsText(Value);
	}
}

void UPlayerSkillsComponent::SetAllocatedPointsSorcerer(int32 Value)
{
	UPlayerSaveGame* SaveGame = GetPlayerSaveGame();
	if (SaveGame == nullptr)
	{
		return;
	}

	SaveGame->SkillPointsAllocationInfo.SorcererPoints = Value;
	if (SPIWidget)
	{
		SPIWidget->UpdateSorcererPointsText(Value);
	}
}

void UPlayerSkillsComponent::SetAvailableSkillPoints(int32 Value)
{
	UPlayerSaveGame* SaveGame = GetPlayerSaveGame();
	if (SaveGame == nullptr)
	{
		return;
	}

	SaveGame->SkillPointsAllocationInfo.AvailableSkillPoints = Value;
	if (SPIWidget)
	{
		SPIWidget->UpdateAvailableSkillPointsText(Value);
	}
}

void UPlayerSkillsComponent::SetUsedSkillPoints(int32 Value)
{
	UPlayerSaveGame* SaveGame = GetPlayerSaveGame();
	if (SaveGame == nullptr)
	{
		return;
	}

	SaveGame->SkillPointsAllocationInfo.UsedSkillPoints = Value;
	if (SPIWidget)
	{
		SPIWidget->UpdateUsedSkillPointsText(Value);
	}
}

void UPlayerSkillsComponent::ModifySkillSlotUpgrade(FName SkillGroup, int32 Value)
{
	UPlayerSaveGame* SaveGame = GetPlayerSaveGame();
	if (SaveGame == nullptr)
	{
		return;
	}

	if (SaveGame->SkillTreeSlotsSaveData.Contains(SkillGroup))
	{
		FSkillTreeSlotSaveData& SaveData = SaveGame->SkillTreeSlotsSaveData[SkillGroup];
		SetSkillSlotUpgrade(SkillGroup, SaveData.CurrentUpgrade + Value);
	}
	else
	{
		FSkillTreeSlotSaveData SaveData;
		SaveData.CurrentUpgrade += Value;
		SaveGame->SkillTreeSlotsSaveData.Add(SkillGroup, SaveData);
		SetSkillSlotUpgrade(SkillGroup, SaveData.CurrentUpgrade);
	}

	UWorld* World = GetWorld();
	UEODGameInstance* EODGI = World ? Cast<UEODGameInstance>(World->GetGameInstance()) : nullptr;
	if (EODGI)
	{
		UGameplayStatics::SaveGameToSlot(SaveGame, EODGI->GetCurrentPlayerSaveGameName(), EODGI->PlayerIndex);
	}
}

void UPlayerSkillsComponent::SetSkillSlotUpgrade(FName SkillGroup, int32 Value)
{
	UPlayerSaveGame* SaveGame = GetPlayerSaveGame();
	check(SaveGame);

	check(SaveGame->SkillTreeSlotsSaveData.Contains(SkillGroup));
	FSkillTreeSlotSaveData& SaveData = SaveGame->SkillTreeSlotsSaveData[SkillGroup];
	SaveData.CurrentUpgrade = Value;

	USkillTreeContainerWidget* SlotWidget = STWidget ? STWidget->GetSkillSlotForSkillGroup(SkillGroup) : nullptr;
	if (SlotWidget)
	{
		SlotWidget->SetCurrentValue(Value);
	}

	UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(SkillGroupToSkillMap[SkillGroup]);
	if (Skill)
	{
		Skill->SetCurrentUpgrade(Value);
	}
}
