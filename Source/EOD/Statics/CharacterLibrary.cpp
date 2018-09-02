// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CharacterLibrary.h"
#include "Core/GameSingleton.h"

#include "Engine/Engine.h"
#include "Engine/Texture.h"
#include "Engine/StreamableManager.h"
#include "Animation/AnimMontage.h"
#include "GenericPlatform/GenericPlatformProcess.h"

UCharacterLibrary::UCharacterLibrary(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

FPlayerAnimationReferences * UCharacterLibrary::GetPlayerAnimationReferences(EWeaponAnimationType PlayerWeaponAnimationType, ECharacterGender Gender)
{
	FPlayerAnimationReferences* PlayerAnimationReferences = nullptr;

	if (GEngine && GEngine->GameSingleton)
	{
		UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
		FPlayerAnimationReferencesTableRow* PlayerAnimationSoftReferences = nullptr;
		FName PlayerAnimationReferencesTableRowID = NAME_None;
		
		if (Gender == ECharacterGender::Female)
		{
			switch (PlayerWeaponAnimationType)
			{
			case EWeaponAnimationType::NoWeapon:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Female_NoWeapon"), FString("Animation references for no weapon equipped"));
				PlayerAnimationReferencesTableRowID = FName("Female_NoWeapon");
				break;
			case EWeaponAnimationType::SheathedWeapon:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Female_SheathedWeapon"), FString("Animation references when weapon is sheathed"));
				PlayerAnimationReferencesTableRowID = FName("Female_SheathedWeapon");
				break;
			case EWeaponAnimationType::ShieldAndSword:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Female_ShieldAndSword"), FString("Animation references for Shield and Sword"));
				PlayerAnimationReferencesTableRowID = FName("Female_ShieldAndSword");
				break;
			case EWeaponAnimationType::ShieldAndMace:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Female_ShieldAndMace"), FString("Animation references for Shield and Mace"));
				PlayerAnimationReferencesTableRowID = FName("Female_ShieldAndMace");
				break;
			case EWeaponAnimationType::GreatSword:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Female_GreatSword"), FString("Animation references for GreatSword"));
				PlayerAnimationReferencesTableRowID = FName("Female_GreatSword");
				break;
			case EWeaponAnimationType::WarHammer:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Female_WarHammer"), FString("Animation references for WarHammer"));
				PlayerAnimationReferencesTableRowID = FName("Female_WarHammer");
				break;
			case EWeaponAnimationType::Staff:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Female_Staff"), FString("Animation references for Staff"));
				PlayerAnimationReferencesTableRowID = FName("Female_Staff");
				break;
			case EWeaponAnimationType::Daggers:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Female_Daggers"), FString("Animation references for Daggers"));
				PlayerAnimationReferencesTableRowID = FName("Female_Daggers");
				break;
			default:
				break;
			}
		}
		else if (Gender == ECharacterGender::Male)
		{
			switch (PlayerWeaponAnimationType)
			{
			case EWeaponAnimationType::NoWeapon:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Male_NoWeapon"), FString("Animation references for no weapon equipped"));
				PlayerAnimationReferencesTableRowID = FName("Male_NoWeapon");
				break;
			case EWeaponAnimationType::SheathedWeapon:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Male_SheathedWeapon"), FString("Animation references when weapon is sheathed"));
				PlayerAnimationReferencesTableRowID = FName("Male_SheathedWeapon");
				break;
			case EWeaponAnimationType::ShieldAndSword:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Male_ShieldAndSword"), FString("Animation references for Shield and Sword"));
				PlayerAnimationReferencesTableRowID = FName("Male_ShieldAndSword");
				break;
			case EWeaponAnimationType::ShieldAndMace:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Male_ShieldAndMace"), FString("Animation references for Shield and Mace"));
				PlayerAnimationReferencesTableRowID = FName("Male_ShieldAndMace");
				break;
			case EWeaponAnimationType::GreatSword:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Male_GreatSword"), FString("Animation references for GreatSword"));
				PlayerAnimationReferencesTableRowID = FName("Male_GreatSword");
				break;
			case EWeaponAnimationType::WarHammer:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Male_WarHammer"), FString("Animation references for WarHammer"));
				PlayerAnimationReferencesTableRowID = FName("Male_WarHammer");
				break;
			case EWeaponAnimationType::Staff:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Male_Staff"), FString("Animation references for Staff"));
				PlayerAnimationReferencesTableRowID = FName("Male_Staff");
				break;
			case EWeaponAnimationType::Daggers:
				PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Male_Daggers"), FString("Animation references for Daggers"));
				PlayerAnimationReferencesTableRowID = FName("Male_Daggers");
				break;
			default:
				break;
			}
		}


		// @todo Modify following code to load assets asynchronously
		if (PlayerAnimationSoftReferences)
		{
			PlayerAnimationReferences = new FPlayerAnimationReferences;
			PlayerAnimationReferences->FPlayerAnimationReferencesTableRowID = PlayerAnimationReferencesTableRowID;
			PlayerAnimationReferences->AnimationMontage_Dodge				= GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Dodge);
			PlayerAnimationReferences->AnimationMontage_Jump				= GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Jump);
			PlayerAnimationReferences->AnimationMontage_NormalAttacks		= GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->NormalAttacks);
			PlayerAnimationReferences->AnimationMontage_SpecialActions		= GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->SpecialActions);
			PlayerAnimationReferences->AnimationMontage_Skills				= GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Skills);
			PlayerAnimationReferences->AnimationMontage_Spells				= GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Spells);
			PlayerAnimationReferences->AnimationMontage_SpecialMovement		= GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->SpecialMovement);
			PlayerAnimationReferences->AnimationMontage_HitEffects = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->HitEffects);
			PlayerAnimationReferences->AnimationMontage_Flinch				= GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Flinch);

		}
	}

	return PlayerAnimationReferences;
}

/*
FPlayerAnimationReferences * UCharacterLibrary::GetPlayerAnimationReferences(const FName AnimationID)
{
	return nullptr;
}
*/

bool UCharacterLibrary::UnloadPlayerAnimationReferences(FPlayerAnimationReferences * PlayerAnimationReferences, ECharacterGender Gender)
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

	PlayerAnimationReferences->AnimationMontage_Jump					= nullptr;
	PlayerAnimationReferences->AnimationMontage_Dodge					= nullptr;
	PlayerAnimationReferences->AnimationMontage_NormalAttacks			= nullptr;
	PlayerAnimationReferences->AnimationMontage_SpecialActions			= nullptr;
	PlayerAnimationReferences->AnimationMontage_Skills					= nullptr;
	PlayerAnimationReferences->AnimationMontage_Spells					= nullptr;
	PlayerAnimationReferences->AnimationMontage_SpecialMovement			= nullptr;
	PlayerAnimationReferences->AnimationMontage_HitEffects				= nullptr;
	PlayerAnimationReferences->AnimationMontage_Flinch					= nullptr;	

	GameSingleton->StreamableManager.Unload(PlayerAnimationSoftReferences->HitEffects.ToSoftObjectPath());
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

bool UCharacterLibrary::AreEnemies(AEODCharacterBase * CharacterOne, AEODCharacterBase * CharacterTwo)
{
	// return false;
	return true;
}

FSkill::FSkill()
{
	this->CurrentSkillLevel = 0;
}

FSkill::FSkill(FSkillTableRow * SkillTableRow)
{
	this->CurrentSkillLevel 				= 1;	// Always one when initializing from SkillTableRow pointer
	this->InGameName 						= SkillTableRow->InGameName;
	this->Description 						= SkillTableRow->Description;
	this->SupportedWeapons 					= SkillTableRow->SupportedWeapons;
	this->SkillStartMontageSectionName 		= SkillTableRow->SkillStartMontageSectionName;
	this->SkillLoopMontageSectionName 		= SkillTableRow->SkillLoopMontageSectionName;
	this->SkillEndMontageSectionName 		= SkillTableRow->SkillEndMontageSectionName;
	this->DamageType 						= SkillTableRow->DamageType;

	if (SkillTableRow->SkillLevelUpsInfo.Num() > 1)
	{
		this->SkillLevelUpInfo = SkillTableRow->SkillLevelUpsInfo[1];
	}

	if (SkillTableRow->Icon.IsNull())
	{
		this->Icon = nullptr;
	}
	else if (SkillTableRow->Icon.IsPending())
	{
		this->Icon = SkillTableRow->Icon.LoadSynchronous();
	}
	else if (SkillTableRow->Icon.IsValid())
	{
		this->Icon = SkillTableRow->Icon.Get();
	}
}
