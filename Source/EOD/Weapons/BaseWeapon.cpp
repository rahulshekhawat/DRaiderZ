// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BaseWeapon.h"
#include "Components/WeaponStatsComponent.h"


// Sets default values
ABaseWeapon::ABaseWeapon(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
		StatsComp = ObjectInitializer.CreateDefaultSubobject<UWeaponStatsComponent>(this, FName("Weapon Stats Component"));

}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
void ABaseWeapon::OnEquip(FWeaponData * NewWeaponData)
{
}

void ABaseWeapon::OnUnEquip()
{
}
*/
