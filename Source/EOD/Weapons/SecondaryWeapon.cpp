// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SecondaryWeapon.h"
#include "Player/BaseCharacter.h"

#include "Components/SkeletalMeshComponent.h"

ASecondaryWeapon::ASecondaryWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	LeftHandWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Left Hand Weapon"));
	SheathedWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Sheathed Weapon"));
	FallenWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Fallen Weapon"));

	// RightHandWeaponMeshComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	
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
	// No need to setup attachment at creation
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

	switch (NewWeaponData->WeaponType)
	{
	case EWeaponType::GreatSword:
		break;
	case EWeaponType::WarHammer:
		break;
	case EWeaponType::LongSword:
		break;
	case EWeaponType::Mace:
		break;
	case EWeaponType::Staff:
		break;
	case EWeaponType::Dagger:
		LeftHandWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGL"));
		SheathedWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGL_b"));
		FallenWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGL_w"));
		break;
	case EWeaponType::Shield:
		LeftHandWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SLD"));
		SheathedWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SLD_b"));
		FallenWeaponMeshComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SLD_w"));
		break;
	default:
		break;
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
