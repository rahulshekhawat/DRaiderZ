// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CharacterLibrary.h"
#include "Core/GameSingleton.h"
#include "Player/EODCharacterBase.h"
#include "Statics/EODLibrary.h"

#include "Engine/Engine.h"
#include "Engine/Texture.h"
#include "Engine/StreamableManager.h"
#include "Animation/AnimMontage.h"
#include "GenericPlatform/GenericPlatformProcess.h"

const FName UCharacterLibrary::SectionName_AttackBlocked 			= FName("AttackBlocked");
const FName UCharacterLibrary::SectionName_BackwardFlinch 			= FName("BackwardFlinch");
const FName UCharacterLibrary::SectionName_ForwardFlinch 			= FName("ForwardFlinch");
const FName UCharacterLibrary::SectionName_BackwardInterrupt		= FName("BackwardInterrupt");
const FName UCharacterLibrary::SectionName_ForwardInterrupt			= FName("ForwardInterrupt");
const FName UCharacterLibrary::SectionName_KnockdownEnd 			= FName("KnockedDownEnd");
const FName UCharacterLibrary::SectionName_KnockdownIdle 			= FName("KnockedDownIdle");
const FName UCharacterLibrary::SectionName_KnockdownStart 			= FName("KnockedDownStart");
const FName UCharacterLibrary::SectionName_LavitationEnd 			= FName("LavitationEnd");
const FName UCharacterLibrary::SectionName_LavitationStart 			= FName("LavitationStart");
const FName UCharacterLibrary::SectionName_StunEnd 					= FName("StunEnd");
const FName UCharacterLibrary::SectionName_StunLoop 				= FName("StunLoop");
const FName UCharacterLibrary::SectionName_StunStart 				= FName("StunStart");
const FName UCharacterLibrary::SectionName_JumpEnd					= FName("JumpEnd");
const FName UCharacterLibrary::SectionName_JumpLoop					= FName("JumpLoop");
const FName UCharacterLibrary::SectionName_JumpStart				= FName("JumpStart");

const FName UCharacterLibrary::SectionName_FirstSwing 				= FName("FirstSwing");
const FName UCharacterLibrary::SectionName_FirstSwingEnd 			= FName("FirstSwingEnd");
const FName UCharacterLibrary::SectionName_SecondSwing 				= FName("SecondSwing");
const FName UCharacterLibrary::SectionName_SecondSwingEnd 			= FName("SecondSwingEnd");
const FName UCharacterLibrary::SectionName_ThirdSwing 				= FName("ThirdSwing");
const FName UCharacterLibrary::SectionName_ThirdSwingEnd 			= FName("ThirdSwingEnd");
const FName UCharacterLibrary::SectionName_FourthSwing 				= FName("FourthSwing");
const FName UCharacterLibrary::SectionName_FourthSwingEnd 			= FName("FourthSwingEnd");
const FName UCharacterLibrary::SectionName_FifthSwing 				= FName("FifthSwing");
const FName UCharacterLibrary::SectionName_FifthSwingEnd 			= FName("FifthSwingEnd");
const FName UCharacterLibrary::SectionName_ForwardSPSwing 			= FName("ForwardSPSwing");
const FName UCharacterLibrary::SectionName_ForwardSPSwingEnd 		= FName("ForwardSPSwingEnd");
const FName UCharacterLibrary::SectionName_BackwardSPSwing 			= FName("BackwardSPSwing");
const FName UCharacterLibrary::SectionName_BackwardSPSwingEnd 		= FName("BackwardSPSwingEnd");

const FName UCharacterLibrary::SectionName_ForwardDodge 			= FName("ForwardDodge");
const FName UCharacterLibrary::SectionName_BackwardDodge 			= FName("BackwardDodge");
const FName UCharacterLibrary::SectionName_LeftDodge 				= FName("LeftDodge");
const FName UCharacterLibrary::SectionName_RightDodge 				= FName("RightDodge");

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

			if (PlayerAnimationSoftReferences->Dodge.IsNull())
			{
				PlayerAnimationReferences->AnimationMontage_Dodge = nullptr;
			}
			else if (PlayerAnimationSoftReferences->Dodge.IsPending())
			{
				PlayerAnimationReferences->AnimationMontage_Dodge = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Dodge);
			}
			else if (PlayerAnimationSoftReferences->Dodge.IsValid())
			{
				PlayerAnimationReferences->AnimationMontage_Dodge = PlayerAnimationSoftReferences->Dodge.Get();
			}

			if (PlayerAnimationSoftReferences->Jump.IsNull())
			{
				PlayerAnimationReferences->AnimationMontage_Jump = nullptr;
			}
			else if (PlayerAnimationSoftReferences->Jump.IsPending())
			{
				PlayerAnimationReferences->AnimationMontage_Jump = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Jump);
			}
			else if (PlayerAnimationSoftReferences->Jump.IsValid())
			{
				PlayerAnimationReferences->AnimationMontage_Jump = PlayerAnimationSoftReferences->Jump.Get();
			}
			
			if (PlayerAnimationSoftReferences->NormalAttacks.IsNull())
			{
				PlayerAnimationReferences->AnimationMontage_NormalAttacks = nullptr;
			}
			else if (PlayerAnimationSoftReferences->NormalAttacks.IsPending())
			{
				PlayerAnimationReferences->AnimationMontage_NormalAttacks = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->NormalAttacks);
			}
			else if (PlayerAnimationSoftReferences->NormalAttacks.IsValid())
			{
				PlayerAnimationReferences->AnimationMontage_NormalAttacks = PlayerAnimationSoftReferences->NormalAttacks.Get();
			}
			
			if (PlayerAnimationSoftReferences->SpecialActions.IsNull())
			{
				PlayerAnimationReferences->AnimationMontage_SpecialActions = nullptr;
			}
			else if (PlayerAnimationSoftReferences->SpecialActions.IsPending())
			{
				PlayerAnimationReferences->AnimationMontage_SpecialActions = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->SpecialActions);
			}
			else if (PlayerAnimationSoftReferences->SpecialActions.IsValid())
			{
				PlayerAnimationReferences->AnimationMontage_SpecialActions = PlayerAnimationSoftReferences->SpecialActions.Get();
			}
			
			if (PlayerAnimationSoftReferences->Skills.IsNull())
			{
				PlayerAnimationReferences->AnimationMontage_Skills = nullptr;
			}
			else if (PlayerAnimationSoftReferences->Skills.IsPending())
			{
				PlayerAnimationReferences->AnimationMontage_Skills = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Skills);
			}
			else if (PlayerAnimationSoftReferences->Skills.IsValid())
			{
				PlayerAnimationReferences->AnimationMontage_Skills = PlayerAnimationSoftReferences->Skills.Get();
			}

			if (PlayerAnimationSoftReferences->Spells.IsNull())
			{
				PlayerAnimationReferences->AnimationMontage_Spells = nullptr;
			}
			else if (PlayerAnimationSoftReferences->Spells.IsPending())
			{
				PlayerAnimationReferences->AnimationMontage_Spells = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Spells);
			}
			else if (PlayerAnimationSoftReferences->Spells.IsValid())
			{
				PlayerAnimationReferences->AnimationMontage_Spells = PlayerAnimationSoftReferences->Spells.Get();
			}
			
			if (PlayerAnimationSoftReferences->SpecialMovement.IsNull())
			{
				PlayerAnimationReferences->AnimationMontage_SpecialMovement = nullptr;
			}
			else if (PlayerAnimationSoftReferences->SpecialMovement.IsPending())
			{
				PlayerAnimationReferences->AnimationMontage_SpecialMovement = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->SpecialMovement);
			}
			else if (PlayerAnimationSoftReferences->SpecialMovement.IsValid())
			{
				PlayerAnimationReferences->AnimationMontage_SpecialMovement = PlayerAnimationSoftReferences->SpecialMovement.Get();
			}
			
			if (PlayerAnimationSoftReferences->HitEffects.IsNull())
			{
				PlayerAnimationReferences->AnimationMontage_HitEffects = nullptr;
			}
			else if (PlayerAnimationSoftReferences->HitEffects.IsPending())
			{
				PlayerAnimationReferences->AnimationMontage_HitEffects = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->HitEffects);
			}
			else if (PlayerAnimationSoftReferences->HitEffects.IsValid())
			{
				PlayerAnimationReferences->AnimationMontage_HitEffects = PlayerAnimationSoftReferences->HitEffects.Get();
			}
			
			if (PlayerAnimationSoftReferences->Flinch.IsNull())
			{
				PlayerAnimationReferences->AnimationMontage_Flinch = nullptr;
			}
			else if (PlayerAnimationSoftReferences->Flinch.IsPending())
			{
				PlayerAnimationReferences->AnimationMontage_Flinch = GameSingleton->StreamableManager.LoadSynchronous<UAnimMontage>(PlayerAnimationSoftReferences->Flinch);
			}
			else if (PlayerAnimationSoftReferences->Flinch.IsValid())
			{
				PlayerAnimationReferences->AnimationMontage_Flinch = PlayerAnimationSoftReferences->Flinch.Get();
			}

			/*
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
			*/
		}
	}

	return PlayerAnimationReferences;
}

FSkill * UCharacterLibrary::GetPlayerSkill(FName SKillID, uint8 SkillLevel)
{
	FSkill* Skill = nullptr;

	if (GEngine && GEngine->GameSingleton)
	{
		UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
		if (GameSingleton && GameSingleton->PlayerSkillsTable)
		{
			FSkillTableRow* SkillTableRow = GameSingleton->PlayerSkillsTable->FindRow<FSkillTableRow>(SKillID, FString("Looking up player skill"));

			if (SkillTableRow)
			{
				Skill = new FSkill(SkillTableRow, SkillLevel);
			}
		}
	}

	return Skill;
}

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
	if (CharacterOne->GetFaction() == CharacterTwo->GetFaction())
	{
		return false;
	}

	return true;
}

void UCharacterLibrary::GetAllAICharacterSkills(const FString & CharacterName, const UDataTable * SkillsDataTable, TArray<FSkill*>& OutSkills)
{
	if (!SkillsDataTable)
	{
		return;
	}

	FString ContextString = FString("Looking up skills for ") + CharacterName;
	TArray<FSkillTableRow*> SkillTableRows;
	SkillsDataTable->GetAllRows<FSkillTableRow>(ContextString, SkillTableRows);

	for (FSkillTableRow* SkillTableRow : SkillTableRows)
	{
		FSkill* Skill = new FSkill(SkillTableRow);
		OutSkills.Add(Skill);
	}

	return;
}

FSkill::FSkill()
{
	this->CurrentSkillLevel = 0;
	this->AnimationMontage_GenderOne = nullptr;
	this->AnimationMontage_GenderTwo = nullptr;
}

FSkill::FSkill(FSkillTableRow * SkillTableRow, uint8 SkillLevel)
{
	this->InGameName 						= SkillTableRow->InGameName;
	this->Description 						= SkillTableRow->Description;
	this->SupportedWeapons 					= SkillTableRow->SupportedWeapons;
	this->SkillStartMontageSectionName 		= SkillTableRow->SkillStartMontageSectionName;
	this->SkillLoopMontageSectionName 		= SkillTableRow->SkillLoopMontageSectionName;
	this->SkillEndMontageSectionName 		= SkillTableRow->SkillEndMontageSectionName;
	this->DamageType 						= SkillTableRow->DamageType;
	this->PrecedingSkillID					= SkillTableRow->PrecedingSkillID;
	this->SupersedingSkillID				= SkillTableRow->SupersedingSkillID;
	this->RequiredStatusID					= SkillTableRow->RequiredStatusID;

	if (SkillTableRow->SkillLevelUpsInfo.Num() >= SkillLevel)
	{
		this->CurrentSkillLevel = SkillLevel;
		this->SkillLevelUpInfo = SkillTableRow->SkillLevelUpsInfo[SkillLevel - 1];
	}
	else if (SkillTableRow->SkillLevelUpsInfo.Num() > 0)
	{
		this->CurrentSkillLevel = 1;
		this->SkillLevelUpInfo = SkillTableRow->SkillLevelUpsInfo[0];
	}
	else
	{
		this->CurrentSkillLevel = 0;
	}

	this->Icon = EODLoadAsset<UTexture>(SkillTableRow->Icon);

	this->AnimationMontage_GenderOne = EODLoadAsset<UAnimMontage>(SkillTableRow->AnimMontage_GenderOne);
	this->AnimationMontage_GenderTwo = EODLoadAsset<UAnimMontage>(SkillTableRow->AnimMontage_GenderTwo);

}
