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
		
		switch (PlayerWeaponAnimationType)
		{
		case EWeaponAnimationType::NoWeapon:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("NoWeapon"), FString("Animation references for no weapon equipped"));
			break;
		case EWeaponAnimationType::SheathedWeapon:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("SheathedWeapon"), FString("Animation references when weapon is sheathed"));
			break;
		case EWeaponAnimationType::ShieldAndSword:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("ShieldAndSword"), FString("Animation references for Shield and Sword"));
			break;
		case EWeaponAnimationType::ShieldAndMace:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("ShieldAndMace"), FString("Animation references for Shield and Mace"));
			break;
		case EWeaponAnimationType::GreatSword:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("GreatSword"), FString("Animation references for GreatSword"));
			break;
		case EWeaponAnimationType::WarHammer:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("WarHammer"), FString("Animation references for WarHammer"));
			break;
		case EWeaponAnimationType::Staff:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Staff"), FString("Animation references for Staff"));
			break;
		case EWeaponAnimationType::Daggers:
			PlayerAnimationSoftReferences = GameSingleton->PlayerAnimationReferencesDataTable->FindRow<FPlayerAnimationReferencesTableRow>(FName("Daggers"), FString("Animation references for Daggers"));
			break;
		default:
			break;
		}

		//@ todo Modify following code to load assets asynchronously
		if (PlayerAnimationSoftReferences)
		{
			PlayerAnimationReferences = new FPlayerAnimationReferences;
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
