// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/Statics/CharacterLibrary.h"

#include "Components/ActorComponent.h"
#include "GameplaySkillsComponent.generated.h"

class AEODCharacterBase;
class UGameplaySkillBase;

/*
USTRUCT(BlueprintType)
struct EOD_API FSkillStatus
{
	GENERATED_USTRUCT_BODY()
public:


};
*/

/** Struct containing information regarding skill's cooldown */
USTRUCT(BlueprintType)
struct EOD_API FSkillCooldownStatus
{
	GENERATED_USTRUCT_BODY()

	bool bIsInCooldown;

	float CooldownRemaining;

	FTimerHandle CooldownTimerHandle;

	FSkillCooldownStatus()
	{
		bIsInCooldown = false;
		CooldownRemaining = 0.f;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EOD_API UGameplaySkillsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGameplaySkillsComponent(const FObjectInitializer& ObjectInitializer);

	virtual void PostLoad() override;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	////////////////////////////////////////////////////////////////////////////////
	// Skill System	
public:
	/**
	 * Trigger a skill
	 * @note This may result in either instantly activating a skill (e.g. Nocturne) or this may start charging the skill (for skills that can be
	 *		 charged by holding down the attack key)
	 */
	void TriggerSkill(FName SkillID, FSkillTableRow* Skill = nullptr);

	/**
	 * If the character is currently charging a chargeable skill then this activates the skill. Otherwise it does nothing
	 */
	void ReleaseSkill(FName SkillID, FSkillTableRow* Skill = nullptr);

	virtual void TriggerSkill(uint8 SkillIndex, UGameplaySkillBase* Skill = nullptr);

	virtual void ReleaseSkill(uint8 SkillIndex, UGameplaySkillBase* Skill = nullptr, float ReleaseDelay = 0.f);

	/** Cancels the skill that the character is currently using */
	void CancelCurrentSkill();

	/** If the character is currently using skill with ID same as the function argument, then cancel it. Otherwise do nothing. */
	void CancelSkill(FName SkillID);

	//~ @todo
	// void EndSkill(FName SkillID, FSkillTableRow* Skill = nullptr);

	virtual void InitializeSkills(AEODCharacterBase* CompOwner = nullptr);

private:
	/** Returns true if skill key index is invalid */
	inline bool IsSkillKeyIndexInvalid(const int32 SkillKeyIndex) const;

	bool CanUseAnySkill() const;

	bool CanUseSkill(FName SkillID) const;

	bool CanUseSkillAtIndex(const int32 SkillKeyIndex) const;

	FName GetPlayerSkillIDFromSG(FString& SkillGroup) const;

	/**
	 * Returns the skill group that should be used when pressing a skill key.
	 * @note It won't necessarily return the skill placed at the skill key index
	 */
	FString GetSkillGroupFromSkillKeyIndex(const int32 SkillKeyIndex) const;

	/**
	 * The skill group that will get triggered on pressing the skill key at index 'ActiveSupersedingChainSkillGroup.Key'
	 * This will auto deactivate after 'ChainSkillResetDelay' seconds.
	 */
	TPair<int32, FString> ActiveSupersedingChainSkillGroup;

	/**
	 * The skill that the player last used. 
	 * This is used to determine whether a skill that requires a 'ActivePrecedingChainSkillGroup' type skill to be used previously, can be used
	 * It will auto deactivate after 'ChainSkillResetDelay' seconds.
	 */
	FString ActivePrecedingChainSkillGroup;

	/** The skill that the character used last time */
	FString LastUsedSkillGroup;


	////////////////////////////////////////////////////////////////////////////////
	// EOD
	////////////////////////////////////////////////////////////////////////////////
private:
	UPROPERTY(Transient)
	AEODCharacterBase* EODCharacterOwner;

	// UPROPERTY(ReplicatedUsing = OnRep_ActiveSkillID)
	FName ActiveSkillID;

	FSkillTableRow* ActiveSkill;

	/** Skill button index to skill group map (this describes which skill group is placed on which skill bar button) */
	TMap<int32, FString> SBIndexToSGMap;

	/** Skill group to skill state map */
	TMap<FString, FSkillState> SGToSSMap;

	/** A map of skill group to a boolean that determines whether the skill group is currently in cooldown or not */
	TMap<FString, bool> SGToCooldownMap;

	/** A map of skill group to it's cooldown status info */
	TMap<FString, FSkillCooldownStatus> SGToCooldownStatusMap;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EOD Character Skills")
	int32 MaxNumSkills;

	UPROPERTY(BlueprintReadOnly, Category = "EOD Character Skills")
	float ChainSkillResetDelay;

	/** Data table for character skills */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Skills)
	UDataTable* SkillsDataTable;

	FORCEINLINE FSkillTableRow* GetSkill(FName SkillID, const FString& ContextString = FString("AEODCharacterBase::GetSkill(), character skill lookup")) const
	{
		return SkillsDataTable ? SkillsDataTable->FindRow<FSkillTableRow>(SkillID, ContextString) : nullptr;
	}

	void UseSkill(FName SkillID);

public:
	void SetCurrentActiveSkill(const FName SkillID);

	FORCEINLINE FName GetCurrentActiveSkillID() const { return ActiveSkillID; }

	FORCEINLINE FSkillTableRow* GetCurrentActiveSkill() const { return ActiveSkill; }

	FORCEINLINE AEODCharacterBase* GetCharacterOwner() const { return EODCharacterOwner; }

	FORCEINLINE TMap<int32, FString>& GetSkillBarLayout() { return SBIndexToSGMap; }

	void LoadSkillBarLayout();

	void SaveSkillBarLayout();

	/**
	 * Add new skill to the skill bar.
	 * Used to add new skills to the skill bar through UI
	 */
	UFUNCTION()
	void AddNewSkill(int32 SkillIndex, FString SkillGroup);


	UPROPERTY(EditDefaultsOnly, Category = Skills)
	TSubclassOf<UGameplaySkillBase> TestSkill;

	UPROPERTY(Transient)
	UGameplaySkillBase* TS;

protected:

	// --------------------------------------
	//  Network
	// --------------------------------------

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TriggerSkill(uint8 SkillIndex);
	virtual void Server_TriggerSkill_Implementation(uint8 SkillIndex);
	virtual bool Server_TriggerSkill_Validate(uint8 SkillIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ReleaseSkill(uint8 SkillIndex, float ChargeDuration);
	virtual void Server_ReleaseSkill_Implementation(uint8 SkillIndex, float ChargeDuration);
	virtual bool Server_ReleaseSkill_Validate(uint8 SkillIndex, float ChargeDuration);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCurrentActiveSkill(const FName SkillID);


};

inline bool UGameplaySkillsComponent::IsSkillKeyIndexInvalid(const int32 SkillKeyIndex) const
{
	return (SkillKeyIndex <= 0) || !SBIndexToSGMap.Contains(SkillKeyIndex);
}
