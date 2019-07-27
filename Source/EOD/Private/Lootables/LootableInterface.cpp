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

/*
int32 ILootableInterface::AcquireLootItem_Implementation(TSubclassOf<UObject> LootItemClass, AEODCharacterBase* Looter)
{
	return int32();
}
*/

void ILootableInterface::GenerateLootInfoArray()
{
}
