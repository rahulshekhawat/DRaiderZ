// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SecondaryWeapon.h"
#include "Player/BaseCharacter.h"

#include "Components/SkeletalMeshComponent.h"

ASecondaryWeapon::ASecondaryWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// This actor doesn't tick
	PrimaryActorTick.bCanEverTick = false;

	LeftHandWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Left Hand Weapon"));
	SheathedWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Sheathed Weapon"));
	FallenWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Fallen Weapon"));

	// Weapon's skeletal components do not have any collision. Also, we do not setup attachment for these components at construction.
	LeftHandWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	LeftHandWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	SheathedWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	SheathedWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SheathedWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	FallenWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	FallenWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FallenWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	RootComponent = LeftHandWeaponMeshComp;

	// @todo Render settings
	// RightHandWeaponMeshComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
}

void ASecondaryWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASecondaryWeapon::OnEquip(FWeaponData * NewWeaponData)
{
	check(NewWeaponData);
	check(OwningCharacter);
	
	if (NewWeaponData->WeaponMesh.IsNull())
	{
		// @todo handle failed to equip
		return;
	}

	USkeletalMesh* NewSkeletalMesh = nullptr;
	
	if (NewWeaponData->WeaponMesh.IsPending())
	{
		NewSkeletalMesh = NewWeaponData->WeaponMesh.LoadSynchronous();
	}
	else if (NewWeaponData->WeaponMesh.IsValid())
	{
		NewSkeletalMesh = NewWeaponData->WeaponMesh.Get();
	}

	LeftHandWeaponMeshComp->Activate();
	SheathedWeaponMeshComp->Activate();
	FallenWeaponMeshComp->Activate();

	LeftHandWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);
	SheathedWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);
	FallenWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);

	if (NewWeaponData->WeaponType == EWeaponType::Dagger)
	{
		LeftHandWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGL"));
		SheathedWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGL_b"));
		FallenWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGL_w"));
	}
	else if (NewWeaponData->WeaponType == EWeaponType::Shield)
	{
		LeftHandWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SLD"));
		SheathedWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SLD_b"));
		FallenWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SLD_w"));
	}
	else
	{
		// pass
	}
}

void ASecondaryWeapon::OnUnEquip()
{
	LeftHandWeaponMeshComp->SetSkeletalMesh(nullptr);
	SheathedWeaponMeshComp->SetSkeletalMesh(nullptr);
	FallenWeaponMeshComp->SetSkeletalMesh(nullptr);
	
	FDetachmentTransformRules DetachmentRules(FAttachmentTransformRules::KeepRelativeTransform, true);
	LeftHandWeaponMeshComp->DetachFromComponent(DetachmentRules);
	SheathedWeaponMeshComp->DetachFromComponent(DetachmentRules);
	FallenWeaponMeshComp->DetachFromComponent(DetachmentRules);

	LeftHandWeaponMeshComp->Deactivate();
	SheathedWeaponMeshComp->Deactivate();
	FallenWeaponMeshComp->Deactivate();
}
