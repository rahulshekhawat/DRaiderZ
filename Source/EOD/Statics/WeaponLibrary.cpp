// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "WeaponLibrary.h"
#include "Core/GameSingleton.h"

bool UWeaponLibrary::IsWeaponDualHanded(EWeaponType WeaponType)
{
	if ((WeaponType == EWeaponType::GreatSword) ||
		(WeaponType == EWeaponType::WarHammer) ||
		(WeaponType == EWeaponType::Dagger) ||
		(WeaponType == EWeaponType::Staff))
		return true;

	return false;
}

bool UWeaponLibrary::IsWeaponSingleHanded(EWeaponType WeaponType)
{
	if ((WeaponType == EWeaponType::LongSword) ||
		(WeaponType == EWeaponType::Mace))
		return true;

	return false;
}

FWeaponData * UWeaponLibrary::GetWeaponData(FName WeaponID)
{
	FWeaponData* WeaponData = nullptr;

	if (GEngine && GEngine->GameSingleton)
	{
		UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
		if (GameSingleton && GameSingleton->WeaponDataTable)
		{
			WeaponData = GameSingleton->WeaponDataTable->FindRow<FWeaponData>(WeaponID, FString("Weapon data lookup"));
		}
	}

	return WeaponData;
}
