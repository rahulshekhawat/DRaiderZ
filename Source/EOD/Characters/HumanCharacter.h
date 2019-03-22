// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Weapons/PrimaryWeapon.h"
#include "EOD/Statics/CharacterLibrary.h"
#include "EOD/Characters/EODCharacterBase.h"

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
	//	UE4 Method Overrides
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
	//	Save/Load System
	// --------------------------------------

	/** Saves current player state */
	virtual void SaveCharacterState() override;

	// --------------------------------------
	//	Components
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
	//	Animations
	// --------------------------------------

	inline FPlayerAnimationReferencesTableRow* GetActiveAnimationReferences() const;

private:

	/** Animations for this player when it has a weapon equipped */
	FPlayerAnimationReferencesTableRow* EquippedWeaponAnimationReferences;

	/** Animations for this player when it does not have a weapon equipped */
	FPlayerAnimationReferencesTableRow* UnequippedWeaponAnimationReferences;

public:

	// --------------------------------------
	//	Weapon System
	// --------------------------------------

	/** Returns the weapon type of primary weapon */
	FORCEINLINE EWeaponType GetEquippedWeaponType() const;

protected:

	/** An actor for primary weapon equipped by the player */
	UPROPERTY(Transient)
	APrimaryWeapon* PrimaryWeapon;

public:

	// --------------------------------------
	//	Character States
	// --------------------------------------

	/** Returns true if character can dodge */
	virtual bool CanDodge() const;

	virtual void StartDodge() override;

	virtual void FinishDodge() override;

	virtual void EnableCharacterGuard() override;

	virtual void DisableCharacterGuard() override;

private:

	// --------------------------------------
	//	Input Handling
	// --------------------------------------

	void OnPressedForward();

	void OnPressedBackward();

	void OnReleasedForward();

	void OnReleasedBackward();

	bool bForwardPressed;

	bool bBackwardPressed;

	/** Timer handle needed for executing SP normal attacks */
	FTimerHandle SPAttackTimerHandle;

	UFUNCTION()
	void DisableForwardPressed();

	UFUNCTION()
	void DisableBackwardPressed();

public:

	// --------------------------------------
	//	Materials
	// --------------------------------------

	virtual void TurnOnTargetSwitch() override;

	virtual void TurnOffTargetSwitch() override;

private:

	// --------------------------------------
	//	Network
	// --------------------------------------

	
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

EWeaponType AHumanCharacter::GetEquippedWeaponType() const
{
	return PrimaryWeapon ? PrimaryWeapon->GetWeaponType() : EWeaponType::None;
}
