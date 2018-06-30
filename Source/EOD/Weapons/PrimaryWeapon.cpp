// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PrimaryWeapon.h"
#include "Player/BaseCharacter.h"

#include "Components/SkeletalMeshComponent.h"

APrimaryWeapon::APrimaryWeapon(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	RightHandWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Right Hand Weapon"));
	LeftHandWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Left Hand Weapon"));
	SheathedWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Sheathed Weapon"));
	FallenWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Fallen Weapon"));
	
	RootComponent = RightHandWeaponMeshComp;
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
	check(NewWeaponData);
	check(OwningCharacter);

	USkeletalMesh* NewSkeletalMesh = NewWeaponData->WeaponMesh.LoadSynchronous();
	RightHandWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);
	LeftHandWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);
	SheathedWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);
	FallenWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);

	RightHandWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GS"));
	LeftHandWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GS_c"));
	SheathedWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GS_b"));
	FallenWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GS_w"));
	
	UE_LOG(LogTemp, Warning, TEXT("Equipped"));
}

void APrimaryWeapon::OnUnEquip()
{
}
