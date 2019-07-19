// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "TreasureBoxBase.h"
#include "EODCharacterBase.h"

#include "Components/SkeletalMeshComponent.h"

ATreasureBoxBase::ATreasureBoxBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSpawnByDefault = true;

	if (PrimaryMesh)
	{
		PrimaryMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	}
}

void ATreasureBoxBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (bSpawnByDefault)
	{
		PrimaryMesh->PlayAnimation(VisibleIdleAnimation, false);
	}
	else
	{
		PrimaryMesh->PlayAnimation(HiddenIdleAnimation, false);
	}
}

void ATreasureBoxBase::BeginPlay()
{
	Super::BeginPlay();

	GenerateLootInfoArray();
}

EInteractionResult ATreasureBoxBase::OnInteractionStart_Implementation(AEODCharacterBase* Character)
{
	//~ @todo
	/*
	// Ignore interaction request if it's not the local player interacting with us
	if (!Character->Controller && !Character->Controller->IsLocalPlayerController())
	{
		return;
	}

	OpenBox();

	//~ @todo check for loot, play player loot animation if there's loot, etc.
	*/
	return EInteractionResult();
}

EInteractionResult ATreasureBoxBase::OnInteractionUpdate_Implementation(AEODCharacterBase* Character)
{
	return EInteractionResult();
}

void ATreasureBoxBase::OnInteractionCancel_Implementation(AEODCharacterBase* Character, EInteractionCancelType CancelType)
{
}

void ATreasureBoxBase::GenerateLootInfoArray()
{
	for (const FStoredLootInfo& StoredLootInfo : StoredLootInfoArray)
	{
		bool bShouldGenerate = StoredLootInfo.DropChance >= FMath::RandRange(0.f, 100.f) ? true : false;

		if (bShouldGenerate)
		{
			FGeneratedLootInfo GeneratedLootInfo;
			GeneratedLootInfo.ItemCount = FMath::RandRange(1, FMath::Max(1, StoredLootInfo.MaxCount));
			GeneratedLootInfo.ItemClass = StoredLootInfo.ItemClass;
			GeneratedLootInfoArray.Add(GeneratedLootInfo);
		}
	}
}

TArray<FStoredLootInfo> ATreasureBoxBase::GetStoredLootInfo_Implementation() const
{
	return StoredLootInfoArray;
}

TArray<FGeneratedLootInfo> ATreasureBoxBase::GetGeneratedLootInfo_Implementation() const
{
	return GeneratedLootInfoArray;
}

int32 ATreasureBoxBase::AcquireLootItem_Implementation(TSubclassOf<UObject> LootItemClass, AEODCharacterBase* Looter)
{
	for (const FGeneratedLootInfo& GeneratedLootInfo : GeneratedLootInfoArray)
	{
		if (GeneratedLootInfo.ItemClass == LootItemClass)
		{
			return GeneratedLootInfo.ItemCount;
		}
	}

	return 0;
}

void ATreasureBoxBase::SpawnBox()
{
	if (PrimaryMesh)
	{
		PrimaryMesh->PlayAnimation(ChestDropAnimation, false);
	}
}

void ATreasureBoxBase::OpenBox()
{
	// If the box is already open
	if (bOpen)
	{
		return;
	}

	if (PrimaryMesh)
	{
		PrimaryMesh->PlayAnimation(ChestOpenAnimation, false);
	}
	bOpen = true;
}

bool ATreasureBoxBase::BP_IsOpen() const
{
	return bOpen;
}

bool ATreasureBoxBase::HasLoot() const
{
	// If generated loot info array still contains an item
	return GeneratedLootInfoArray.Num() > 0;
}
