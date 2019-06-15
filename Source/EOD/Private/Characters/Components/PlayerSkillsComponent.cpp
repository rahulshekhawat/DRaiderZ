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
#include "GameplayEffectBase.h"
#include "DynamicHUDWidget.h"

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

void UPlayerSkillsComponent::AddGameplayEffect(UGameplayEffectBase* GameplayEffect)
{
	Super::AddGameplayEffect(GameplayEffect);
	if (GameplayEffect && GameplayEffect->Icon)
	{
		AEODCharacterBase* EODChar = GetCharacterOwner();
		AEODPlayerController* EODPC = EODChar ? Cast<AEODPlayerController>(EODChar->Controller) : nullptr;
		UDynamicHUDWidget* HUDWidget = EODPC ? EODPC->GetHUDWidget() : nullptr;

		if (HUDWidget)
		{
			HUDWidget->AddGameplayEffectUI(GameplayEffect);
		}
	}
}

void UPlayerSkillsComponent::RemoveGameplayEffect(UGameplayEffectBase* GameplayEffect)
{
	Super::RemoveGameplayEffect(GameplayEffect);
	if (GameplayEffect && GameplayEffect->Icon)
	{
		AEODCharacterBase* EODChar = GetCharacterOwner();
		AEODPlayerController* EODPC = EODChar ? Cast<AEODPlayerController>(EODChar->Controller) : nullptr;
		UDynamicHUDWidget* HUDWidget = EODPC ? EODPC->GetHUDWidget() : nullptr;

		if (HUDWidget)
		{
			HUDWidget->RemoveGameplayEffectUI(GameplayEffect);
		}
	}
}

TArray<UContainerWidget*> UPlayerSkillsComponent::GetAllContainerWidgetsForSkill(FName SkillGroup) 
{
	AEODCharacterBase* CharOwner = GetCharacterOwner();
	AEODPlayerController* PC = CharOwner ? Cast<AEODPlayerController>(CharOwner->Controller) : nullptr;
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
