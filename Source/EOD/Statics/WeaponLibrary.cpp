// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "WeaponLibrary.h"
#include "EOD/Core/GameSingleton.h"
#include "EOD/Weapons/WeaponDataAsset.h"

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

FWeaponTableRow* UWeaponLibrary::GetWeaponData(FName WeaponID)
{
	FWeaponTableRow* WeaponData = nullptr;
	if (IsValid(GEngine) && IsValid(GEngine->GameSingleton))
	{
		UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
		if (IsValid(GameSingleton) && IsValid(GameSingleton->WeaponsDataTable))
		{
			WeaponData = GameSingleton->WeaponsDataTable->FindRow<FWeaponTableRow>(WeaponID, FString("UWeaponLibrary::GetWeaponData()"));
		}
	}
	return WeaponData;
}

UWeaponDataAsset* UWeaponLibrary::GetWeaponDataAsset(FName WeaponID)
{
	UWeaponDataAsset* WeaponDataAsset = nullptr;

	if (GEngine && GEngine->GameSingleton)
	{
		UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
		if (GameSingleton && GameSingleton->WeaponsDataAssetsDataTable)
		{
			FWeaponDataAssetTableRow* WeaponDataAssetRow = GameSingleton->WeaponsDataAssetsDataTable->FindRow<FWeaponDataAssetTableRow>(WeaponID, FString("UWeaponLibrary::GetWeaponDataAsset(), weapon data asset lookup"));
			if (WeaponDataAssetRow && !WeaponDataAssetRow->WeaponDataAsset.IsNull())
			{
				WeaponDataAsset = WeaponDataAssetRow->WeaponDataAsset.LoadSynchronous();
			}
		}
	}

	return WeaponDataAsset;
}
