// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "WeaponBase.h"
#include "EODCharacterBase.h"
#include "WeaponStatsComponent.h"


// Sets default values
AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// This actor doesn't tick
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(false);
	SetReplicateMovement(false);

	bAttachedToCharacter = false;
	WeaponID = NAME_None;
	WeaponType = EWeaponType::None;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	AttachParentCharacter = Cast<AEODCharacterBase>(GetOwner());
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::OnEquip(FName NewWeaponID, FWeaponTableRow* NewWeaponData)
{
}

void AWeaponBase::OnEquip(FName NewWeaponID, UWeaponDataAsset* WeaponDataAsset)
{
}

void AWeaponBase::OnUnEquip()
{
}
