// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "PrimaryWeapon.generated.h"

class USkeletalMeshComponent;

/**
 Determines whether primary weapon is equipped in single hand or both hands
 */
UENUM(BlueprintType)
enum class EPrimaryWeaponType : uint8
{
	SingleHanded,
	DualHanded
};

/**
 * A PrimaryWeapon is a either a dual handed weapon or a single handed weapon that can be equipped in right hand
 */
UCLASS()
class EOD_API APrimaryWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:

	/** Constructor is used to create this weapon's default skeletal mesh component objects */
	APrimaryWeapon(const FObjectInitializer& ObjectInitializer);
	
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
		
	/** Mesh component that represents the mesh for weapon equipped in right hand */
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RightHandWeaponMeshComp;
		
	/** Mesh component that represents the mesh for the weapon that has fallen on ground */
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FallenWeaponMeshComp;
	
	/** Mesh component that represents the mesh for sheathed weapon */
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SheathedWeaponMeshComp;

	
};
