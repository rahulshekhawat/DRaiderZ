// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/WeaponLibrary.h"

#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class APlayerCharacter;
class UWeaponDataAsset;

/**
 * An abstract base class for player equippable weapons
 */
UCLASS(Abstract)
class EOD_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	

	/** Create the default stats component */
	AWeaponBase(const FObjectInitializer& ObjectInitializer);

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;
	
	/** Called when a new weapon is equipped by a character */
	void OnEquip(FName NewWeaponID);

	/** Called when a new weapon is equipped by a character */
	virtual void OnEquip(FName NewWeaponID, FWeaponTableRow* NewWeaponData) PURE_VIRTUAL(AWeaponBase::OnEquip, );

	virtual void OnEquip(FName NewWeaponID, UWeaponDataAsset* WeaponDataAsset) PURE_VIRTUAL(AWeaponBase::OnEquip, );

	/** Called when this weapon is unequipped by a character */
	virtual void OnUnEquip() PURE_VIRTUAL(ABaseWeapon::OnUnEquip, );
	
	/** Weapon type of equipped weapon */
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	EWeaponType WeaponType;

	bool bEquipped;

	FName WeaponID;

private:

	/**
	 * Contains the stats information of this weapon
	 * @warning This component may get removed in future iterations of code
	 */
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWeaponStatsComponent* StatsComp;
	
	
};
