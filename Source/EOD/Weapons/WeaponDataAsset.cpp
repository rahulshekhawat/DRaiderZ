// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "WeaponDataAsset.h"

UWeaponDataAsset::UWeaponDataAsset(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    OnBlock_PhysicalDamageReduction = 50.f;
    OnBlock_MagickalDamageReduction = 50.f;
    MaxEnchantLevel = 5;
}
