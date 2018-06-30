// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PrimaryWeapon.h"

APrimaryWeapon::APrimaryWeapon(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void APrimaryWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APrimaryWeapon::OnEquip(FWeaponData * NewWeaponData)
{
}

void APrimaryWeapon::OnUnEquip()
{
}
