// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "PrimaryWeapon.generated.h"


/**
 * A PrimaryWeapon is a either a dual handed weapon or a single handed weapon that can be equipped in right hand
 */
UCLASS()
class EOD_API APrimaryWeapon : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	/** Constructor is used to create this weapon's default skeletal mesh component objects */
	APrimaryWeapon(const FObjectInitializer& ObjectInitializer);


	////////////////////////////////////////////////////////////////////////////////
	// Components
	////////////////////////////////////////////////////////////////////////////////
private:
	/** Mesh component that represents the mesh for weapon equipped in left hand */
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LeftHandWeaponMeshComp;
		
	/** Mesh component that represents the mesh for weapon equipped in right hand */
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RightHandWeaponMeshComp;
		
	/** Mesh component that represents the mesh for the weapon fallen on ground */
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

	/** Called when a new weapon is attached to it's character owner */
	virtual void OnEquip(FName NewWeaponID, UWeaponDataAsset* WeaponDataAsset) override;

	/** Called to detach this weapon is from it's character owner */
	virtual void OnUnEquip() override;

	
};
