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

protected:

	virtual void UnloadUnequippedWeaponAnimationReferences();

	virtual void LoadUnequippedWeaponAnimationReferences();

	virtual void UnloadEquippedWeaponAnimationReferences();

	virtual void LoadEquippedWeaponAnimationReferences();

	FName GetAnimationReferencesRowID(EWeaponType WeaponType, ECharacterGender CharGender);

	TSharedPtr<FStreamableHandle> LoadAnimationReferences(FPlayerAnimationReferencesTableRow* AnimationReferences);

	/** Data table containing player animation references */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UDataTable * PlayerAnimationReferencesDataTable;

	TSharedPtr<FStreamableHandle> EquippedWeaponAnimationsStreamableHandle;

	TSharedPtr<FStreamableHandle> UnequippedWeaponAnimationsStreamableHandle;

	/** Animations for this player when it has a weapon equipped */
	FPlayerAnimationReferencesTableRow* EquippedWeaponAnimationReferences;

	/** Animations for this player when it does not have a weapon equipped */
	FPlayerAnimationReferencesTableRow* UnequippedWeaponAnimationReferences;

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

	FORCEINLINE bool CanSwitchWeapon() const;

	FORCEINLINE bool CanToggleSheathe() const;

	FORCEINLINE bool IsSwitchingWeapon() const;

	/** Returns the weapon type of primary weapon */
	virtual EWeaponType GetEquippedWeaponType() const override;

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

	/** Finish dodging */
	virtual void FinishDodge() override;

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
	if (IsWeaponSheathed() || GetEquippedWeaponType() == EWeaponType::None)
	{
		return UnequippedWeaponAnimationReferences;
	}
	return EquippedWeaponAnimationReferences;
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

FORCEINLINE bool AHumanCharacter::CanSwitchWeapon() const
{
	return IsIdleOrMoving();
}

FORCEINLINE bool AHumanCharacter::CanToggleSheathe() const
{
	return IsIdleOrMoving() && IsPrimaryWeaponEquippped();
}

FORCEINLINE bool AHumanCharacter::IsSwitchingWeapon() const
{
	return CharacterStateInfo.CharacterState == ECharacterState::SwitchingWeapon;
}
