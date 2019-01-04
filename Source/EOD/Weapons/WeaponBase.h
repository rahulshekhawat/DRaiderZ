// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/WeaponLibrary.h"

#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class AEODCharacterBase;
class UWeaponDataAsset;
class UWeaponStatsComponent;

/**
 * An abstract base actor class that is responsible for managing and attaching weapon meshes to a character
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


	////////////////////////////////////////////////////////////////////////////////
	// 
	////////////////////////////////////////////////////////////////////////////////
private:
	/** True if this weapon actor is currently attached to the character that owns this actor */
	UPROPERTY(Transient)
	bool bAttachedToCharacter;

	UPROPERTY(Transient)
	FName WeaponID;

	UPROPERTY(Transient)
	EWeaponType WeaponType;

	UPROPERTY(Transient)
	AEODCharacterBase* AttachParentCharacter;

protected:
	FORCEINLINE void SetAttachedToCharacter(bool bNewValue) { bAttachedToCharacter = bNewValue; }

	FORCEINLINE void SetWeaponID(FName ID) { WeaponID = ID; }

	FORCEINLINE void SetWeaponType(EWeaponType NewWeaponType) { WeaponType = NewWeaponType; }

public:
	FORCEINLINE UWeaponStatsComponent* GetWeaponStatsComponent() const { return StatsComp; }

	FORCEINLINE bool IsAttachedToCharacterOwner() const { return bAttachedToCharacter; }

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }


	////////////////////////////////////////////////////////////////////////////////
	// Components
	////////////////////////////////////////////////////////////////////////////////
private:
	/**
	 * Contains the stats information of this weapon
	 * @warning This component may get removed in future iterations of code
	 */
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWeaponStatsComponent* StatsComp;


	////////////////////////////////////////////////////////////////////////////////
	// WEAPON INTERFACE
	////////////////////////////////////////////////////////////////////////////////
public:
	/** Called when a new weapon is attached to it's character owner */
	void OnEquip(FName NewWeaponID);

	/** Called when a new weapon is attached to it's character owner */
	virtual void OnEquip(FName NewWeaponID, FWeaponTableRow* NewWeaponData) { }

	/** Called when a new weapon is attached to it's character owner */
	virtual void OnEquip(FName NewWeaponID, UWeaponDataAsset* WeaponDataAsset) { }

	/** Called to detach this weapon is from it's character owner */
	virtual void OnUnEquip() { }
	

	
};
