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
	/** Constructor is used to create this weapon's default skeletal mesh component objects */
	ASecondaryWeapon(const FObjectInitializer& ObjectInitializer);


	////////////////////////////////////////////////////////////////////////////////
	// Components
	////////////////////////////////////////////////////////////////////////////////
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


	////////////////////////////////////////////////////////////////////////////////
	// WEAPON INTERFACE
	////////////////////////////////////////////////////////////////////////////////
public:
	/** Called when a new weapon is attached to it's character owner */
	virtual void OnEquip(FName NewWeaponID, FWeaponTableRow* NewWeaponData) override;

	/** Called to detach this weapon is from it's character owner */
	virtual void OnUnEquip() override;
	
};
