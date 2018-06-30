// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "SecondaryWeapon.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API ASecondaryWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASecondaryWeapon(const FObjectInitializer& ObjectInitializer);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnEquip(FWeaponData* NewWeaponData) override;

	virtual void OnUnEquip() override;
	
	
	
};
