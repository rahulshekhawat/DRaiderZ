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
	
	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	ATreasureBoxBase(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;


	///////////////////////////////////////////////////////////////////////////
	//  Interaction Interface
public:

	/** This event is called when a (player) character starts interacting with the interactive actor */
	virtual EInteractionResult OnInteractionStart_Implementation(AEODCharacterBase* Character) override;

	/** This event is called when a (player) character wants to update the current interaction with the interactive actor */
	virtual EInteractionResult OnInteractionUpdate_Implementation(AEODCharacterBase* Character) override;

	/** This event is called when a (player) character cancels interaction with the interactive actor */
	virtual void OnInteractionCancel_Implementation(AEODCharacterBase* Character, EInteractionCancelType CancelType) override;

	/** This event is called when a (player) character finishes interaction with the interactive actor */
	virtual void OnInteractionFinish_Implementation(AEODCharacterBase* Character) override;

	/** This event is called to highlight interactive actor's meshes */
	virtual void EnableCustomDepth_Implementation() override;

	/** This event is called to disable highlight on interactive actor's meshes */
	virtual void DisableCustomDepth_Implementation() override;

	virtual void OnGainFocus_Implementation(AEODCharacterBase* Character) override;

	virtual void OnLoseFocus_Implementation(AEODCharacterBase* Character) override;


	///////////////////////////////////////////////////////////////////////////
	//  Lootable Interface
public:

	virtual void GenerateLootInfoArray() override;

	virtual TArray<FStoredLootInfo> GetStoredLootInfo_Implementation() const override;

	virtual TArray<FGeneratedLootInfo> GetGeneratedLootInfo_Implementation() const override;

	virtual void AcquireLoot_Implementation(const FGeneratedLootInfo& LootInfo, class AEODPlayerController* EODPC) override;

	virtual void OnAllLootPicked_Implementation(class AEODPlayerController* EODPC) override;

	virtual void OnLootCancelled_Implementation(class AEODPlayerController* EODPC) override;


	///////////////////////////////////////////////////////////////////////////
	//  Utility
public:

	UFUNCTION(BlueprintCallable, Category = TreasureBox)
	void SpawnBox();

	UFUNCTION(BlueprintCallable, Category = TreasureBox)
	void OpenBox();

	UFUNCTION(BlueprintPure, Category = TreasureBox, meta = (DisplayName = "IsOpen"))
	bool BP_IsOpen() const;

	/** Returns true if this treasure box has any loot remaining */
	UFUNCTION(BlueprintPure, Category = TreasureBox)
	bool HasLoot() const;

	/** Returns true if this treasure box has already been opened */
	inline bool IsOpen() const;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = TreasureBox)
	bool bOpen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TreasureBox)
	bool bSpawnByDefault;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	UAnimSequenceBase* ChestDropAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	UAnimSequenceBase* ChestOpenAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	UAnimSequenceBase* HiddenIdleAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	UAnimSequenceBase* VisibleIdleAnimation;

	/** List of all the loot items that this box contains by default */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Loot)
	TArray<FStoredLootInfo> StoredLootInfoArray;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Loot)
	TArray<FGeneratedLootInfo> GeneratedLootInfoArray;

	UPROPERTY(Transient)
	bool bInteractionInProgress;

};

inline bool ATreasureBoxBase::IsOpen() const
{
	return bOpen;
}
