// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CharacterLibrary.h"
#include "EOD/Core/GameSingleton.h"
#include "EOD/Characters/EODCharacterBase.h"
#include "EOD/Statics/EODLibrary.h"

#include "Engine/Engine.h"
#include "Engine/Texture.h"
#include "Engine/StreamableManager.h"
#include "Animation/AnimMontage.h"
#include "GenericPlatform/GenericPlatformProcess.h"

const FName UCharacterLibrary::SectionName_BlockAttack 				= FName("BlockAttack");
const FName UCharacterLibrary::SectionName_AttackDeflected 			= FName("AttackDeflected");
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

const FName UCharacterLibrary::SectionName_Default 					= FName("Default");

const FName UCharacterLibrary::SectionName_SheatheWeapon 			= FName("SheatheWeapon");
const FName UCharacterLibrary::SectionName_UnsheatheWeapon 			= FName("UnsheatheWeapon");

UCharacterLibrary::UCharacterLibrary(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

FSkillTableRow* UCharacterLibrary::GetPlayerSkill(const FName SkillID, const FString& ContextString)
{
	FSkillTableRow* Skill = nullptr;

	if (GEngine)
	{
		UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);

		if (GameSingleton && GameSingleton->PlayerSkillsDataTable)
		{
			Skill = GameSingleton->PlayerSkillsDataTable->FindRow<FSkillTableRow>(SkillID, ContextString);
		}
	}

	return Skill;
}

FSkillTableRow UCharacterLibrary::BP_GetPlayerSkill(const FName SkillID, const FString& ContextString, bool& bOutSkillFound)
{
	FSkillTableRow* Skill = GetPlayerSkill(SkillID, ContextString);
	if (Skill)
	{
		bOutSkillFound = true;
	}
	else
	{
		bOutSkillFound = false;
	}

	return *Skill;
}

bool UCharacterLibrary::AreEnemies(AEODCharacterBase * CharacterOne, AEODCharacterBase * CharacterTwo)
{
	if (CharacterOne->GetFaction() == CharacterTwo->GetFaction())
	{
		return false;
	}

	return true;
}
