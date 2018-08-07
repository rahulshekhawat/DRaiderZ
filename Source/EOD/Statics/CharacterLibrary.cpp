// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CharacterLibrary.h"
#include "Core/GameSingleton.h"

#include "Engine/Engine.h"
#include "Engine/StreamableManager.h"
#include "Animation/AnimMontage.h"
#include "GenericPlatform/GenericPlatformProcess.h"

UCharacterLibrary::UCharacterLibrary(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

FPlayerAnimationReferences * UCharacterLibrary::GetPlayerAnimationReferences(EWeaponAnimationType PlayerWeaponAnimationType)
{
	FPlayerAnimationReferences* PlayerAnimationReferences = nullptr;

	if (GEngine && GEngine->GameSingleton)
	{
		UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
		FPlayerAnimationReferencesTableRow* PlayerAnimationSoftReferences = nullptr;
		FName PlayerAnimationReferencesTableRowID = NAME_None;
		
		switch (PlayerWeaponAnimationType)
		{
		case EWeaponAnimationType::NoWeapon:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("NoWeapon"), FString("Animation references for no weapon equipped"));
			PlayerAnimationReferencesTableRowID = FName("NoWeapon");
			break;
		case EWeaponAnimationType::SheathedWeapon:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("SheathedWeapon"), FString("Animation references when weapon is sheathed"));
			PlayerAnimationReferencesTableRowID = FName("SheathedWeapon");
			break;
		case EWeaponAnimationType::ShieldAndSword:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("ShieldAndSword"), FString("Animation references for Shield and Sword"));
			PlayerAnimationReferencesTableRowID = FName("ShieldAndSword");
			break;
		case EWeaponAnimationType::ShieldAndMace:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("ShieldAndMace"), FString("Animation references for Shield and Mace"));
			PlayerAnimationReferencesTableRowID = FName("ShieldAndMace");
			break;
		case EWeaponAnimationType::GreatSword:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("GreatSword"), FString("Animation references for GreatSword"));
			PlayerAnimationReferencesTableRowID = FName("GreatSword");
			break;
		case EWeaponAnimationType::WarHammer:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("WarHammer"), FString("Animation references for WarHammer"));
			PlayerAnimationReferencesTableRowID = FName("WarHammer");
			break;
		case EWeaponAnimationType::Staff:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Staff"), FString("Animation references for Staff"));
			PlayerAnimationReferencesTableRowID = FName("Staff");
			break;
		case EWeaponAnimationType::Daggers:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Daggers"), FString("Animation references for Daggers"));
			PlayerAnimationReferencesTableRowID = FName("Daggers");
			break;
		default:
			break;
		}

		//@ todo Modify following code to load assets asynchronously
		if (PlayerAnimationSoftReferences)
		{
			PlayerAnimationReferences = new FPlayerAnimationReferences;
			PlayerAnimationReferences->FPlayerAnimationReferencesTableRowID = PlayerAnimationReferencesTableRowID;
			PlayerAnimationReferences->AnimationMontage_Dodge = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Dodge);
			PlayerAnimationReferences->AnimationMontage_Jump = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Jump);
			PlayerAnimationReferences->AnimationMontage_NormalAttacks = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->NormalAttacks);
			PlayerAnimationReferences->AnimationMontage_SpecialActions = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->SpecialActions);
			PlayerAnimationReferences->AnimationMontage_Skills = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Skills);
			PlayerAnimationReferences->AnimationMontage_Spells = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Spells);
			PlayerAnimationReferences->AnimationMontage_SpecialMovement = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->SpecialMovement);
			PlayerAnimationReferences->AnimationMontage_CrowdControlEffects = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->CrowdControlEffects);
			PlayerAnimationReferences->AnimationMontage_Flinch = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Flinch);

		}
	}

	return PlayerAnimationReferences;
}

bool UCharacterLibrary::UnloadPlayerAnimationReferences(FPlayerAnimationReferences * PlayerAnimationReferences)
{
	if (!(PlayerAnimationReferences && GEngine && GEngine->GameSingleton))
	{
		return false;
	}

	UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
	if (!GameSingleton)
	{
		return false;
	}

	FPlayerAnimationReferencesTableRow* PlayerAnimationSoftReferences = nullptr;
	PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(PlayerAnimationReferences->FPlayerAnimationReferencesTableRowID, FString("Animation references lookup for unloading"));

	if (!PlayerAnimationSoftReferences)
	{
		return false;
	}

	GameSingleton->StreamableManager.Unload(PlayerAnimationSoftReferences->CrowdControlEffects.ToSoftObjectPath());
	GameSingleton->StreamableManager.Unload(PlayerAnimationSoftReferences->Dodge.ToSoftObjectPath());
	GameSingleton->StreamableManager.Unload(PlayerAnimationSoftReferences->Flinch.ToSoftObjectPath());
	GameSingleton->StreamableManager.Unload(PlayerAnimationSoftReferences->Jump.ToSoftObjectPath());
	GameSingleton->StreamableManager.Unload(PlayerAnimationSoftReferences->NormalAttacks.ToSoftObjectPath());
	GameSingleton->StreamableManager.Unload(PlayerAnimationSoftReferences->Skills.ToSoftObjectPath());
	GameSingleton->StreamableManager.Unload(PlayerAnimationSoftReferences->SpecialActions.ToSoftObjectPath());
	GameSingleton->StreamableManager.Unload(PlayerAnimationSoftReferences->SpecialMovement.ToSoftObjectPath());
	GameSingleton->StreamableManager.Unload(PlayerAnimationSoftReferences->Spells.ToSoftObjectPath());

	return true;
}

FPlayerAnimationReferences::~FPlayerAnimationReferences()
{
	AnimationMontage_Jump = nullptr;
	AnimationMontage_Dodge = nullptr;
	AnimationMontage_NormalAttacks = nullptr;
	AnimationMontage_SpecialActions = nullptr;
	AnimationMontage_Skills = nullptr;
	AnimationMontage_Spells = nullptr;
	AnimationMontage_SpecialMovement = nullptr;
	AnimationMontage_CrowdControlEffects = nullptr;
	AnimationMontage_Flinch = nullptr;

}
