// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLibrary.h"
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

	/** Initialize this skill. Intended to be called immediately after the skill object is created */
	virtual void InitSkill(AEODCharacterBase* Instigator, AController* Owner);

	//~ @todo methods to manage skills
	/** Activate this skill */
	// virtual void ActivateSkill();

	/** Deactivate this skill */
	// virtual void EndSkill();

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



};
