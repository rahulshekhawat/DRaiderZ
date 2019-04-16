// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PrimaryWeapon.h"
#include "SecondaryWeapon.h"
#include "CharacterLibrary.h"
#include "EODCharacterBase.h"

#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "HumanCharacter.generated.h"


class UAudioComponent;
class USkeletalMeshComponent;

/**
 * 
 */
UCLASS()
class EOD_API AHumanCharacter : public AEODCharacterBase
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	/** Create and initialize skeletal armor mesh, camera, and inventory components. */
	AHumanCharacter(const FObjectInitializer& ObjectInitializer);

	/** Property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Spawn default weapon(s) */
	virtual void PostInitializeComponents() override;

	/** Initializes player animation references. Creates player HUD widget and adds it to the viewport. */
	virtual void BeginPlay() override;

	/** Update character state every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;

	// --------------------------------------
	//  Save/Load System
	// --------------------------------------

	/** Saves current player state */
	virtual void SaveCharacterState() override;

	// --------------------------------------
	//  Components
	// --------------------------------------

	static FName HairComponentName;

	static FName HatItemComponentName;

	static FName FaceItemComponentName;

	static FName ChestComponentName;

	static FName HandsComponentName;

	static FName LegsComponentName;

	static FName FeetComponentName;

	FORCEINLINE USkeletalMeshComponent* GetHair() const { return Hair; }

	FORCEINLINE USkeletalMeshComponent* GetHatItem() const { return HatItem; }

	FORCEINLINE USkeletalMeshComponent* GetFaceItem() const { return FaceItem; }

	FORCEINLINE USkeletalMeshComponent* GetChest() const { return Chest; }
	
	FORCEINLINE USkeletalMeshComponent* GetHands() const { return Hands; }

	FORCEINLINE USkeletalMeshComponent* GetLegs() const { return Legs; }
	
	FORCEINLINE USkeletalMeshComponent* GetFeet() const { return Feet; }

private:

	//~ @note The default skeletal mesh component inherited from ACharacter class will reference the skeletal mesh for player face

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Hair;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* HatItem;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FaceItem;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Chest;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Hands;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Legs;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Feet;

	/** [Constructor Only] A helper function that creates and returns new armor skeletal mesh component */
	USkeletalMeshComponent* CreateNewArmorComponent(const FName Name, const FObjectInitializer& ObjectInitializer);

	inline void SetMasterPoseComponentForMeshes();

public:

	// --------------------------------------
	//  Animations
	// --------------------------------------

	inline FPlayerAnimationReferencesTableRow* GetActiveAnimationReferences() const;

	inline FPlayerAnimationReferencesTableRow* GetEquippedWeaponAnimationReferences() const;

protected:

	inline void AddAnimationSoftObjectPathToArray(const TSoftObjectPtr<UAnimMontage>& MontageSoftPtr, TArray<FSoftObjectPath>& PathArray);

	/** Load animation references for the given weapon type */
	virtual void LoadAnimationReferencesForWeapon(EWeaponType WeaponType);

	/** Unload animation references for the given weapon type */
	virtual void UnloadAnimationReferencesForWeapon(EWeaponType WeaponType);

	TSharedPtr<FStreamableHandle> LoadAnimationReferences(FPlayerAnimationReferencesTableRow* AnimationReferences);

	/** Animation references by weapon type */
	TMap<EWeaponType, FPlayerAnimationReferencesTableRow*> AnimationReferencesMap;

	/** Streamable handle for animation references by weapon type */
	TMap<EWeaponType, TSharedPtr<FStreamableHandle>> AnimationReferencesStreamableHandles;

	FName GetAnimationReferencesRowID(EWeaponType WeaponType, ECharacterGender CharGender);

	/** Data table containing player animation references */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Table")
	UDataTable* PlayerAnimationReferencesDataTable;

public:

	// --------------------------------------
	//  Weapon System
	// --------------------------------------

	/** Returns true if primary weapon is equipped */
	FORCEINLINE bool IsPrimaryWeaponEquippped() const;

	/** Returns true if secondary weapon is equipped */
	FORCEINLINE bool IsSecondaryWeaponEquipped() const;

	/** Returns primary weapon actor */
	FORCEINLINE APrimaryWeapon* GetPrimaryWeapon() const;

	/** Returns secondary weapon actor */
	FORCEINLINE ASecondaryWeapon* GetSecondaryWeapon() const;

	FORCEINLINE bool CanToggleSheathe() const;

	FORCEINLINE bool IsSwitchingWeapon() const;

	/** Returns the weapon type of primary weapon */
	virtual EWeaponType GetEquippedWeaponType() const override;

	/** Replace primary weapon with a new weapon */
	virtual void SetCurrentPrimaryWeapon(const FName WeaponID);

	/** Replace secondary weapon with a new weapon */
	virtual void SetCurrentSecondaryWeapon(const FName WeaponID);

	/** Removes primary weapon if it is currently equipped */
	virtual void RemovePrimaryWeapon();

	/** Removes secondary weapon if it is currently equipped */
	virtual void RemoveSecondaryWeapon();

protected:

	/** An actor for primary weapon equipped by the player */
	UPROPERTY(Transient)
	APrimaryWeapon* PrimaryWeapon;

	/** An actor for secondary weapon equipped by the player */
	UPROPERTY(Transient)
	ASecondaryWeapon* SecondaryWeapon;

	/** ID of the current primary weapon equipped. It will be NAME_None if no primary weapon is equipped */
	UPROPERTY(ReplicatedUsing = OnRep_PrimaryWeaponID, EditDefaultsOnly, BlueprintReadOnly)
	FName PrimaryWeaponID;

	/** ID of current secondary weapon equipped. It will be NAME_None if no secondary weapon is equipped */
	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeaponID, EditDefaultsOnly, BlueprintReadOnly)
	FName SecondaryWeaponID;

public:

	// --------------------------------------
	//  Character States
	// --------------------------------------

	/** Returns true if character can dodge */
	virtual bool CanDodge() const;

	/** Start dodging */
	virtual void StartDodge() override;

	/** Cancel dodging */
	virtual void CancelDodge() override;

	/** Finish dodging */
	virtual void FinishDodge() override;

	/** Returns true if character can use normal attack */
	virtual bool CanNormalAttack() const override;

	/** Start normal attacks */
	virtual void StartNormalAttack() override;

	/** Cancel normal attacks */
	virtual void CancelNormalAttack() override;

	/** Finish normal attacks and reset back to Idle-Walk-Run */
	virtual void FinishNormalAttack() override;

	/** Updates character normal attck state every frame if the character wants to normal attack */
	virtual void UpdateNormalAttackState(float DeltaTime) override;

	/** Get the name of next normal attack section that comes after CurrentSection */
	FName GetNextNormalAttackSectionName(const FName& CurrentSection) const;

	/** Switch between normal attack sections */
	void ChangeNormalAttackSection(FName OldSection, FName NewSection);

	/** Event called when a new normal attack section starts playing */
	void OnNormalAttackSectionStart(FName SectionName);

	/** Set whether normal attack section can change or not  */
	UFUNCTION(BlueprintCallable, Category = "Character State")
	void SetNormalAttackSectionChangeAllowed(bool bNewValue);

protected:

	UPROPERTY(Transient)
	uint32 bNormalAttackSectionChangeAllowed : 1;

public:

	// --------------------------------------
	//  Utility
	// --------------------------------------

	FORCEINLINE ECharacterGender GetCharacterGender() const;

	inline FName GetNormalAttackSectionName(uint8 AttackIndex);

	inline uint8 GetNormalAttackIndex(FName SectionName);

	/** Get the suffix string from the normal attack section */
	inline FString GetNormalAttackSuffix(FName NormalAttackSection) const;


	// --------------------------------------
	//  Input Handling
	// --------------------------------------

	/** Event called when forward key is pressed */
	void OnPressedForward();

	/** Event called when backward key is pressed */
	void OnPressedBackward();

	/** Event called when forward key is released */
	void OnReleasedForward();

	/** Event called when backward key is relased */
	void OnReleasedBackward();

	UPROPERTY(Transient)
	uint32 bForwardPressed : 1;

	UPROPERTY(Transient)
	uint32 bBackwardPressed : 1;

	/** Timer handle needed for executing SP normal attacks */
	FTimerHandle SPAttackTimerHandle;

	UFUNCTION()
	void DisableForwardPressed();

	UFUNCTION()
	void DisableBackwardPressed();

	// --------------------------------------
	//  Materials
	// --------------------------------------

	virtual void TurnOnTargetSwitch() override;

	virtual void TurnOffTargetSwitch() override;

	// --------------------------------------
	//  Sounds
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	FWeaponHitSounds WeaponHitSounds;

protected:

	// --------------------------------------
	//  Network
	// --------------------------------------

	UFUNCTION()
	virtual void OnRep_PrimaryWeaponID();

	UFUNCTION()
	virtual	void OnRep_SecondaryWeaponID();
	
};

inline void AHumanCharacter::SetMasterPoseComponentForMeshes()
{
	if (GetMesh())
	{
		if (Hair)		{ Hair->SetMasterPoseComponent(GetMesh()); }
		if (HatItem)	{ HatItem->SetMasterPoseComponent(GetMesh()); }
		if (FaceItem)	{ FaceItem->SetMasterPoseComponent(GetMesh()); }
		if (Chest)		{ Chest->SetMasterPoseComponent(GetMesh()); }
		if (Hands)		{ Hands->SetMasterPoseComponent(GetMesh()); }
		if (Legs)		{ Legs->SetMasterPoseComponent(GetMesh()); }
		if (Feet)		{ Feet->SetMasterPoseComponent(GetMesh()); }
	}
}

inline FPlayerAnimationReferencesTableRow* AHumanCharacter::GetActiveAnimationReferences() const
{
	EWeaponType WeaponType = IsWeaponSheathed() ? EWeaponType::None : GetEquippedWeaponType();
	if (AnimationReferencesMap.Contains(WeaponType))
	{
		return AnimationReferencesMap[WeaponType];
	}

	return nullptr;
}

inline FPlayerAnimationReferencesTableRow* AHumanCharacter::GetEquippedWeaponAnimationReferences() const
{
	EWeaponType WeaponType = GetEquippedWeaponType();
	if (AnimationReferencesMap.Contains(WeaponType))
	{
		return AnimationReferencesMap[WeaponType];
	}

	return nullptr;
}

inline void AHumanCharacter::AddAnimationSoftObjectPathToArray(const TSoftObjectPtr<UAnimMontage>& MontageSoftPtr, TArray<FSoftObjectPath>& PathArray)
{
	if (MontageSoftPtr.IsPending())
	{
		PathArray.Add(MontageSoftPtr.ToSoftObjectPath());
	}
}

FORCEINLINE bool AHumanCharacter::IsPrimaryWeaponEquippped() const
{
	return PrimaryWeaponID != NAME_None && PrimaryWeapon->IsAttachedToCharacterOwner();
}

FORCEINLINE bool AHumanCharacter::IsSecondaryWeaponEquipped() const
{
	return SecondaryWeaponID != NAME_None && SecondaryWeapon->IsAttachedToCharacterOwner();
}

FORCEINLINE APrimaryWeapon* AHumanCharacter::GetPrimaryWeapon() const
{
	return PrimaryWeapon;
}

FORCEINLINE ASecondaryWeapon* AHumanCharacter::GetSecondaryWeapon() const
{
	return SecondaryWeapon;
}

FORCEINLINE bool AHumanCharacter::CanToggleSheathe() const
{
	return IsIdleOrMoving() && IsPrimaryWeaponEquippped();
}

FORCEINLINE bool AHumanCharacter::IsSwitchingWeapon() const
{
	return CharacterStateInfo.CharacterState == ECharacterState::SwitchingWeapon;
}

FORCEINLINE ECharacterGender AHumanCharacter::GetCharacterGender() const
{
	return Gender;
}

inline FName AHumanCharacter::GetNormalAttackSectionName(uint8 AttackIndex)
{
	if (AttackIndex == 1)
	{
		return UCharacterLibrary::SectionName_FirstSwing;
	}
	else if (AttackIndex == 2)
	{
		return UCharacterLibrary::SectionName_SecondSwing;
	}
	else if (AttackIndex == 3)
	{
		return UCharacterLibrary::SectionName_ThirdSwing;
	}
	else if (AttackIndex == 4)
	{
		return UCharacterLibrary::SectionName_FourthSwing;
	}
	else if (AttackIndex == 5)
	{
		return UCharacterLibrary::SectionName_FifthSwing;
	}
	else if (AttackIndex == 11)
	{
		return UCharacterLibrary::SectionName_ForwardSPSwing;
	}
	else if (AttackIndex == 12)
	{
		return UCharacterLibrary::SectionName_BackwardSPSwing;
	}
	else
	{
		return NAME_None;
	}
}

inline uint8 AHumanCharacter::GetNormalAttackIndex(FName SectionName)
{
	if (SectionName == NAME_None)
	{
		return 0;
	}
	else if (SectionName == UCharacterLibrary::SectionName_FirstSwing)
	{
		return 1;
	}
	else if (SectionName == UCharacterLibrary::SectionName_SecondSwing)
	{
		return 2;
	}
	else if (SectionName == UCharacterLibrary::SectionName_ThirdSwing)
	{
		return 3;
	}
	else if (SectionName == UCharacterLibrary::SectionName_FourthSwing)
	{
		return 4;
	}
	else if (SectionName == UCharacterLibrary::SectionName_FifthSwing)
	{
		return 5;
	}
	else if (SectionName == UCharacterLibrary::SectionName_ForwardSPSwing)
	{
		return 11;
	}
	else if (SectionName == UCharacterLibrary::SectionName_BackwardSPSwing)
	{
		return 12;
	}
	else
	{
		return 0;
	}
}

inline FString AHumanCharacter::GetNormalAttackSuffix(FName NormalAttackSection) const
{
	if (NormalAttackSection == UCharacterLibrary::SectionName_FirstSwing)
	{
		return FString("1");
	}
	else if (NormalAttackSection == UCharacterLibrary::SectionName_SecondSwing)
	{
		return FString("2");
	}
	else if (NormalAttackSection == UCharacterLibrary::SectionName_ThirdSwing)
	{
		return FString("3");
	}
	else if (NormalAttackSection == UCharacterLibrary::SectionName_FourthSwing)
	{
		return FString("4");
	}
	else if (NormalAttackSection == UCharacterLibrary::SectionName_FifthSwing)
	{
		return FString("5");
	}
	else if (NormalAttackSection == UCharacterLibrary::SectionName_BackwardSPSwing)
	{
		return FString("BSP");
	}
	else if (NormalAttackSection == UCharacterLibrary::SectionName_ForwardSPSwing)
	{
		return FString("FSP");
	}
	else
	{
		return FString("");
	}
}
