// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "WeaponActorBase.h"
#include "WeaponStatsComponent.h"

const FName AWeaponActorBase::WeaponStatsComponentName(TEXT("WeaponStats"));

AWeaponActorBase::AWeaponActorBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);
	SetReplicateMovement(false);

	WeaponStats = ObjectInitializer.CreateDefaultSubobject<UWeaponStatsComponent>(this, AWeaponActorBase::WeaponStatsComponentName);
}

void AWeaponActorBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

