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

	virtual bool CanTriggerSkill() const;

	/** Trigger this skill, i.e., either instantly activate this skill or start charging this skill. */
	virtual void TriggerSkill();

	virtual bool CanReleaseSkill() const;

	/** Release this skill, i.e., if this skill is chargeable then activate this skill, otherwise it does nothing */
	virtual void ReleaseSkill(float ChargeDuration);

	virtual void UpdateSkill(float DeltaTime);

	/** Returns true if this skill can be cancelled */
	virtual bool CanCancelSkill() const;

	/** Cancel this skill */
	virtual void CancelSkill();

	virtual void FinishSkill();

	/** Returns true if this skill is valid, i.e, skill belongs to a valid skill group */
	FORCEINLINE bool IsValid() const { return SkillGroup != NAME_None && SkillIndex != 0; }

	/** Event called when skill owner changes it's weapon */
	virtual void OnOwnerWeaponChange(FName NewWeaponID, FWeaponTableRow* NewWeaponData, FName OldWeaponID, FWeaponTableRow* OldWeaponData);

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	FORCEINLINE EDamageType GetDamageType() const { return DamageType; }

	FORCEINLINE UTexture* GetSkillIcon() const { return Icon; }

	FORCEINLINE FString	GetInGameSkillName() const { return InGameName; }

	FORCEINLINE FString	GetInGameDescription() const { return Description; }

	FORCEINLINE FName GetSkillGroup() const { return SkillGroup; }

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

	/** Skill group of this skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	FName SkillGroup;

	/** Describes the effect of this skill. This will be used by AI for determining most suitable skill to use */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information")
	ESkillEffect SkillEffect;

	/** Type of damage inflicted by this skill if this skill can inflict damage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Information")
	EDamageType DamageType;

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

	FORCEINLINE uint8 GetSkillIndex() const { return SkillIndex; }

	inline void SetSkillIndex(uint8 NewSkillIndex) { SkillIndex = NewSkillIndex; }

protected:

	/** 
	 * This is used to identify skill during replication. SkillIndex(uint8) is cheaper to replicate than SkillID(FName)
	 * This will be set by the gameplay skill component when an object of this class is created.
	 */
	UPROPERTY(Transient)
	uint8 SkillIndex;


};
