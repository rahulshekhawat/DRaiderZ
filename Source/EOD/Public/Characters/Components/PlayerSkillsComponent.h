// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

};
