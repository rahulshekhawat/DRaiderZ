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

	/** Returns true if any skill point is available to be allocated */
	FORCEINLINE bool IsAnySkillPointAvailable() const { return SkillPointsAllocationInfo.AvailableSkillPoints > 0; }

	/** Attempt to allocate a skill point to a slot associated with the given SkillGroup. Returns true if the point allocation was successful */
	bool AttemptPointAllocationToSlot(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo = nullptr);

	/** Returns true if any point has been allocated to the skill slot associated with the SkillGroup already */
	bool IsAnySkillPointAllocatedToSlot(FName SkillGroup);

	/** Returns true if a skill point can be allocated to this skill slot */
	bool IsSkillAvailable(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo = nullptr);

	/** Returns the status of this skill slot */
	ESkillSlotStatus GetSkillSlotStatus(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo = nullptr);

	/** Returns true if the skill is completely blocked, i.e., no point has been allocated to it so far nor any point can be allocated to it   */
	// bool IsSkillBlocked(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo = nullptr);

	/** Returns true if skill is not blocked but no skill point can be allocated to it as of now */
	// bool IsSkillLocked(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo = nullptr);

protected:

	UPROPERTY(Transient)
	TMap<FName, FSkillTreeSlotSaveData> SkillTreeSlotsSaveData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Layout")
	int32 SkillPointsUnlockedByDefault;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Layout")
	UDataTable* SkillTreeLayoutTable;

private:

	UPROPERTY(Transient)
	FSkillPointsAllocationInfo SkillPointsAllocationInfo;

	/** Returns true if player can currently allocate a point to slot associated with the given SkillGroup */
	bool CanAllocatePointToSlot(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo = nullptr);

	// --------------------------------------
	//	Skill points modification
	// --------------------------------------

	inline void ModifyAllocatedPointsAssassin(int32 Value);
	inline void ModifyAllocatedPointsBerserker(int32 Value);
	inline void ModifyAllocatedPointsCleric(int32 Value);
	inline void ModifyAllocatedPointsDefender(int32 Value);
	inline void ModifyAllocatedPointsSorcerer(int32 Value);
	inline void ModifyAvailableSkillPoints(int32 Value);
	inline void ModifyUsedSkillPoints(int32 Value);

	void SetAllocatedPointsAssassin(int32 Value);
	void SetAllocatedPointsBerserker(int32 Value);
	void SetAllocatedPointsCleric(int32 Value);
	void SetAllocatedPointsDefender(int32 Value);
	void SetAllocatedPointsSorcerer(int32 Value);
	void SetAvailableSkillPoints(int32 Value);
	void SetUsedSkillPoints(int32 Value);

	void ModifySkillSlotUpgrade(FName SkillGroup, int32 Value);
	void SetSkillSlotUpgrade(FName SkillGroup, int32 Value);

	inline void SetSkillPointsAllocationInfo(const FSkillPointsAllocationInfo& NewInfo);
};

void USkillTreeComponent::ModifyAllocatedPointsAssassin(int32 Value)
{
	SetAllocatedPointsAssassin(SkillPointsAllocationInfo.AssassinPoints + Value);
}

void USkillTreeComponent::ModifyAllocatedPointsBerserker(int32 Value)
{
	SetAllocatedPointsBerserker(SkillPointsAllocationInfo.BerserkerPoints + Value);
}

inline void USkillTreeComponent::ModifyAllocatedPointsCleric(int32 Value)
{
	SetAllocatedPointsCleric(SkillPointsAllocationInfo.ClericPoints + Value);
}

inline void USkillTreeComponent::ModifyAllocatedPointsDefender(int32 Value)
{
	SetAllocatedPointsDefender(SkillPointsAllocationInfo.DefenderPoints + Value);
}

inline void USkillTreeComponent::ModifyAllocatedPointsSorcerer(int32 Value)
{
	SetAllocatedPointsSorcerer(SkillPointsAllocationInfo.SorcererPoints + Value);
}

inline void USkillTreeComponent::ModifyAvailableSkillPoints(int32 Value)
{
	SetAvailableSkillPoints(SkillPointsAllocationInfo.AvailableSkillPoints + Value);
}

inline void USkillTreeComponent::ModifyUsedSkillPoints(int32 Value)
{
	SetUsedSkillPoints(SkillPointsAllocationInfo.UsedSkillPoints + Value);
}

inline void USkillTreeComponent::SetSkillPointsAllocationInfo(const FSkillPointsAllocationInfo& NewInfo)
{
	SkillPointsAllocationInfo = NewInfo;
	SetAvailableSkillPoints(NewInfo.AvailableSkillPoints);
	SetUsedSkillPoints(NewInfo.UsedSkillPoints);
	SetAllocatedPointsAssassin(NewInfo.AssassinPoints);
	SetAllocatedPointsBerserker(NewInfo.BerserkerPoints);
	SetAllocatedPointsCleric(NewInfo.ClericPoints);
	SetAllocatedPointsDefender(NewInfo.DefenderPoints);
	SetAllocatedPointsSorcerer(NewInfo.SorcererPoints);
}
