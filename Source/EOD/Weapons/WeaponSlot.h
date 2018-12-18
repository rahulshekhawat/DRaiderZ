// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponSlot.generated.h"

class AWeaponBase;
class APrimaryWeapon;
class ASecondaryWeapon;
class UWeaponDataAsset;


/**
 * 
 */
UCLASS()
class EOD_API UWeaponSlot : public UObject
{
	GENERATED_BODY()
	
public:
	UWeaponSlot(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitProperties() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon Slot")
	AWeaponBase* PrimaryWeapon;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon Slot")
	AWeaponBase* SecondaryWeapon;

public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon Slot")
	UWeaponDataAsset* PrimaryWeaponDataAsset;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon Slot")
	UWeaponDataAsset* SecondaryWeaponDataAsset;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon Slot")
	FName PrimaryWeaponID;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon Slot")
	FName SecondaryWeaponID;

public:
	void AttachPrimaryWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset);

	void AttachPrimaryWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset, AWeaponBase* PrimaryWep);

	void AttachPrimaryWeapon(AWeaponBase* PrimaryWep);

	void AttachSecondaryWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset);

	void DetachPrimaryWeapon();

	void DetachSecondaryWeapon();

};
