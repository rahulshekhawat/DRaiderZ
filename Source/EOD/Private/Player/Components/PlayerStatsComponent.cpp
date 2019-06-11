// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerStatsComponent.h"
#include "StatusIndicatorWidget.h"
#include "PlayerCharacter.h"
#include "EODGameInstance.h"
#include "PlayerSaveGame.h"

#include "Engine/World.h"
#include "UnrealNetwork.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetSystemLibrary.h"

UPlayerStatsComponent::UPlayerStatsComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	bHasHealthRegenration			= true;
	bHasManaRegenration				= true;
	bHasStaminaRegenration			= true;
}

void UPlayerStatsComponent::PostInitProperties()
{
	Super::PostInitProperties();
}

void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadPlayerStats();

	// @todo load damage and resistance stats
	ActivateHealthRegeneration();
	ActivateManaRegeneration();
	ActivateStaminaRegeneration();
}

void UPlayerStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UPlayerStatsComponent::AddPrimaryWeaponStats(FWeaponTableRow* WeaponData)
{
	if (PrimaryWeaponData && PrimaryWeaponData == WeaponData)
	{
		return;
	}
	else if (PrimaryWeaponData && PrimaryWeaponData != WeaponData)
	{
		RemovePrimaryWeaponStats();
	}

	PrimaryWeaponData = WeaponData;
}

void UPlayerStatsComponent::AddSecondaryWeaponStats(FWeaponTableRow* WeaponData)
{
	if (SecondaryWeaponData && SecondaryWeaponData == WeaponData)
	{
		return;
	}
	else if (SecondaryWeaponData && SecondaryWeaponData != WeaponData)
	{
		RemoveSecondaryWeaponStats();
	}

	SecondaryWeaponData = WeaponData;
}

void UPlayerStatsComponent::RemovePrimaryWeaponStats()
{
	PrimaryWeaponData = nullptr;
}

void UPlayerStatsComponent::RemoveSecondaryWeaponStats()
{
	SecondaryWeaponData = nullptr;
}

void UPlayerStatsComponent::AddArmorStats(FArmorTableRow* ArmorData)
{
	if (ArmorsData.Contains(ArmorData->ArmorType))
	{
		RemoveArmorStats(ArmorData->ArmorType);
	}

	ArmorsData.Add(ArmorData->ArmorType, ArmorData);

	//~ @todo add armor stats to player stats
}

void UPlayerStatsComponent::RemoveArmorStats(EArmorType ArmorType)
{
	ArmorsData.Remove(ArmorType);

	//~ @todo remove armor stats from player stats
}

void UPlayerStatsComponent::LoadPlayerStats()
{
	UWorld* World = GetWorld();
	UEODGameInstance* EODGI = World ? Cast<UEODGameInstance>(World->GetGameInstance()) : nullptr;
	UPlayerSaveGame* SaveGame = EODGI ? EODGI->GetCurrentPlayerSaveGameObject() : nullptr;
	UDataTable* PSDataTable = EODGI ? EODGI->PlayerStatsDataTable : nullptr;

	if (!SaveGame || !PSDataTable)
	{
		return;
	}
	
	int32 PlayerLevel = SaveGame->CharacterLevel;
	PlayerLevel = PlayerLevel <= 0 ? 1 : PlayerLevel;
	PlayerLevel = PlayerLevel >= 99 ? 99 : PlayerLevel;
	FName LevelName = FName(*FString::FromInt(PlayerLevel));

	FPlayerStatsTableRow* TableRow = PSDataTable->FindRow<FPlayerStatsTableRow>(LevelName, FString("UPlayerStatsComponent::LoadPlayerStats()"));
	if (!TableRow)
	{
		return;
	}

	Health.SetMaxValue(TableRow->Health);
	Health.ForceBroadcastDelegate();

	Mana.SetMaxValue(TableRow->Mana);
	Mana.ForceBroadcastDelegate();

	Stamina.SetMaxValue(TableRow->Stamina);
	Stamina.ForceBroadcastDelegate();

	HealthRegenRate.SetValue(TableRow->HealthRegenRate);
	HealthRegenRate.ForceBroadcastDelegate();

	ManaRegenRate.SetValue(TableRow->ManaRegenRate);
	ManaRegenRate.ForceBroadcastDelegate();

	StaminaRegenRate.SetValue(TableRow->StaminaRegenRate);
	StaminaRegenRate.ForceBroadcastDelegate();

	PhysicalAttack.SetValue(TableRow->PhysicalAttack);
	PhysicalAttack.ForceBroadcastDelegate();

	PhysicalCritRate.SetValue(TableRow->PhysicalCritRate);
	PhysicalCritRate.ForceBroadcastDelegate();

	PhysicalResistance.SetValue(TableRow->PhysicalResistance);
	PhysicalResistance.ForceBroadcastDelegate();

	MagickalAttack.SetValue(TableRow->MagickalAttack);
	MagickalAttack.ForceBroadcastDelegate();

	MagickalCritRate.SetValue(TableRow->MagickalCritRate);
	MagickalCritRate.ForceBroadcastDelegate();

	MagickalResistance.SetValue(TableRow->MagickalResistance);
	MagickalResistance.ForceBroadcastDelegate();

	BleedResistance.SetValue(TableRow->BleedResistance);
	BleedResistance.ForceBroadcastDelegate();

}
