// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponLibrary.h"
#include "CombatLibrary.h"
#include "UObject/NoExportTypes.h"
#include "CharacterLibrary.generated.h"

class UAnimMontage;

/**
 * This enum describes the character movement direction relative to character's line of sight.
 * e.g, If the character is moving to the left of the direction it is looking in then ECharMovementDirection would be Left.
 */
UENUM(BlueprintType)
enum class ECharMovementDirection : uint8
{
	None,
	F 		UMETA(DisplayName = "Forward"),
	B 		UMETA(DisplayName = "Backward"),
	L 		UMETA(DisplayName = "Left"),
	R 		UMETA(DisplayName = "Right"),
	FL 		UMETA(DisplayName = "Forward Left"),
	FR 		UMETA(DisplayName = "Forward Right"),
	BL 		UMETA(DisplayName = "Backward Left"),
	BR 		UMETA(DisplayName = "Backward Right"),
};

/** This enum describes the cause of character death */
UENUM(BlueprintType)
enum class ECauseOfDeath : uint8
{
	ZeroHP,
	Suicide,
	FellOutOfWorld,
};

/** This enum descibes the faction of in-game character */
UENUM(BlueprintType)
enum class EFaction : uint8
{
	Rendel_Commoner, // People of rendel kingdom
	Rendel_Soldier,
	Undead,
	Corrupted,
	Player,
};

UENUM(BlueprintType)
enum class ESpecies : uint8
{
	Human,
	Goblin,
	Ghoul,
	Skeleton,
	Yeti,
	Troll,
	Ogre,
};

/**
 * This enum describes the gender of human character
 * Mainly used to determine the armor mesh and animations to use
 */
UENUM(BlueprintType)
enum class ECharacterGender : uint8
{
	Female,
	Male
};

/** This enum describes the current action/state of character */
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	IdleWalkRun,
	AutoRun,
	Jumping,
	Dodging,
	Blocking,
	Attacking,
	Looting,
	SpecialAction,
	Interacting, 			// Interacting with another character, i.e., engaged in dialogue
	UsingActiveSkill,
	CastingSpell,
	SpecialMovement,
	GotHit,
	Dead
};

/** A struct containing current skill state */
USTRUCT(BlueprintType)
struct EOD_API FSkillState
{
	GENERATED_USTRUCT_BODY()

public:

	/** The current level that this skill has been upgraded to */
	UPROPERTY()
	int32 CurrentUpgradeLevel;
	
	/** The maximum level that this skill can be upgraded to */
	UPROPERTY()
	int32 MaxUpgradeLevel;

	/** Determines whether this skill has been unlocked on the skill tree */
	UPROPERTY()
	bool bUnlocked;

};

/**
 * This struct holds strong pointers to animation montages that are compatible with player's current equipped weapon.
 * @note This struct does not contain pointer to facial animations because facial animations are independent of the weapon equipped.
 */
USTRUCT(BlueprintType)
struct EOD_API FPlayerAnimationReferences
{
	GENERATED_USTRUCT_BODY()

public:

	//~ @note Words like `JumpStart` and `JumpEnd` have been intentionally capitalized because
	//~ they are animation montage section names

	UPROPERTY(BlueprintReadOnly, Category = AnimationReferences)
	FName FPlayerAnimationReferencesTableRowID;

	/** Contains animations for player JumpStart, JumpLoop, and JumpEnd */
	UPROPERTY(BlueprintReadOnly, Category = AnimationReferences)
	UAnimMontage* AnimationMontage_Jump;
	
	/** Contains animations for ForwardDodge, BackwardDodge, LeftDodge, and RightDodge */
	UPROPERTY(BlueprintReadOnly, Category = AnimationReferences)
	UAnimMontage* AnimationMontage_Dodge;
	
	/** Contains animations for:
	 * FirstSwing, FirstSwingEnd
	 * SecondSwing, SecondSwingEnd
	 * ...
	 * ForwardSwing, ForwardSwingEnd
	 * BackwardSwing, BackwardSwingEnd
	 * ...
	 */
	UPROPERTY(BlueprintReadOnly, Category = AnimationReferences)
	UAnimMontage* AnimationMontage_NormalAttacks;

	//~ @note Add AnimationMontage_WeaponChange animations here
	//~ @todo List montage section names for AnimationMontage_SpecialActions
	UPROPERTY(BlueprintReadOnly, Category = AnimationReferences)
	UAnimMontage* AnimationMontage_SpecialActions;
	
	/**
	 * Contains animations for instant skills.
	 * Section name will be same as skill name
	 */
	UPROPERTY(BlueprintReadOnly, Category = AnimationReferences)
	UAnimMontage* AnimationMontage_Skills;
	
	/**
	 * Contains animations for spells
	 * Section name will be same as spell name
	 */
	UPROPERTY(BlueprintReadOnly, Category = AnimationReferences)
	UAnimMontage* AnimationMontage_Spells;
	
	//~ @todo documentation
	UPROPERTY(BlueprintReadOnly, Category = AnimationReferences)
	UAnimMontage* AnimationMontage_SpecialMovement;
	
	//~ @todo List montage section names for AnimationMontage_CrowdControlEffects
	/** Contains animations for crowd control effects like interrupted, frozen, etc. */
	UPROPERTY(BlueprintReadOnly, Category = AnimationReferences)
	UAnimMontage* AnimationMontage_HitEffects;
	
	/** Contains animations for player flinching */
	UPROPERTY(BlueprintReadOnly, Category = AnimationReferences)
	UAnimMontage* AnimationMontage_Flinch;

	// @todo Death animations

};


/**
 * This struct is equivalent of FPlayerAnimationReferences except that it contains
 * string references to animations instead of strong pointers. This will be used to
 * construct the player animation references data table
 * @see FPlayerAnimationReferences
 */
USTRUCT(BlueprintType)
struct EOD_API FPlayerAnimationReferencesTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	
	/** Reference to player animation montage that contains animations for jumping */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> Jump;
	
	/** Reference to player animation montage that contains animations for dodging */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> Dodge;
	
	/** Reference to player animation montage that contains animations for normal attacks */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> NormalAttacks;

	//~ @note Add AnimationMontage_WeaponChange animations here
	/** Reference to player animation montage that contains animations for special actions (@todo list special actions) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> SpecialActions;
	
	/** Reference to player animation montage that contains animations for using weapon skils */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> Skills;
	
	/** Reference to player animation montage that contains animations for spells */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> Spells;
	
	/** Reference to player animation montage that contains animations for special movement (@todo more info) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> SpecialMovement;
	
	/** Reference to player animation montage that contains animations for crowd control effects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> HitEffects;
	
	/** Reference to player animation montage that contains animations for flinching */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> Flinch;

	// @todo Death animations

};

/*
UENUM(BlueprintType)
enum class ESkillCastType : uint8
{
	// Skills that are cast instantly, i.e., almost all physical attacks and magic attacks such as rapid blast and divine punishment
	InstantCast,
	// Skills that require time to cast, i.e., spells
	DelayedCast,
	// Passives skill that can't be cast
	NoCast
};
*/

UENUM(BlueprintType)
enum class EEODTaskStatus : uint8
{
	Active,
	Inactive,
	Finished,
	Aborted
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	DamageMelee,
	DamageRanged,
	HealSelf,
	HealParty,
	BuffSelf,
	BuffParty,
	DebuffEnemy
};

/**
 * Struct containing information for skills that will be used by AI characters
 * 
 * An AI skill doesn't have any in-game name, description, and an icon
 * An AI skill can never be a passive skill
 * An AI skill doesn't have a preceding or superseding skill
 * An AI skill may require a status effect for activation (i.e., an skill that a monster can only use when it's in rage state)
 */
USTRUCT(BlueprintType)
struct EOD_API FAISkillTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	
	/** Animation montage containing animation for this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	UAnimMontage* AnimMontage;

	/** Section name of skill start animation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	FName SkillStartMontageSectionName;

	/** Section name of skill loop animation (used in spell casting) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	FName SkillLoopMontageSectionName;

	/** Section name of skill end animation (used in spell casting) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	FName SkillEndMontageSectionName;
	
	/** Type of damage inflicted from this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	EDamageType DamageType;

	/** Determines skill type which will be used by AI for calculating most suitable skill to use */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	ESkillType SkillType;

	/** The status effect required to use this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	FName RequiredStatusID;

	/** The duration before which the same skill can be used again */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float Cooldown;

	/** The duration of loop section if the skill has loop section */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float LoopDuration;

	/** Damage in percentage of player's magickal or physical attack that will be inflicted on enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 DamagePercent;

	/** Determines if this skill can be blocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bUnblockable;

	/** Determines if this skill can be dodged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bUndodgable;

	/** Determines if the 'skill deflected' animation should play on getting blocked. Only applicable if this skill can be blocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bIgnoresBlock;

	/** Crowd control effect on hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	ECrowdControlEffect CrowdControlEffect;

	/** The duration for which the crowd control effect should last (if applicable) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float CrowdControlEffectDuration;

	/** Immunities from crowd control effects granted on using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills, meta = (Bitmask, BitmaskEnum = "ECrowdControlEffect"))
	uint8 CrowdControlImmunities;

	/** Status effect that this skill triggers */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	TSubclassOf<class UStatusEffectBase> StatusEffect;

	FAISkillTableRow()
	{
		SkillStartMontageSectionName = FName("Default");
		SkillLoopMontageSectionName = NAME_None;
		SkillEndMontageSectionName = NAME_None;
	}
};

/** Struct containing information for skills that will be used by player character */
USTRUCT(BlueprintType)
struct EOD_API FPlayerSkillTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	/** In-game name of this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FString InGameName;

	/** In-game description of this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FString Description;

	/** In-game icon representation of this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	UTexture* Icon;

	/** Animation montage containing animation for this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	TSoftObjectPtr<UAnimMontage> AnimMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills, meta = (Bitmask, BitmaskEnum = "EWeaponType"))
	uint8 SupportedWeapons;

	/** Section name of skill start animation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	FName SkillStartMontageSectionName;

	/** Section name of skill loop animation (used in spell casting) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	FName SkillLoopMontageSectionName;

	/** Section name of skill end animation (used in spell casting) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	FName SkillEndMontageSectionName;
	
	/** Type of damage inflicted from this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	EDamageType DamageType;

	/** Determines skill type which will be used by AI for calculating most suitable skill to use */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	ESkillType SkillType;

	/** Minimum stamina required to use this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 StaminaRequired;
	
	/** Minimum mana required to use this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 ManaRequired;

	/** Determines whether this skill is a passive skill or an active skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bPassiveSkill;

	/** Skills, any of which MUST be used before using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	TArray<FString> PrecedingSkillGroups;

	/** Skill that can be used after using this skill (skill chaining) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FString SupersedingSkillGroup;

	/**
	 * SkillID for skill that comes on upgrading this skill.
	 * This will be NAME_None if the skill can't be upgraded any further
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName UpgradeSkillID;
	
	/**
	 * SkillID for skill from which you can upgrade to this skill.
	 * This will be NAME_None for a base skill
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName DowngradeSkillID;
	
	/** Current upgrade level of this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 CurrentUpgradeLevel;

	/** Maximum upgrades available for this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 MaxUpgrades;

	/** The status effect required to use this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skills)
	FName RequiredStatusID;

	/** The duration before which the same skill can be used again */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float Cooldown;

	/** The duration of loop section if the skill has loop section */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float LoopDuration;

	/** Damage in percentage of player's magickal or physical attack that will be inflicted on enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 DamagePercent;

	/** Determines if this skill can be blocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bUnblockable;

	/** Determines if this skill can be dodged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bUndodgable;

	/** Determines if the 'skill deflected' animation should play on getting blocked. Only applicable if this skill can be blocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bIgnoresBlock;

	/** Crowd control effect on hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	ECrowdControlEffect CrowdControlEffect;

	/** The duration for which the crowd control effect should last (if applicable) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float CrowdControlEffectDuration;

	/** Immunities from crowd control effects granted on using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills, meta = (Bitmask, BitmaskEnum = "ECrowdControlEffect"))
	uint8 CrowdControlImmunities;

	/** Status effect that this skill triggers */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	TSubclassOf<class UStatusEffectBase> StatusEffect;

	FPlayerSkillTableRow()
	{
		SkillStartMontageSectionName = FName("Default");
		SkillLoopMontageSectionName = NAME_None;
		SkillEndMontageSectionName = NAME_None;
	}
};

/** Struct containing information regarding last used character skill */
USTRUCT(BlueprintType)
struct FLastUsedSkillInfo
{
	GENERATED_USTRUCT_BODY()

public:

	/** SkillID of last used skill */
	UPROPERTY(Transient, BlueprintReadOnly, Category = LastUsedSkill)
	FName LastUsedSkillID;

	/** True if the last skill was interrupted */
	UPROPERTY(Transient, BlueprintReadOnly, Category = LastUsedSkill)
	bool bInterrupted;

	FLastUsedSkillInfo()
	{
		bInterrupted = false;
		LastUsedSkillID = NAME_None;
	}
};

/** Struct containing level specific info for an in-game skill */
USTRUCT(BlueprintType, Blueprintable)
struct EOD_API FSkillLevelUpInfo
{
	GENERATED_USTRUCT_BODY()

public:

	/** Minimum stamina required to use this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 StaminaRequired;
	
	/** Minimum mana required to use this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 ManaRequired;
	
	/** The duration before which the same skill can be used again */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float Cooldown;

	/** The duration of loop section if the skill has loop section */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float LoopDuration;

	/** Damage in percentage of player's magickal or physical attack that will be inflicted on enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	int32 DamagePercent;

	/** Determines if this skill can be blocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bUnblockable;

	/** Determines if this skill can be dodged */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bUndodgable;

	/** Determines if the 'skill deflected' animation should play on getting blocked. Only applicable if this skill can be blocked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bIgnoresBlock;

	/** Crowd control effect on hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	ECrowdControlEffect CrowdControlEffect;

	/** The duration for which the crowd control effect should last (if applicable) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	float CrowdControlEffectDuration;

	/** Immunities from crowd control effects granted on using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills, meta = (Bitmask, BitmaskEnum = "ECrowdControlEffect"))
	uint8 CrowdControlImmunities;

	/** Status effect that this skill triggers */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	TSubclassOf<class UStatusEffectBase> StatusEffect;

};

/** Table row struct for in-game skills */
USTRUCT(BlueprintType, Blueprintable)
struct EOD_API FSkillTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FString InGameName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	TSoftObjectPtr<UTexture> Icon;
	
	/**
	 * The montage that contains the animation(s) of this skill for first gender.
	 * If the character supports gender (i.e. player character), first gender would be female.
	 * Otherwise whatever the default gender character is.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	TSoftObjectPtr<UAnimMontage> AnimMontage_GenderOne;

	/** The montage that contains the animation(s) of this skill for second gender */
	/**
	 * The montage that contains the animation(s) of this skill for second gender.
	 * If the character supports gender (i.e. player character), second gender would be female.
	 * Otherwise NULL.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	TSoftObjectPtr<UAnimMontage> AnimMontage_GenderTwo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills, meta = (Bitmask, BitmaskEnum = "EWeaponType"))
	uint8 SupportedWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName SkillStartMontageSectionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName SkillLoopMontageSectionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName SkillEndMontageSectionName;
	
	/** Type of damage inflicted from this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	EDamageType DamageType;

	/** Determines skill type which will be used by AI for calculating most suitable skill to use */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	ESkillType SkillType;
	
	/** Determines whether this skill is a passive skill or an active skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	bool bPassiveSkill;

	/** SkillID for skill that MUST be used before using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName PrecedingSkillID;

	/** SkillID for skill that can be used after using this skill (skill chaining)  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName SupersedingSkillID;

	/** The status effect required to use this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName RequiredStatusID;

	//~ Maximum number of level ups = SkillLevelUpsInfo.Num()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	TArray<FSkillLevelUpInfo> SkillLevelUpsInfo;

	FSkillTableRow()
	{
		SkillStartMontageSectionName = FName("Default");
		SkillLoopMontageSectionName = NAME_None;
		SkillEndMontageSectionName = NAME_None;
	}
};

/** In-game skill */
USTRUCT(BlueprintType, Blueprintable)
struct FSkill
{
	GENERATED_USTRUCT_BODY()

public:

	// FName SkillID; // What was this for?

	UPROPERTY(BlueprintReadWrite, Category = Skills)
	FString InGameName;

	UPROPERTY(BlueprintReadWrite, Category = Skills)
	FString Description;

	UPROPERTY(BlueprintReadWrite, Category = Skills)
	UTexture* Icon;

	UPROPERTY(BlueprintReadWrite, Category = Skills)
	uint8 CurrentSkillLevel;

	UPROPERTY(BlueprintReadWrite, Category = Skills)
	UAnimMontage* AnimationMontage_GenderOne;

	UPROPERTY(BlueprintReadWrite, Category = Skills)
	UAnimMontage* AnimationMontage_GenderTwo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills, meta = (Bitmask, BitmaskEnum = "EWeaponType"))
	uint8 SupportedWeapons;

	UPROPERTY(BlueprintReadWrite, Category = Skills)
	FName SkillStartMontageSectionName;

	UPROPERTY(BlueprintReadWrite, Category = Skills)
	FName SkillLoopMontageSectionName;

	UPROPERTY(BlueprintReadWrite, Category = Skills)
	FName SkillEndMontageSectionName;

	UPROPERTY(BlueprintReadWrite, Category = Skills)
	EDamageType DamageType;

	/** SkillID for skill that MUST be used before using this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName PrecedingSkillID;

	/** SkillID for skill that can be used after using this skill (skill chaining)  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName SupersedingSkillID;

	/** The status effect required to use this skill */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skills)
	FName RequiredStatusID;

	UPROPERTY(BlueprintReadWrite, Category = Skills)
	FSkillLevelUpInfo SkillLevelUpInfo;

	FSkill();

	FSkill(FSkillTableRow* SkillTableRow, uint8 SkillLevel = 1);

};

/**
 * CharacterLibrary contains static helper functions for in-game characters.
 * @note Do not inherit from this class
 */
UCLASS()
class EOD_API UCharacterLibrary : public UObject
{
	GENERATED_BODY()
	
public:

	UCharacterLibrary(const FObjectInitializer& ObjectInitializer);
	
	//~ @note Blueprints don't support raw struct pointers, therefore it can't be BlueprintCallable
	/** Returns player animation references based on the EWeaponAnimationType of player */
	static FPlayerAnimationReferences* GetPlayerAnimationReferences(EWeaponAnimationType PlayerWeaponAnimationType, ECharacterGender Gender = ECharacterGender::Female);

	// static FSkill* GetPlayerSkill(FName SKillID, uint8 SkillLevel = 1);
	static FPlayerSkillTableRow* GetPlayerSkill(const FName SkillID, const FString& ContextString);

	/** Attempts to unload player animation references, returns true if successful */
	static bool UnloadPlayerAnimationReferences(FPlayerAnimationReferences* PlayerAnimationReferences, ECharacterGender Gender = ECharacterGender::Female);

	static bool AreEnemies(AEODCharacterBase* CharacterOne, AEODCharacterBase* CharacterTwo);

	static void GetAllAICharacterSkills(const FString& CharacterName, const UDataTable* SkillsDataTable, TArray<FSkill*> &OutSkills);

	//~ Begin anim montage section names
	static const FName SectionName_ForwardFlinch;
	
	static const FName SectionName_BackwardFlinch;

	static const FName SectionName_ForwardInterrupt;

	static const FName SectionName_BackwardInterrupt;

	static const FName SectionName_AttackDeflected;

	static const FName SectionName_BlockAttack;

	static const FName SectionName_KnockdownStart;

	static const FName SectionName_KnockdownIdle;

	static const FName SectionName_KnockdownEnd;

	static const FName SectionName_LavitationStart;

	static const FName SectionName_LavitationEnd;

	static const FName SectionName_StunStart;

	static const FName SectionName_StunLoop;

	static const FName SectionName_StunEnd;

	static const FName SectionName_JumpStart;

	static const FName SectionName_JumpLoop;

	static const FName SectionName_JumpEnd;

	static const FName SectionName_FirstSwing;

	static const FName SectionName_FirstSwingEnd;

	static const FName SectionName_SecondSwing;

	static const FName SectionName_SecondSwingEnd;

	static const FName SectionName_ThirdSwing;

	static const FName SectionName_ThirdSwingEnd;

	static const FName SectionName_FourthSwing;

	static const FName SectionName_FourthSwingEnd;

	static const FName SectionName_FifthSwing;

	static const FName SectionName_FifthSwingEnd;

	static const FName SectionName_ForwardSPSwing;

	static const FName SectionName_ForwardSPSwingEnd;

	static const FName SectionName_BackwardSPSwing;

	static const FName SectionName_BackwardSPSwingEnd;

	static const FName SectionName_ForwardDodge;

	static const FName SectionName_BackwardDodge;

	static const FName SectionName_LeftDodge;

	static const FName SectionName_RightDodge;
	//~ End anim montage section names

};