// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
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
	
	/** Dummy declaration. This actor doesn't tick */
	virtual void Tick(float DeltaTime) override;
	
	/** Called when this weapon is equipped by a character */
	virtual void OnEquip(FWeaponData* NewWeaponData) override;

	/** Called when this weapon is unequipped by a character */
	virtual void OnUnEquip() override;
	
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
	
	
};
