// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "ElementalBase.h"
#include "Weapons/WeaponBase.h"

UElementalBase::UElementalBase(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UElementalBase::Initialize(AEODCharacterBase * Owner, AActor * Instigator)
{
	// Make sure instigator is a weapon
	AWeaponBase* WeaponBase = Cast<AWeaponBase>(Instigator);
	check(WeaponBase);

	Super::Initialize(Owner, Instigator);
}
