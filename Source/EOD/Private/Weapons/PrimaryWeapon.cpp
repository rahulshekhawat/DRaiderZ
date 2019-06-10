// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "PrimaryWeapon.h"
#include "PlayerCharacter.h"
#include "SecondaryWeapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"


const FName APrimaryWeapon::LeftHandWeaponMeshCompName(TEXT("Left Hand Weapon"));
const FName APrimaryWeapon::RightHandWeaponMeshCompName(TEXT("Right Hand Weapon"));
const FName APrimaryWeapon::FallenWeaponMeshCompName(TEXT("Fallen Weapon"));
const FName APrimaryWeapon::SheathedWeaponMeshCompName(TEXT("Sheathed Weapon"));

APrimaryWeapon::APrimaryWeapon(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// This actor doesn't tick
	PrimaryActorTick.bCanEverTick = false;

	//~ @note Weapon's skeletal components do not have any collision. Also, we do not setup attachment for these components at construction.
	RightHandWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, APrimaryWeapon::RightHandWeaponMeshCompName);
	if (RightHandWeaponMeshComp)
	{
		RootComponent = RightHandWeaponMeshComp;

		RightHandWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
		RightHandWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RightHandWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		RightHandWeaponMeshComp->SetCanEverAffectNavigation(false);
		RightHandWeaponMeshComp->SetReceivesDecals(false);
		RightHandWeaponMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	}

	LeftHandWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, APrimaryWeapon::LeftHandWeaponMeshCompName);
	if (LeftHandWeaponMeshComp)
	{
		LeftHandWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
		LeftHandWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		LeftHandWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		LeftHandWeaponMeshComp->SetCanEverAffectNavigation(false);
		LeftHandWeaponMeshComp->SetReceivesDecals(false);
		LeftHandWeaponMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	}

	SheathedWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, APrimaryWeapon::SheathedWeaponMeshCompName);
	if (SheathedWeaponMeshComp)
	{
		SheathedWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
		SheathedWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SheathedWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		SheathedWeaponMeshComp->SetCanEverAffectNavigation(false);
		SheathedWeaponMeshComp->SetReceivesDecals(false);
		SheathedWeaponMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	}

	FallenWeaponMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, APrimaryWeapon::FallenWeaponMeshCompName);
	{
		FallenWeaponMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
		FallenWeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FallenWeaponMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		FallenWeaponMeshComp->SetCanEverAffectNavigation(false);
		FallenWeaponMeshComp->SetReceivesDecals(false);
		FallenWeaponMeshComp->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	}
}

void APrimaryWeapon::OnEquip(FName NewWeaponID, FWeaponTableRow* NewWeaponData)
{
	APlayerCharacter* PlayerOwner = Cast<APlayerCharacter>(GetOwner());
	if (!NewWeaponData ||
		!IsValid(PlayerOwner) ||
		!IsValid(LeftHandWeaponMeshComp) ||
		!IsValid(RightHandWeaponMeshComp) ||
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

	// Do not Activate and SetSkeletalMesh for LeftHandWeaponMeshComp as not all primary weapon types have LeftHandWeaponMeshComp
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

		RightHandWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GS"));
		LeftHandWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GS_c"));
		SheathedWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GS_b"));
		FallenWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GS_w"));
		break;

	case EWeaponType::WarHammer:
		LeftHandWeaponMeshComp->Activate();
		LeftHandWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);

		RightHandWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WH"));
		LeftHandWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WH_c"));
		SheathedWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WH_b"));
		FallenWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WH_w"));
		break;

	case EWeaponType::LongSword:
		RightHandWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LS"));
		SheathedWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LS_b"));
		FallenWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LS_w"));
		break;

	case EWeaponType::Mace:
		RightHandWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("MC"));
		SheathedWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("MC_b"));
		FallenWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("MC_w"));
		break;

	case EWeaponType::Dagger:
		RightHandWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGR"));
		SheathedWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGR_b"));
		FallenWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("DGR_w"));
		break;

	case EWeaponType::Staff:
		LeftHandWeaponMeshComp->Activate();
		LeftHandWeaponMeshComp->SetSkeletalMesh(NewSkeletalMesh);

		RightHandWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ST"));
		LeftHandWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ST_c"));
		SheathedWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ST_b"));
		FallenWeaponMeshComp->AttachToComponent(PlayerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ST_w"));
		break;

	default:
		break;
	}

	SetAttachedToCharacter(true);
	SetWeaponID(NewWeaponID);
	SetWeaponType(NewWeaponData->WeaponType);

	// @todo intialize weapon stats

}

void APrimaryWeapon::OnUnEquip()
{
	FDetachmentTransformRules DetachmentRules(FAttachmentTransformRules::KeepRelativeTransform, true);

	if (LeftHandWeaponMeshComp)
	{
		LeftHandWeaponMeshComp->SetSkeletalMesh(nullptr);
		LeftHandWeaponMeshComp->DetachFromComponent(DetachmentRules);
		LeftHandWeaponMeshComp->Deactivate();
	}

	if (RightHandWeaponMeshComp)
	{
		RightHandWeaponMeshComp->SetSkeletalMesh(nullptr);
		RightHandWeaponMeshComp->DetachFromComponent(DetachmentRules);
		RightHandWeaponMeshComp->Deactivate();
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
