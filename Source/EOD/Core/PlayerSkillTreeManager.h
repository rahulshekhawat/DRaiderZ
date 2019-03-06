// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLibrary.h"
#include "GameFramework/Info.h"
#include "PlayerSkillTreeManager.generated.h"

class UGameplaySkillBase;

/** Struct containing skill tree slot information */
USTRUCT(BlueprintType)
struct EOD_API FSkillTreeSlot
{
	GENERATED_USTRUCT_BODY()

public:
	/** Determines if the skill placed in this slot is unlocked by default */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUnlockedByDefault;

	/** Vocation that this skill slot belongs to */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EVocations Vocation;

	/** Player skill associated with this slot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplaySkillBase> PlayerSkill;

	int32 GetMaxSkillSlotLevel() const { return 0; } // SlotSkills.Num(); }

	FSkillTreeSlot()
	{
		bUnlockedByDefault = true;
		Vocation = EVocations::Berserker;

	}
};


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class EOD_API APlayerSkillTreeManager : public AInfo
{
	GENERATED_BODY()

public:
	APlayerSkillTreeManager(const FObjectInitializer& ObjectInitializer);
	
	/** Classes of all the player skills that this skill tree contains */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Tree|Skill Layout")
	TArray<TSubclassOf<UGameplaySkillBase>> PlayerSkillClasses;



};
