// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/GameplaySkillsComponent.h"
#include "AISkillsComponent.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UAISkillsComponent : public UGameplaySkillsComponent
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UAISkillsComponent(const FObjectInitializer& ObjectInitializer);

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// --------------------------------------
	//  Gameplay
	// --------------------------------------

	virtual void InitializeSkills(AEODCharacterBase* CompOwner = nullptr) override;

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

	/** Returns the melee attack skill that is more appropriate to use in current state against the given enemy */
	FName GetMostWeightedMeleeSkillID(const AEODCharacterBase* TargetCharacter) const;

	void GenerateSkillTypesList();

private:
	
	/** Cache for skills that can knock down enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> KnockDownSkills;

	/** Cache for skills that can knock back enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> KnockBackSkills;

	/** Cache for skills that can interrupt enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> InterruptSkills;

	/** Cache for skills that can stun enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> StunSkills;
	
	/** Cache for skills that can crystalize enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> CrystalizeSkills;

	/** Cache for skills that can flinch enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> FlinchSkills;

	/** Cache for ranged skills */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> RangedSkills;

	/** Cache for melee skills */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> MeleeSkills;

	/** Cache for self healing skills */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> SelfHealSkills;

	/** Cache for party healing skills */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> PartyHealSkills;

	/** Cache for skills that put a debuff on enemies */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> DebuffSkills;

	/** Cache for self buff skills */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> SelfBuffSkills;

	/** Cache for party buff skills */
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FName> PartyBuffSkills;

	/** A map of skills to their usability weight */
	// UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TMap<FName, int32> SkillPriorityMap;

	/** A map of skills to their usability weight */
	// UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// TMap<FName, int32> SkillWeightMap;


};
