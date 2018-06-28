// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	GreatSword,
	WarHammer,
	LongSword,
	Mace,
	Dagger,
	Staff,
	Shield
};

UCLASS()
class EOD_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnEquip(ABaseWeapon* LastWeapon);

	virtual void OnUnEquip();
	
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	EWeaponType WeaponType;

	// @todo add status effect class
	// @todo add elemental class

};
