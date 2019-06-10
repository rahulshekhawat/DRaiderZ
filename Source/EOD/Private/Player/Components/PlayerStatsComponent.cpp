// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PlayerStatsComponent.h"
#include "StatusIndicatorWidget.h"
#include "PlayerCharacter.h"

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
