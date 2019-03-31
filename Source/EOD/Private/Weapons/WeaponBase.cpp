// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "WeaponBase.h"
#include "EODCharacterBase.h"
#include "WeaponStatsComponent.h"


// Sets default values
AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// This actor doesn't tick
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(false);

	// Setup stats component
	// StatsComp = ObjectInitializer.CreateDefaultSubobject<UWeaponStatsComponent>(this, FName("Weapon Stats Component"));

	WeaponType = EWeaponType::None;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	AttachParentCharacter = Cast<AEODCharacterBase>(GetOwner());
}

void AWeaponBase::OnEquip(FName NewWeaponID)
{
	FWeaponTableRow* WeaponData = UWeaponLibrary::GetWeaponData(NewWeaponID);
	if (WeaponData)
	{
		OnEquip(NewWeaponID, WeaponData);
	}
}
