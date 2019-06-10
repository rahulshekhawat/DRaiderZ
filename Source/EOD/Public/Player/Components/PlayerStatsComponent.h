// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArmorLibrary.h"
#include "WeaponLibrary.h"
#include "StatsComponentBase.h"

#include "PlayerStatsComponent.generated.h"

class APlayerCharacter;
class UStatusIndicatorWidget;

/**
 * PlayerStatsComponent is used to manage stats of a player controlled character
 */
UCLASS()
class EOD_API UPlayerStatsComponent : public UStatsComponentBase
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UPlayerStatsComponent(const FObjectInitializer& ObjectInitializer);

	/** Initialize current health, mana, and stamina */
	virtual void PostInitProperties() override;

	virtual void BeginPlay() override;

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// --------------------------------------
	//  Stats Management
	// --------------------------------------

	void AddPrimaryWeaponStats(FWeaponTableRow* WeaponData);
	void AddSecondaryWeaponStats(FWeaponTableRow* WeaponData);
	void RemovePrimaryWeaponStats();
	void RemoveSecondaryWeaponStats();

	void AddArmorStats(FArmorTableRow* ArmorData);
	void RemoveArmorStats(EArmorType ArmorType);

private:

	FWeaponTableRow* PrimaryWeaponData;
	FWeaponTableRow* SecondaryWeaponData;

	TMap<EArmorType, FArmorTableRow*> ArmorsData;
	

};
