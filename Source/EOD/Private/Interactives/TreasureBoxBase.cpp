// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "TreasureBoxBase.h"
#include "EODCharacterBase.h"
#include "EODPlayerController.h"

#include "Components/SkeletalMeshComponent.h"

ATreasureBoxBase::ATreasureBoxBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bSpawnByDefault = true;
	bInteractionInProgress = false;

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
	AEODPlayerController* PC = Character ? Cast<AEODPlayerController>(Character->Controller) : nullptr;
	if (PC == nullptr || PC->IsLocalPlayerController() == false)
	{
		return EInteractionResult::InteractionRequestFailed;
	}

	bool bLootWidgetCreated = PC->CreateLootWidget(GeneratedLootInfoArray, this);
	if (bLootWidgetCreated == false)
	{
		return EInteractionResult::InteractionRequestFailed;
	}

	// Only start looting once the loot widget has been created successfully
	bool bLootStarted = Character->StartLooting();
	if (bLootStarted == false)
	{
		// Remove loot widget if the player couldn't start looting
		PC->RemoveLootWidget(this);
		return EInteractionResult::InteractionRequestFailed;
	}

	OpenBox();
	bInteractionInProgress = true;
	return EInteractionResult::InteractionUpdated;
	
	return EInteractionResult();
}

EInteractionResult ATreasureBoxBase::OnInteractionUpdate_Implementation(AEODCharacterBase* Character)
{
	AEODPlayerController* PC = Character ? Cast<AEODPlayerController>(Character->Controller) : nullptr;
	if (PC == nullptr || PC->IsLocalPlayerController() == false)
	{
		Execute_OnInteractionCancel(this, Character, EInteractionCancelType::AutoCancel);
		return EInteractionResult::InteractionCancelled;
	}

	PC->PickAllLoot();
	Execute_OnInteractionFinish(this, Character);
	
	return EInteractionResult::InteractionFinished;
}

void ATreasureBoxBase::OnInteractionCancel_Implementation(AEODCharacterBase* Character, EInteractionCancelType CancelType)
{
	Execute_OnInteractionFinish(this, Character);
}

void ATreasureBoxBase::OnInteractionFinish_Implementation(AEODCharacterBase* Character)
{
	if (bInteractionInProgress && Character)
	{
		Character->StopLooting();
		AEODPlayerController* PC = Cast<AEODPlayerController>(Character->Controller);
		if (PC)
		{
			PC->RemoveLootWidget(this);
		}
		bInteractionInProgress = false;
	}
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
