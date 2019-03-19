// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/CharacterLibrary.h"

#include "Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "SkillTreeComponent.generated.h"

class UPlayerSaveGame;
class AEODCharacterBase;
class UGameplaySkillBase;
class USkillPointsInfoWidget;
class UDynamicSkillTreeWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EOD_API USkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	USkillTreeComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

public:

	// --------------------------------------
	//	User Interface
	// --------------------------------------

	FORCEINLINE UDynamicSkillTreeWidget* GetSkillTreeWidget() const { return SkillTreeWidget; }

	FORCEINLINE USkillPointsInfoWidget* GetSkillPointsInfoWidget() const { return SkillPointsInfoWidget; }

	void ToggleSkillTreeUI();

	// void InitializeSkillTreeWidget();

	/** Create skill tree widget and initialize the skill tree layout */
	void CreateAndInitializeSkillTreeWidget();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<USkillPointsInfoWidget> SkillPointsInfoWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UDynamicSkillTreeWidget> SkillTreeWidgetClass;

private:

	UPROPERTY(Transient)
	UDynamicSkillTreeWidget* SkillTreeWidget;

	UPROPERTY(Transient)
	USkillPointsInfoWidget* SkillPointsInfoWidget;

public:

	// --------------------------------------
	//	Skill Tree Layout
	// --------------------------------------

	FORCEINLINE TMap<FName, UGameplaySkillBase*> GetActivePlayerSkillsMap() const { return PlayerSkillsMap; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Layout")
	UDataTable* SkillTreeLayoutTable;

	UPROPERTY(Transient)
	TMap<FName, UGameplaySkillBase*> PlayerSkillsMap;

private:

	void InitializeSkills();

	void InitializeSkillTreeSlots();
	
	void Internal_LoadSkill(const FName& SkillGroup,
		const TSubclassOf<UGameplaySkillBase>& SkillClass,
		AEODCharacterBase* const SkillOwner,
		UPlayerSaveGame* const SaveGame);

};
