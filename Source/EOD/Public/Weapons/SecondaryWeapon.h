// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "SecondaryWeapon.generated.h"

/**
 * A SecondaryWeapon is a single handed weapon that can be equipped in left hand
 */
UCLASS()
class EOD_API ASecondaryWeapon : public AWeaponBase
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	/** Constructor is used to create this weapon's default skeletal mesh component objects */
	ASecondaryWeapon(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//  Components
	// --------------------------------------

	FORCEINLINE USkeletalMeshComponent* GetLeftHandWeaponMeshComp() const { return LeftHandWeaponMeshComp; }

	FORCEINLINE USkeletalMeshComponent* GetFallenWeaponMeshComp() const { return FallenWeaponMeshComp; }

	FORCEINLINE USkeletalMeshComponent* GetSheathedWeaponMeshComp() const { return SheathedWeaponMeshComp; }

	static const FName LeftHandWeaponMeshCompName;

	static const FName FallenWeaponMeshCompName;

	static const FName SheathedWeaponMeshCompName;

private:

	/** Mesh component that represents the mesh for weapon equipped in left hand */
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LeftHandWeaponMeshComp;
		
	/** Mesh component that represents the mesh for the weapon that has fallen on ground */
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FallenWeaponMeshComp;
	
	/** Mesh component that represents the mesh for sheathed weapon */
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SheathedWeaponMeshComp;


public:

	// --------------------------------------
	//  Weapon Interface
	// --------------------------------------

	/** Called when a new weapon is attached to it's character owner */
	virtual void OnEquip(FName NewWeaponID, FWeaponTableRow* NewWeaponData = nullptr) override;

	/** Called to detach this weapon is from it's character owner */
	virtual void OnUnEquip() override;
	
};
