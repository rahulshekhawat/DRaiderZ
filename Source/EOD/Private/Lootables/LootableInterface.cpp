#include "LootableInterface.h"

ULootableInterface::ULootableInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

TArray<FStoredLootInfo> ILootableInterface::GetStoredLootInfo_Implementation() const
{
	return TArray<FStoredLootInfo>();
}

TArray<FGeneratedLootInfo> ILootableInterface::GetGeneratedLootInfo_Implementation() const
{
	return TArray<FGeneratedLootInfo>();
}

void ILootableInterface::AcquireLoot_Implementation(const FGeneratedLootInfo& LootInfo, AEODPlayerController* EODPC)
{
}

void ILootableInterface::OnAllLootPicked_Implementation(AEODPlayerController* EODPC)
{
}

void ILootableInterface::OnLootCancelled_Implementation(AEODPlayerController* EODPC)
{
}

void ILootableInterface::GenerateLootInfoArray()
{
}
