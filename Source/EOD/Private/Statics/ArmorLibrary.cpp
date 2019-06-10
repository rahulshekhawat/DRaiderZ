// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "ArmorLibrary.h"
#include "GameSingleton.h"

#include "Engine/Engine.h"

FArmorTableRow* UArmorLibrary::GetArmorData(FName ArmorID)
{
	FArmorTableRow* ArmorData = nullptr;
	if (GEngine && GEngine->GameSingleton)
	{
		UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
		if (GameSingleton && GameSingleton->ArmorsDataTable)
		{
			ArmorData = GameSingleton->ArmorsDataTable->FindRow<FArmorTableRow>(ArmorID, FString("UArmorLibrary::GetWeaponData()"));
		}
	}
	return ArmorData;
}
