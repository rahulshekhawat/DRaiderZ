// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CharacterLibrary.h"
#include "Core/GameSingleton.h"

UCharacterLibrary::UCharacterLibrary(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

FPlayerAnimationReferences * UCharacterLibrary::GetPlayerAnimationReferences(EWeaponAnimationType PlayerWeaponAnimationType)
{
	FPlayerAnimationReferences* PlayerAnimationReferences = nullptr;

	if (GEngine && GEngine->GameSingleton)
	{
		UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);

		switch (PlayerWeaponAnimationType)
		{
		case EWeaponAnimationType::NoWeapon:
			PlayerAnimationReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferences>(FName("NoWeapon"), FString("Animation references for no weapon equipped"));
			break;
		case EWeaponAnimationType::SheathedWeapon:
			PlayerAnimationReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferences>(FName("SheathedWeapon"), FString("Animation references when weapon is sheathed"));
			break;
		case EWeaponAnimationType::ShieldAndSword:
			PlayerAnimationReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferences>(FName("ShieldAndSword"), FString("Animation references for Shield and Sword"));
			break;
		case EWeaponAnimationType::ShieldAndMace:
			PlayerAnimationReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferences>(FName("ShieldAndMace"), FString("Animation references for Shield and Mace"));
			break;
		case EWeaponAnimationType::GreatSword:
			PlayerAnimationReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferences>(FName("GreatSword"), FString("Animation references for GreatSword"));
			break;
		case EWeaponAnimationType::WarHammer:
			PlayerAnimationReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferences>(FName("WarHammer"), FString("Animation references for WarHammer"));
			break;
		case EWeaponAnimationType::Staff:
			PlayerAnimationReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferences>(FName("Staff"), FString("Animation references for Staff"));
			break;
		case EWeaponAnimationType::Daggers:
			PlayerAnimationReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferences>(FName("Daggers"), FString("Animation references for Daggers"));
			break;
		default:
			break;
		}
	}

	return PlayerAnimationReferences;
}
