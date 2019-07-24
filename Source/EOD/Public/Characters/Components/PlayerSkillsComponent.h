// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLibrary.h"
#include "GameplaySkillsComponent.h"
#include "PlayerSkillsComponent.generated.h"

class UContainerWidget;
class UHUDWidget;
class UGameplaySkillBase;
class UPlayerSkillBase;
class USkillBarWidget;
class USkillTreeWidget;
class USkillPointsInfoWidget;

/**
 * 
 */
UCLASS()
class EOD_API UPlayerSkillsComponent : public UGameplaySkillsComponent
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UPlayerSkillsComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// --------------------------------------
	//  Input Handling
	// --------------------------------------

	void OnPressingSkillKey(const int32 SkillKeyIndex);

	void OnReleasingSkillKey(const int32 SkillKeyIndex);

	// --------------------------------------
	//  Gameplay
	// -------------------------------------

	virtual void UpdateSkillCooldown(FName SkillGroup, float RemainingCooldown) override;

	virtual void UpdateSkillCooldown(uint8 SkillIndex, float RemainingCooldown) override;

	virtual void AddGameplayEffect(UGameplayEffectBase* GameplayEffect) override;

	virtual void RemoveGameplayEffect(UGameplayEffectBase* GameplayEffect) override;

	// --------------------------------------
	//  Skill System
	// --------------------------------------

	virtual void TriggerSkill(uint8 SkillIndex, UGameplaySkillBase* Skill = nullptr) override;

	virtual void ReleaseSkill(uint8 SkillIndex, UGameplaySkillBase* Skill = nullptr, float ReleaseDelay = 0.f) override;

	virtual void CancelSkill(uint8 SkillIndex, UGameplaySkillBase* Skill = nullptr) override;

	virtual void CancelAllActiveSkills() override;

	virtual bool CanUseAnySkill() const override;

	virtual bool CanUseSkill(uint8 SkillIndex, UGameplaySkillBase* Skill = nullptr) override;

	virtual void ActivateChainSkill(UGameplaySkillBase* CurrentSkill) override;

	void OnPlayerWeaponChanged();
	// @todo
	// void OnPlayerWeaponChanged(EWeaponType NewWeaponType, EWeaponType OldWeaponType);

	/** Attempt to add a new skill to player skill bar. Returns true if skill succesfully added */
	bool AddSkillToSkillBar(uint8 SkillBarIndex, FName SkillGroup);
	bool RemoveSkillFromSkillBar(uint8 SkillBarIndex, FName SkillGroup);
	bool SwapSkillsOnSkillBar(uint8 SBI1, FName SG1, uint8 SBI2, FName SG2);

	UPlayerSkillBase const* const GetSkillAtSkillBarIndex(uint8 SkillBarIndex) const;

	bool SaveSkillBarMap();

	inline const TMap<uint8, uint8>& GetSkillBarMap() const { return SkillBarMap; }

	TArray<UContainerWidget*> GetAllContainerWidgetsForSkill(FName SkillGroup);

protected:

	/** A map of all the skills placed on skill bar to their Skill Index */
	UPROPERTY(Transient)
	TMap<uint8, uint8> SkillBarMap;

	int32 LastPressedSkillKey;

	int32 LastReleasedSkillKey;

	virtual void ResetChainSkill() override;

	// --------------------------------------
	//  Network
	// --------------------------------------

	virtual void Server_TriggerSkill_Implementation(uint8 SkillIndex) override;
	virtual void Server_ReleaseSkill_Implementation(uint8 SkillIndex, float ChargeDuration) override;


	///////////////////////////////////////////////////////////////////////////
	//  Player Skills
public:

	inline USkillTreeWidget* GetSkillTreeWidget() const { return STWidget; }

	inline USkillBarWidget* GetSkillBarWidget() const { return SBWidget; }

	inline USkillPointsInfoWidget* GetSkillPointsInfoWidget() const { return SPIWidget; }

	void InitializeUIWidgets(AEODCharacterBase* CompOwner = nullptr);

	virtual void InitializeSkills(AEODCharacterBase* CompOwner = nullptr) override;

private:

	/** Verifies skills have been initialized correctly. Raises an exception if not */
	void VerifySkillsInitializedCorrectly();

	void UnlockPlayerSkillsFromSaveGame(AEODCharacterBase* CompOwner = nullptr);

	void InitializeSkillBarWidget(AEODCharacterBase* CompOwner = nullptr);

	void InitializeSkillTreeWidget(AEODCharacterBase* CompOwner = nullptr);

	UPROPERTY(Transient)
	UHUDWidget* HUDWidget;

	UPROPERTY(Transient)
	USkillTreeWidget* STWidget;
	
	UPROPERTY(Transient)
	USkillBarWidget* SBWidget;
	
	UPROPERTY(Transient)
	USkillPointsInfoWidget* SPIWidget;


	///////////////////////////////////////////////////////////////////////////
	//  Skill Tree Manager
public:

	/** Returns true if any skill point is available to be allocated */
	inline bool IsAnySkillPointAvailable() const { return SkillPointsAllocationInfo.AvailableSkillPoints > 0; }

	inline int32 GetSkillPointsUnlockedByDefault() const { return SkillPointsUnlockedByDefault; }

	/** Attempt to allocate a skill point to a slot associated with the given SkillGroup. Returns true if the point allocation was successful */
	bool AttemptPointAllocationToSlot(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo = nullptr);

	/** Returns true if any point has been allocated to the skill slot associated with the SkillGroup already */
	bool IsAnySkillPointAllocatedToSlot(FName SkillGroup);

	/** Returns true if a skill point can be allocated to this skill slot */
	bool IsSkillAvailable(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo = nullptr);

	/** Returns the status of this skill slot */
	// ESkillSlotStatus GetSkillSlotStatus(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo = nullptr);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SkillTree)
	UDataTable* SkillTreeLayoutTable;

protected:

	UPROPERTY(Transient)
	TMap<FName, FSkillTreeSlotSaveData> SkillTreeSlotsSaveData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Layout")
	int32 SkillPointsUnlockedByDefault;

private:

	UPROPERTY(Transient)
	FSkillPointsAllocationInfo SkillPointsAllocationInfo;

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

public:

	/** Returns true if player can currently allocate a point to slot associated with the given SkillGroup */
	bool CanAllocatePointToSlot(FName SkillGroup, FSkillTreeSlot* SkillSlotInfo = nullptr);

	inline void SetSkillPointsAllocationInfo(const FSkillPointsAllocationInfo& NewInfo);

};

inline void UPlayerSkillsComponent::ModifyAllocatedPointsAssassin(int32 Value)
{
	SetAllocatedPointsAssassin(SkillPointsAllocationInfo.AssassinPoints + Value);
}

inline void UPlayerSkillsComponent::ModifyAllocatedPointsBerserker(int32 Value)
{
	SetAllocatedPointsBerserker(SkillPointsAllocationInfo.BerserkerPoints + Value);
}

inline void UPlayerSkillsComponent::ModifyAllocatedPointsCleric(int32 Value)
{
	SetAllocatedPointsCleric(SkillPointsAllocationInfo.ClericPoints + Value);
}

inline void UPlayerSkillsComponent::ModifyAllocatedPointsDefender(int32 Value)
{
	SetAllocatedPointsDefender(SkillPointsAllocationInfo.DefenderPoints + Value);
}

inline void UPlayerSkillsComponent::ModifyAllocatedPointsSorcerer(int32 Value)
{
	SetAllocatedPointsSorcerer(SkillPointsAllocationInfo.SorcererPoints + Value);
}

inline void UPlayerSkillsComponent::ModifyAvailableSkillPoints(int32 Value)
{
	SetAvailableSkillPoints(SkillPointsAllocationInfo.AvailableSkillPoints + Value);
}

inline void UPlayerSkillsComponent::ModifyUsedSkillPoints(int32 Value)
{
	SetUsedSkillPoints(SkillPointsAllocationInfo.UsedSkillPoints + Value);
}

inline void UPlayerSkillsComponent::SetSkillPointsAllocationInfo(const FSkillPointsAllocationInfo& NewInfo)
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
