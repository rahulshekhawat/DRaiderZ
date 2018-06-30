// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SecondaryWeapon.h"

ASecondaryWeapon::ASecondaryWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ASecondaryWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASecondaryWeapon::OnEquip(FWeaponData * NewWeaponData)
{
}

void ASecondaryWeapon::OnUnEquip()
{
}
