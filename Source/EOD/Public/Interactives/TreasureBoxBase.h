// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactives/InteractiveSkeletalMesh.h"
#include "TreasureBoxBase.generated.h"

class UAnimSequenceBase;

/**
 * 
 */
UCLASS()
class EOD_API ATreasureBoxBase : public AInteractiveSkeletalMesh
{
	GENERATED_BODY()

public:

	ATreasureBoxBase(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:

	inline bool IsOpen() const { return bOpen; }

	virtual void OnInteract_Implementation(AEODCharacterBase* Character) override;

	UFUNCTION(BlueprintCallable, Category = "Treasure Box")
	void SpawnBox();

	UFUNCTION(BlueprintCallable, Category = "Treasure Box")
	void OpenBox();

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

};
