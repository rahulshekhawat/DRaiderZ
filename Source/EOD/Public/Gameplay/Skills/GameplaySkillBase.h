// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLibrary.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "GameplaySkillBase.generated.h"

class AEODCharacterBase;
class AController;
class UGameplaySkillsComponent;

/**
 * The base class for all gameplay skills
 */
UCLASS(BlueprintType, Blueprintable)
class EOD_API UGameplaySkillBase : public UObject
{
	GENERATED_BODY()

public:

	UGameplaySkillBase(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//	Gameplay Skill Interface
	// --------------------------------------

	/** Initialize this skill. Intended to be called immediately after the skill object is created */
	virtual void InitSkill(AEODCharacterBase* Instigator, AController* Owner);

	/** Reinitialize this skill. Intended to be called when the skill owner changes weapons */
	virtual void Reinitialize();

	virtual bool CanTriggerSkill() const;

	/** Trigger this skill, i.e., either instantly activate this skill or start charging this skill. */
	virtual void TriggerSkill();

	virtual bool CanReleaseSkill() const;

	/** Release this skill, i.e., if this skill is chargeable then activate this skill, otherwise it does nothing */
	virtual void ReleaseSkill(float ChargeDuration);

	/** Returns true if this skill can be cancelled */
	virtual bool CanCancelSkill() const;

	/** Cancel this skill */
	virtual void CancelSkill();

	virtual void FinishSkill();

	/** Returns true if this skill is valid, i.e, skill belongs to a valid skill group */
	FORCEINLINE bool IsValid() const { return SkillGroup != NAME_None; }

	FORCEINLINE int32 GetMaxUpgradeLevel() const { return MaxUpgrades; }

	FORCEINLINE UTexture* GetSkillIcon() const { return Icon; }

	FORCEINLINE FString	GetInGameSkillName() const { return InGameName; }

	FORCEINLINE FString	GetInGameDescription() const { return Description; }

	FORCEINLINE FName GetSkillGroup() const { return SkillGroup; }

	FORCEINLINE void SetSkillGroup(FName NewGroup) { SkillGroup = NewGroup; }

	/** Event called when skill owner changes it's weapon */
	virtual void OnOwnerWeaponChange(FName NewWeaponID, FWeaponTableRow* NewWeaponData, FName OldWeaponID, FWeaponTableRow* OldWeaponData);

	/** Returns true if this skill is currently in cooldown */
	inline bool IsSkillInCooldown() const;

	/** Returns true if this skill is currently unlocked on skill tree */
	inline bool IsUnlocked() const { return CurrentUpgrade > 0; }

	// --------------------------------------
	//	Constants : Default values that are not supposed to be modified
	// --------------------------------------

	/** Set this to true if this skill can be charged */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	bool bSkillCanBeCharged;

protected:

	/** Name of this skill that will be displayed to player inside the game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "In-Game Information")
	FString InGameName;

	/** Description of this skill that will be displayed to player inside the game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "In-Game Information")
	FString Description;

	/** Icon used to represent this skill inside the game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "In-Game Information")
	UTexture* Icon;

	/** Maximum upgrades available for this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	int32 MaxUpgrades;

	/** Skill group of this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	FName SkillGroup;

	// --------------------------------------
	//	Gameplay Tags
	// --------------------------------------

	/** The tags used to identify this ability. (example usecase : Cancelling abilities by tags) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer AbilityTags;

	/** The tags that are added to owner while this ability is active */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer ActivationOwnedTags;

	/** The tags that are required on owner to use this ability */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTag RequiredOwnerGameplayTag;

	// --------------------------------------
	//	Cache
	// --------------------------------------

	/** This skill's instigator */
	UPROPERTY(Transient)
	TWeakObjectPtr<AEODCharacterBase> SkillInstigator;

	/** Skill component of this skill's instigator */
	UPROPERTY(Transient)
	TWeakObjectPtr<UGameplaySkillsComponent> InstigatorSkillComponent;

	/** This skill's owner (player controller or ai controller) */
	UPROPERTY(Transient)
	TWeakObjectPtr<AController> SkillOwner;

public:

	// --------------------------------------
	//	Skill State
	// --------------------------------------

	FORCEINLINE int32 GetCurrentUpgrade()const { return CurrentUpgrade; }

	//~ @todo add check to make sure current upgrade is not more than maxupgrade
	void SetCurrentUpgrade(int32 Value) { CurrentUpgrade = Value; }

protected:

	int32 CurrentUpgrade;

	UPROPERTY()
	FTimerHandle CooldownTimerHandle;

	float CooldownRemaining;

	UFUNCTION()
	virtual void StartCooldown();

	UFUNCTION()
	virtual void FinishCooldown();

	UFUNCTION()
	virtual void CancelCooldown();

	UFUNCTION()
	virtual void UpdateCooldown();

};

inline bool UGameplaySkillBase::IsSkillInCooldown() const
{
	return CooldownRemaining > 0.f;
}
