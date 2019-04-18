// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponLibrary.h"

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

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	/** Create the default stats component */
	AWeaponBase(const FObjectInitializer& ObjectInitializer);

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// --------------------------------------
	//  Weapon Interface
	// --------------------------------------

	/** Called when a new weapon is attached to it's character owner */
	virtual void OnEquip(FName NewWeaponID, FWeaponTableRow* NewWeaponData = nullptr);

	/** Called when a new weapon is attached to it's character owner */
	virtual void OnEquip(FName NewWeaponID, UWeaponDataAsset* WeaponDataAsset = nullptr);

	/** Called to detach this weapon is from it's character owner */
	virtual void OnUnEquip();

	FORCEINLINE bool IsAttachedToCharacterOwner() const { return bAttachedToCharacter; }

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

protected:

	FORCEINLINE void SetAttachedToCharacter(bool bNewValue) { bAttachedToCharacter = bNewValue; }

	FORCEINLINE void SetWeaponID(FName ID) { WeaponID = ID; }

	FORCEINLINE void SetWeaponType(EWeaponType NewWeaponType) { WeaponType = NewWeaponType; }

private:

	/** True if this weapon actor is currently attached to the character that owns this actor */
	UPROPERTY()
	bool bAttachedToCharacter;

	UPROPERTY()
	FName WeaponID;

	UPROPERTY()
	EWeaponType WeaponType;

	UPROPERTY(Transient)
	AEODCharacterBase* AttachParentCharacter;

};
