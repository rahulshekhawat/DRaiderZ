// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/WeaponLibrary.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class AEODCharacterBase;

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

protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:	

	/** Dummy declaration. Weapon actors are not supposed to tick */
	virtual void Tick(float DeltaTime) override;
	
	/** Called when a new weapon is equipped by a character */
	void OnEquip(FName NewWeaponID);

	/** Called when a new weapon is equipped by a character */
	virtual void OnEquip(FName NewWeaponID, FWeaponTableRow* NewWeaponData) PURE_VIRTUAL(AWeaponBase::OnEquip, );

	/** Called when this weapon is unequipped by a character */
	virtual void OnUnEquip() PURE_VIRTUAL(ABaseWeapon::OnUnEquip, );
	
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	EWeaponType WeaponType;

	bool bEquipped;

	FName WeaponID;

	/** 
	 * Call this to set the owner of this weapon.
	 * @note It is important to set owner before attempting to equip weapon.
	 */
	void SetOwningCharacter(AEODCharacterBase* NewCharacter);

protected:
	
	//~ @todo network replication
	AEODCharacterBase* OwningCharacter;

private:

	/**
	 * Contains the stats information of this weapon
	 * @warning This component may get removed in future iterations of code
	 */
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWeaponStatsComponent* StatsComp;
	
	
};
