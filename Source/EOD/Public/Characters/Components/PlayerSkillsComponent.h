// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplaySkillsComponent.h"
#include "PlayerSkillsComponent.generated.h"

class UGameplaySkillBase;
class UContainerWidget;

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

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// --------------------------------------
	//  Input Handling
	// --------------------------------------

	void OnPressingSkillKey(const int32 SkillKeyIndex);

	void OnReleasingSkillKey(const int32 SkillKeyIndex);

	// --------------------------------------
	//  Skill System
	// --------------------------------------

	virtual void InitializeSkills(AEODCharacterBase* CompOwner = nullptr) override;

	void OnSkillAddedToSkillBar(uint8 SkillBarIndex, FName SkillGroup);

	virtual void TriggerSkill(uint8 SkillIndex, UGameplaySkillBase* Skill = nullptr) override;

	virtual void ReleaseSkill(uint8 SkillIndex, UGameplaySkillBase* Skill = nullptr, float ReleaseDelay = 0.f) override;

	FORCEINLINE TMap<uint8, UGameplaySkillBase*> GetSkillsMap() const { return SkillsMap; }

	FORCEINLINE TMap<uint8, uint8> GetSkillBarMap() const { return SkillBarMap; }

	virtual void UpdateSkillCooldown(FName SkillGroup, float RemainingCooldown) override;

	TArray<UContainerWidget*> GetAllContainerWidgetsForSkill(FName SkillGroup) const;

protected:

	/** Skill index to skil map. Skill index will be used during replication */
	UPROPERTY(Transient)
	TMap<uint8, UGameplaySkillBase*> SkillsMap;

	/** A map of all the skills placed on skill bar to their Skill Index */
	UPROPERTY(Transient)
	TMap<uint8, uint8> SkillBarMap;

	bool bSkillCharging;

	float SkillChargeDuration;

	// --------------------------------------
	//  Network
	// --------------------------------------

	virtual void Server_TriggerSkill_Implementation(uint8 SkillIndex) override;
	virtual void Server_ReleaseSkill_Implementation(uint8 SkillIndex, float ChargeDuration) override;



};
