// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/CharacterLibrary.h"
#include "UI/EODItemContainer.h"
#include "SkillTreeItemContainer.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API USkillTreeItemContainer : public UEODItemContainer
{
	GENERATED_BODY()
	
public:

	USkillTreeItemContainer(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	/**
	 * Determines the skill group this skill belongs to
	 * For a skill with SkillID 'BZ1_1', it's skill group is 'BZ1'
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillInfo)
	FString SkillGroup;

	/** Current state of this skill */
	UPROPERTY(Transient, BlueprintReadOnly, Category = SkillInfo)
	FSkillState SkillState;

	/** Skill group that must be unlocked before unlocking this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillInfo)
	FString SkillGroupRequiredToUnlock;

	/** The skill tree column that this skill container belongs to */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillInfo)
	int32 ColumnPosition;

	/** The skill tree row that this skill container belongs to */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillInfo)
	int32 RowPosition;

	/** Text block displaying skill upgrade in the form : CurrentUpgradeLevel / MaxUpgradeLevel */
	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* SkillUpgradeText;
	
private:

	/** Load previously saved skill state from current save slot */
	FORCEINLINE void LoadSkillState();


	

};
