// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "Engine/DataAsset.h"
#include "HealthPotionBase.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UHealthPotionBase : public UPrimaryDataAsset, public IInventoryInterface
{
	GENERATED_BODY()

public:

	UHealthPotionBase(const FObjectInitializer& ObjectInitializer);

	virtual void OnClick_Implementation(AEODCharacterBase* ClickingChar) const override;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Stats")
	float Value;

};
