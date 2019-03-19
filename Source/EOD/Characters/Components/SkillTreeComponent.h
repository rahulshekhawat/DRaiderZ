// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/CharacterLibrary.h"

#include "Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "SkillTreeComponent.generated.h"

class UDynamicHUDWidget;
class UPlayerSaveGame;
class AEODCharacterBase;
class AEODPlayerController;
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

	/** Initializes the skill tree widget. Called from Player Controller */
	void InitializeSkillTreeWidget();

private:

	UPROPERTY(Transient)
	UDynamicSkillTreeWidget* SkillTreeWidget;

	UPROPERTY(Transient)
	USkillPointsInfoWidget* SkillPointsInfoWidget;

public:

	// --------------------------------------
	//	Skill Tree Layout and Points Allocation
	// --------------------------------------

	/** Attempt to allocate a skill point to a slot associated with the given SkillGroup. Returns true if the point allocation was successful */
	bool AttemptPointAllocationToSlot(FName SkillGroup);

	/** Returns true if player can currently allocate a point to slot associated with the given SkillGroup */
	bool CanAllocatePointToSlot(FName SkillGroup);

protected:

	UPROPERTY(Transient)
	FSkillPointsAllocationInfo SkillPointsAllocationInfo;

	UPROPERTY(Transient)
	TMap<FName, FSkillTreeSlotSaveData> SkillTreeSlotsSaveData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Layout")
	int32 SkillPointsUnlockedByDefault;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Layout")
	UDataTable* SkillTreeLayoutTable;


};
