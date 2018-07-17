// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BaseWeapon.h"
#include "Components/WeaponStatsComponent.h"


// Sets default values
ABaseWeapon::ABaseWeapon(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// This actor doesn't tick
	PrimaryActorTick.bCanEverTick = false;
	
	// Setup stats component
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

void ABaseWeapon::SetOwningCharacter(ABaseCharacter * NewCharacter)
{
	OwningCharacter = NewCharacter;
}
