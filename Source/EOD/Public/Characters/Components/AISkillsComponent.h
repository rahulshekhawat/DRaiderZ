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

	/** Event called when a skill gets cancelled */
	virtual void OnSkillCancelled(uint8 SkillIndex, FName SkillGroup, UGameplaySkillBase* Skill) override;

	/** Event called when a skill gets finished */
	virtual void OnSkillFinished(uint8 SkillIndex, FName SkillGroup, UGameplaySkillBase* Skill) override;

	FName GetMostWeightedSkillID(const AEODCharacterBase* TargetCharacter) const;

	/** Returns the melee attack skill that is more appropriate to use in current state against the given enemy */
	FName GetMostWeightedMeleeSkillID(const AEODCharacterBase* TargetCharacter) const;

	FName GetMostWeightedRangedSkillID(const AEODCharacterBase* TargetCharacter) const;

	FName GetHealingSkillID(bool bPartyHeal = false) const;

	FName GetWeightedBuffSkillID(bool bPartyBuff = false) const;

	FName GetWeightedDebuffSkillID() const;

	void GenerateSkillTypesList();

protected:

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, Category = Skills, BlueprintReadOnly)
	float MaxMeleeRange;

	// --------------------------------------
	//  Cache
	// --------------------------------------

	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly)
	TArray<FName> MeleeSkills;

	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly)
	TArray<FName> RangedSkills;

	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly)
	TArray<FName> SelfHealingSkills;
	
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly)
	TArray<FName> PartyHealingSkills;

	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly)
	TArray<FName> SelfBuffSkills;
	
	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly)
	TArray<FName> PartyBuffSkills;

	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly)
	TArray<FName> DebuffSkills;

private:

	FName GetRandomUnblockableSkill(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const;
	FName GetRandomUndodgableSkill(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const;
	FName GetRandomInterruptSkill(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const;
	FName GetRandomCCESkill(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const;
	FName GetRandomNormalSkill(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const;
	FName GetAnyRandomSkill(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const;

	inline TArray<FName> GetAvailableSkills(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const;
	inline FName GetRandomName(const TArray<FName>& NameArray) const;

};

inline TArray<FName> UAISkillsComponent::GetAvailableSkills(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const
{
	if (SkillSource == SkillsToIgnore)
	{
		return TArray<FName>();
	}

	if (SkillsToIgnore.Num() == 0)
	{
		return SkillSource;
	}
	else
	{
		TArray<FName> AvailableSkills;
		for (const FName& Key : SkillSource)
		{
			if (!SkillsToIgnore.Contains(Key))
			{
				AvailableSkills.Add(Key);
			}
		}
		return AvailableSkills;
	}
}

inline FName UAISkillsComponent::GetRandomName(const TArray<FName>& NameArray) const
{
	int32 ArraySize = NameArray.Num();
	if (ArraySize > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, ArraySize - 1);
		FName RandomName = NameArray[RandomIndex];
		return RandomName;
	}
	else
	{
		return NAME_None;
	}
}
