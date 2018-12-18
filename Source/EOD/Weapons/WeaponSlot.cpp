// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/Weapons/WeaponSlot.h"
#include "EOD/Weapons/WeaponBase.h"

#include "UnrealNetwork.h"

UWeaponSlot::UWeaponSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWeaponSlot::PostInitProperties()
{
	Super::PostInitProperties();

}

void UWeaponSlot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UWeaponSlot, PrimaryWeapon, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(UWeaponSlot, PrimaryWeaponID, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(UWeaponSlot, PrimaryWeaponDataAsset, COND_InitialOnly);
}

void UWeaponSlot::AttachPrimaryWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset)
{
}

void UWeaponSlot::AttachPrimaryWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset, AWeaponBase* PrimaryWep)
{
	PrimaryWeaponID = WeaponID;
	PrimaryWeaponDataAsset = WeaponDataAsset;
	PrimaryWeapon = PrimaryWep;

	PrimaryWeapon->OnEquip(WeaponID, WeaponDataAsset);
}

void UWeaponSlot::AttachPrimaryWeapon(AWeaponBase* PrimaryWep)
{
	PrimaryWeapon = PrimaryWep;
	PrimaryWeapon->OnEquip(PrimaryWeaponID, PrimaryWeaponDataAsset);
}

void UWeaponSlot::AttachSecondaryWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset)
{
}

void UWeaponSlot::DetachPrimaryWeapon()
{
}

void UWeaponSlot::DetachSecondaryWeapon()
{
}
