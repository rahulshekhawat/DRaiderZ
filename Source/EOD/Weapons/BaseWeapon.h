// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/WeaponLibrary.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class ABaseCharacter;

/**
 * An abstract base class for player equippable weapons
 */
UCLASS(Abstract)
class EOD_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	/** Create the default stats component */
	ABaseWeapon(const FObjectInitializer& ObjectInitializer);

protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:	

	/** Dummy declaration. Weapon actors are not supposed to tick */
	virtual void Tick(float DeltaTime) override;
	
	/** Called when this weapon is equipped by a character */
	virtual void OnEquip(FWeaponData* NewWeaponData) PURE_VIRTUAL(ABaseWeapon::OnEquip, );

	/** Called when this weapon is unequipped by a character */
	virtual void OnUnEquip() PURE_VIRTUAL(ABaseWeapon::OnUnEquip, );
	
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	EWeaponType WeaponType;

	/** Call this to set the owner of this weapon.
	 * @note It is important to set owner before attempting to equip weapon.
	 */
	void SetOwningCharacter(ABaseCharacter* NewCharacter);

protected:
	
	//~ @todo network replication
	ABaseCharacter* OwningCharacter;

private:

	/**
	 * Contains the stats information of this weapon
	 * @warning This component may get removed in future iterations of code
	 */
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWeaponStatsComponent* StatsComp;


};
