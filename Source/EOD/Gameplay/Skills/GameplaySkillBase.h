// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLibrary.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "GameplaySkillBase.generated.h"

class AEODCharacterBase;
class AController;

/**
 * The base class for all gameplay skills
 */
UCLASS()
class EOD_API UGameplaySkillBase : public UObject
{
	GENERATED_BODY()

public:
	UGameplaySkillBase(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//	Gameplay Skill Interface
	// --------------------------------------

	/** Initialize this skill. Intended to be called immediately after the skill object is created */
	virtual void InitSkill(AEODCharacterBase* Instigator, AController* Owner);

	/** Activate this skill */
	virtual void ActivateSkill();

	/** Cancel this skill */
	virtual void CancelSkill();

	/** Deactivate this skill */
	virtual void EndSkill();

	/** Returns true if this skill is valid, i.e, skill belongs to a valid skill group */
	FORCEINLINE bool IsValid() const { return SkillGroup != NAME_None; }

protected:

	/** Returns true if this activity can be activated */
	virtual bool CanActivateSkill() const;

	/** Returns true if this skill can be cancelled */
	virtual bool CanCancelSkill() const;

protected:

	// --------------------------------------
	//	Constants : Default values that are not supposed to be modified
	// --------------------------------------

	/** Name of this skill that will be displayed to player inside the game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Base Information")
	FString InGameName;

	/** Description of this skill that will be displayed to player inside the game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Base Information")
	FString Description;

	/** Icon used to represent this skill inside the game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Base Information")
	TSoftObjectPtr<UTexture> Icon;

	/** Maximum upgrades available for this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Base Information")
	int32 MaxUpgrades;

	/** Skill group of this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants|Base Information")
	FName SkillGroup;

	// --------------------------------------
	//	Gameplay Tags
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer AbilityTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer ActivationOwnedTags;




};
