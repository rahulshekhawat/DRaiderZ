// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PrimaryWeapon.h"

APrimaryWeapon::APrimaryWeapon(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	LeftHandedMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Left Hand Weapon"));
	RightHandedMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Right Hand Weapon"));
	SheathedMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Sheathed Weapon"));
	FallenMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Fallen Weapon"));
	
	// No need to setup attachment at creation
}

void APrimaryWeapon::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Primary Weapon begins play"));
}

void APrimaryWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APrimaryWeapon::OnEquip(FWeaponData * NewWeaponData)
{
}

void APrimaryWeapon::OnUnEquip()
{
}
