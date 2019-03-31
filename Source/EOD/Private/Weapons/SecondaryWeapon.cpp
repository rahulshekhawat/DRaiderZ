// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SecondaryWeapon.h"
#include "PlayerCharacter.h"
#include "PrimaryWeapon.h"

#include "Components/SkeletalMeshComponent.h"

ASecondaryWeapon::ASecondaryWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// This actor doesn't tick
	PrimaryActorTick.bCanEverTick = false;

	//~ @note Weapon's skeletal components do not have any collision. Also, we do not setup attachment for these components at construction.
	LeftHandWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Left Hand Weapon"));
	if (LeftHandWeaponMeshComp)
	{
		RootComponent = LeftHandWeaponMeshComp;

		LeftHandWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
		LeftHandWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		LeftHandWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		LeftHandWeaponMeshComp->SetCanEverAffectNavigation(false);
		LeftHandWeaponMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	}

	SheathedWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Sheathed Weapon"));
	if (SheathedWeaponMeshComp)
	{
		SheathedWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
		SheathedWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SheathedWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		SheathedWeaponMeshComp->SetCanEverAffectNavigation(false);
		SheathedWeaponMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	}

	FallenWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Fallen Weapon"));
	{
		FallenWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
		FallenWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FallenWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		FallenWeaponMeshComp->SetCanEverAffectNavigation(false);
		FallenWeaponMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	}
}

void ASecondaryWeapon::OnEquip(FName NewWeaponID, FWeaponTableRow* NewWeaponData)
{
	APlayerCharacter* PlayerOwner = Cast<APlayerCharacter>(GetOwner());
	if (!NewWeaponData ||
		!IsValid(PlayerOwner) ||
		!IsValid(LeftHandWeaponMeshComp) ||
		!IsValid(SheathedWeaponMeshComp) ||
		!IsValid(FallenWeaponMeshComp))
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

	if (!NewSkeletalMesh)
	{
		// @todo handle missing weapon mesh
		return;
	}

	LeftHandWeaponMeshComp->Activate();
	SheathedWeaponMeshComp->Activate();
	FallenWeaponMeshComp->Activate();

	LeftHandWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);
	SheathedWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);
	FallenWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);

	if (NewWeaponData->WeaponType == EWeaponType::Dagger)
	{
		LeftHandWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGL"));
		SheathedWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGL_b"));
		FallenWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGL_w"));
	}
	else if (NewWeaponData->WeaponType == EWeaponType::Shield)
	{
		LeftHandWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SLD"));
		SheathedWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SLD_b"));
		FallenWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SLD_w"));
	}

	SetAttachedToCharacter(true);
	SetWeaponID(NewWeaponID);
	SetWeaponType(NewWeaponData->WeaponType);

	// @todo intialize weapon stats
}

void ASecondaryWeapon::OnUnEquip()
{
	FDetachmentTransformRules DetachmentRules(FAttachmentTransformRules::KeepRelativeTransform, true);

	if (LeftHandWeaponMeshComp)
	{
		LeftHandWeaponMeshComp->SetSkeletalMesh(nullptr);
		LeftHandWeaponMeshComp->DetachFromComponent(DetachmentRules);
		LeftHandWeaponMeshComp->Deactivate();
	}

	if (SheathedWeaponMeshComp)
	{
		SheathedWeaponMeshComp->SetSkeletalMesh(nullptr);
		SheathedWeaponMeshComp->DetachFromComponent(DetachmentRules);
		SheathedWeaponMeshComp->Deactivate();
	}

	if (FallenWeaponMeshComp)
	{
		FallenWeaponMeshComp->SetSkeletalMesh(nullptr);
		FallenWeaponMeshComp->DetachFromComponent(DetachmentRules);
		FallenWeaponMeshComp->Deactivate();
	}

	SetAttachedToCharacter(false);
	SetWeaponID(NAME_None);
	SetWeaponType(EWeaponType::None);

	// @todo reset weapon stats

}
