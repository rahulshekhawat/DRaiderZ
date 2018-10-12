// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "WeaponLibrary.h"
#include "Core/GameSingleton.h"

#include "Engine/Engine.h"

bool UWeaponLibrary::IsWeaponDualHanded(EWeaponType WeaponType)
{
	if ((WeaponType == EWeaponType::GreatSword) ||
		(WeaponType == EWeaponType::WarHammer) ||
		(WeaponType == EWeaponType::Staff))
		return true;

	return false;
}

bool UWeaponLibrary::IsWeaponSingleHanded(EWeaponType WeaponType)
{
	if ((WeaponType == EWeaponType::LongSword) ||
		(WeaponType == EWeaponType::Mace) ||
		(WeaponType == EWeaponType::Dagger) ||
		(WeaponType == EWeaponType::Shield))
		return true;

	return false;
}

bool UWeaponLibrary::IsPrimaryWeapon(EWeaponType WeaponType)
{
	if ((WeaponType == EWeaponType::GreatSword) ||
		(WeaponType == EWeaponType::WarHammer) ||
		(WeaponType == EWeaponType::Staff) ||
		(WeaponType == EWeaponType::LongSword) ||
		(WeaponType == EWeaponType::Mace))
		return true;

	return false;
}

bool UWeaponLibrary::IsSecondaryWeapon(EWeaponType WeaponType)
{
	if (WeaponType == EWeaponType::Shield)
		return true;

	return false;
}

bool UWeaponLibrary::IsHybridWeapon(EWeaponType WeaponType)
{
	if (WeaponType == EWeaponType::Dagger)
		return true;

	return false;
}

FWeaponTableRow * UWeaponLibrary::GetWeaponData(FName WeaponID)
{
	FWeaponTableRow* WeaponData = nullptr;

	if (GEngine && GEngine->GameSingleton)
	{
		UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
		if (GameSingleton && GameSingleton->WeaponsDataTable)
		{
			WeaponData = GameSingleton->WeaponsDataTable->FindRow<FWeaponTableRow>(WeaponID, FString("Weapon data lookup"));
		}
	}

	return WeaponData;
}
