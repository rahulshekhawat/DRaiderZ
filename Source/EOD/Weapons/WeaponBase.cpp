// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "WeaponBase.h"
#include "Components/WeaponStatsComponent.h"


// Sets default values
AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// This actor doesn't tick
	PrimaryActorTick.bCanEverTick = false;
	
	// Setup stats component
	StatsComp = ObjectInitializer.CreateDefaultSubobject<UWeaponStatsComponent>(this, FName("Weapon Stats Component"));
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::SetOwningCharacter(AEODCharacterBase * NewCharacter)
{
	OwningCharacter = NewCharacter;
}
