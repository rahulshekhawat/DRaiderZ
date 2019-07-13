// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LootableInterface.h"
#include "InteractiveSkeletalMesh.h"
#include "TreasureBoxBase.generated.h"

class UAnimSequenceBase;

/**
 * 
 */
UCLASS()
class EOD_API ATreasureBoxBase : public AInteractiveSkeletalMesh, public ILootableInterface
{
	GENERATED_BODY()

public:

	ATreasureBoxBase(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	//~ Begin Interactive Interface
	virtual void OnInteract_Implementation(AEODCharacterBase* Character) override;
	//~ End Interactive Interface

	//~ Begin Lootable Interface
	virtual void GenerateLootInfoArray() override;
	virtual TArray<FStoredLootInfo> GetStoredLootInfo_Implementation() const override;
	virtual TArray<FGeneratedLootInfo> GetGeneratedLootInfo_Implementation() const override;
	virtual int32 AcquireLootItem_Implementation(TSubclassOf<UObject> LootItemClass, AEODCharacterBase* Looter) override;
	//~ End Lootable Interface

	UFUNCTION(BlueprintCallable, Category = "Treasure Box")
	void SpawnBox();

	UFUNCTION(BlueprintCallable, Category = "Treasure Box")
	void OpenBox();

	UFUNCTION(BlueprintPure, Category = "Treasure Box", meta = (DisplayName = "IsOpen"))
	bool BP_IsOpen() const;

	/** Returns true if this treasure box has any loot remaining */
	UFUNCTION(BlueprintPure, Category = "Treasure Box", meta = (DisplayName = "IsOpen"))
	bool HasLoot() const;

	/** Returns true if this treasure box has already been opened */
	inline bool IsOpen() const;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Treasure Box")
	bool bOpen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Treasure Box")
	bool bSpawnByDefault;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequenceBase* ChestDropAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequenceBase* ChestOpenAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequenceBase* HiddenIdleAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimSequenceBase* VisibleIdleAnimation;

	/** List of all the loot items that this box contains by default */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
	TArray<FStoredLootInfo> StoredLootInfoArray;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Loot")
	TArray<FGeneratedLootInfo> GeneratedLootInfoArray;

};

inline bool ATreasureBoxBase::IsOpen() const
{
	return bOpen;
}
