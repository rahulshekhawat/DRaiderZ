// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "TreasureBoxBase.h"
#include "EODCharacterBase.h"
#include "EODPlayerController.h"
#include "InventoryComponent.h"

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
	if (PC == nullptr || PC->IsLocalPlayerController() == false || GeneratedLootInfoArray.Num() == 0)
	{
		return EInteractionResult::InteractionRequestFailed;
	}

	bool bLootWidgetCreated = PC->CreateLootWidget(this);
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

			if (GeneratedLootInfoArray.Num() == 0)
			{
				PC->UnregisterPopupWidget(this);
				Execute_DisableCustomDepth(this);
			}
		}
		bInteractionInProgress = false;
	}
}

void ATreasureBoxBase::EnableCustomDepth_Implementation()
{
	if (GeneratedLootInfoArray.Num() == 0)
	{
		return;
	}

	if (PrimaryMesh)
	{
		PrimaryMesh->SetRenderCustomDepth(true);
	}
}

void ATreasureBoxBase::DisableCustomDepth_Implementation()
{
	if (PrimaryMesh)
	{
		PrimaryMesh->SetRenderCustomDepth(false);
	}
}

void ATreasureBoxBase::OnGainFocus_Implementation(AEODCharacterBase* Character)
{
	if (GeneratedLootInfoArray.Num() == 0)
	{
		return;
	}

	Super::OnGainFocus_Implementation(Character);
}

void ATreasureBoxBase::OnLoseFocus_Implementation(AEODCharacterBase* Character)
{
	Super::OnLoseFocus_Implementation(Character);
}

void ATreasureBoxBase::GenerateLootInfoArray()
{
	int32 GLID = 100;
	for (const FStoredLootInfo& StoredLootInfo : StoredLootInfoArray)
	{
		bool bShouldGenerate = StoredLootInfo.DropChance >= FMath::RandRange(0.f, 100.f) ? true : false;

		if (bShouldGenerate)
		{
			FGeneratedLootInfo GeneratedLootInfo;
			GeneratedLootInfo.GLID = GLID;
			GeneratedLootInfo.ItemCount = FMath::RandRange(1, FMath::Max(1, StoredLootInfo.MaxCount));
			GeneratedLootInfo.ItemClass = StoredLootInfo.ItemClass;
			GeneratedLootInfoArray.Add(GeneratedLootInfo);
			GLID++;
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

void ATreasureBoxBase::AcquireLoot_Implementation(const FGeneratedLootInfo& LootInfo, AEODPlayerController* EODPC)
{
	if (EODPC)
	{
		UInventoryComponent* InvComp = EODPC->GetInventoryComponent();
		check(InvComp);
		InvComp->AddLoot(LootInfo);
		int32 Num = GeneratedLootInfoArray.Num();
		for (int i = Num - 1; i >= 0; i--)
		{
			if (GeneratedLootInfoArray[i] == LootInfo)
			{
				GeneratedLootInfoArray.RemoveAt(i);
				break;
			}
		}
	}
}

void ATreasureBoxBase::OnAllLootPicked_Implementation(AEODPlayerController* EODPC)
{
	AEODCharacterBase* Character = EODPC ? Cast<AEODCharacterBase>(EODPC->GetPawn()) : nullptr;
	Execute_OnInteractionFinish(this, Character);
}

void ATreasureBoxBase::OnLootCancelled_Implementation(AEODPlayerController* EODPC)
{
	AEODCharacterBase* Character = EODPC ? Cast<AEODCharacterBase>(EODPC->GetPawn()) : nullptr;
	Execute_OnInteractionFinish(this, Character);
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
