// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PrimaryWeapon.h"
#include "Player/PlayerCharacter.h"
#include "Weapons/SecondaryWeapon.h"

#include "Components/SkeletalMeshComponent.h"

APrimaryWeapon::APrimaryWeapon(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// This actor doesn't tick
	PrimaryActorTick.bCanEverTick = false;

	RightHandWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Right Hand Weapon"));
	LeftHandWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Left Hand Weapon"));
	SheathedWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Sheathed Weapon"));
	FallenWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Fallen Weapon"));

	// Weapon's skeletal components do not have any collision. Also, we do not setup attachment for these components at construction.
	RightHandWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	RightHandWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	LeftHandWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	LeftHandWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	SheathedWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	SheathedWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SheathedWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	FallenWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	FallenWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FallenWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	RootComponent = RightHandWeaponMeshComp;
	
	// @todo Render settings
	// RightHandWeaponMeshComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
}

void APrimaryWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APrimaryWeapon::OnEquip(FName NewWeaponID, FWeaponTableRow * NewWeaponData)
{
	check(NewWeaponData && OwningPlayer);

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

	// Do not Activate and SetSkeletalMesh for LeftHandWeaponMeshComp as not all weapon types have LeftHandWeaponMeshComp
	RightHandWeaponMeshComp->Activate();
	SheathedWeaponMeshComp->Activate();
	FallenWeaponMeshComp->Activate();

	RightHandWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);
	SheathedWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);
	FallenWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);

	// Setup attachment for all weapon mesh components as well as also Activate and SetSkeletalMesh for LeftHandWeaponMeshComp wherever applicable
	switch (NewWeaponData->WeaponType)
	{
	case EWeaponType::GreatSword:
		LeftHandWeaponMeshComp->Activate();
		LeftHandWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);

		RightHandWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GS"));
		LeftHandWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GS_c"));
		SheathedWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GS_b"));
		FallenWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GS_w"));
		break;

	case EWeaponType::WarHammer:
		LeftHandWeaponMeshComp->Activate();
		LeftHandWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);

		RightHandWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WH"));
		LeftHandWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WH_c"));
		SheathedWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WH_b"));
		FallenWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WH_w"));
		break;

	case EWeaponType::LongSword:
		RightHandWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LS"));
		SheathedWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LS_b"));
		FallenWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LS_w"));
		break;

	case EWeaponType::Mace:
		RightHandWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("MC"));
		SheathedWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("MC_b"));
		FallenWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("MC_w"));
		break;

	case EWeaponType::Dagger:
		RightHandWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGR"));
		SheathedWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGR_b"));
		FallenWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGR_w"));
		break;

	case EWeaponType::Staff:
		LeftHandWeaponMeshComp->Activate();
		LeftHandWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);

		RightHandWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ST"));
		LeftHandWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ST_c"));
		SheathedWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ST_b"));
		FallenWeaponMeshComp->AttachToComponent(OwningPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ST_w"));
		break;

	default:
		break;
	}

	// Change player animation references if the newer weapon type is different from previous weapon type
	if (NewWeaponData->WeaponType != WeaponType)
	{
		OwningPlayer->UpdateEquippedWeaponAnimationReferences(NewWeaponData->WeaponType);
	}

	OwningPlayer->UpdateEquippedWeaponAnimationReferences(NewWeaponData->WeaponType);

	bEquipped = true;
	WeaponID = NewWeaponID;
	WeaponType = NewWeaponData->WeaponType;

	// @todo intialize weapon stats

}

void APrimaryWeapon::OnUnEquip()
{
	RightHandWeaponMeshComp->SetSkeletalMesh(nullptr);
	LeftHandWeaponMeshComp->SetSkeletalMesh(nullptr);
	SheathedWeaponMeshComp->SetSkeletalMesh(nullptr);
	FallenWeaponMeshComp->SetSkeletalMesh(nullptr);
	
	FDetachmentTransformRules DetachmentRules(FAttachmentTransformRules::KeepRelativeTransform, true);
	RightHandWeaponMeshComp->DetachFromComponent(DetachmentRules);
	LeftHandWeaponMeshComp->DetachFromComponent(DetachmentRules);
	SheathedWeaponMeshComp->DetachFromComponent(DetachmentRules);
	FallenWeaponMeshComp->DetachFromComponent(DetachmentRules);

	RightHandWeaponMeshComp->Deactivate();
	LeftHandWeaponMeshComp->Deactivate();
	SheathedWeaponMeshComp->Deactivate();
	FallenWeaponMeshComp->Deactivate();

	bEquipped = false;
	WeaponID = NAME_None;
	WeaponType = EWeaponType::None;
	// @todo reset weapon stats

	// If owning player doesn't have a dagger equipped as secondary weapon then update player animation references to 'no weapon equipped'
	if (OwningPlayer && OwningPlayer->GetSecondaryWeapon() && OwningPlayer->GetSecondaryWeapon()->WeaponType != EWeaponType::Dagger)
	{
		OwningPlayer->UpdateEquippedWeaponAnimationReferences(EWeaponType::None);
	}

}
