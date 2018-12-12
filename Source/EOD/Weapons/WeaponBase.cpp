// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "WeaponBase.h"
#include "EOD/Weapons/Components/WeaponStatsComponent.h"


// Sets default values
AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// This actor doesn't tick
	PrimaryActorTick.bCanEverTick = false;
	
	bEquipped = false;
	WeaponID = NAME_None;
	WeaponType = EWeaponType::None;

	// Setup stats component
	StatsComp = ObjectInitializer.CreateDefaultSubobject<UWeaponStatsComponent>(this, FName("Weapon Stats Component"));
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::OnEquip(FName NewWeaponID)
{
	FWeaponTableRow* WeaponData = UWeaponLibrary::GetWeaponData(NewWeaponID);
	if (WeaponData)
	{
		OnEquip(NewWeaponID, WeaponData);
	}
}
