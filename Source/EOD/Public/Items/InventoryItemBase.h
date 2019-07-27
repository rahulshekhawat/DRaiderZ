// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemBase.generated.h"

class UTexture;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class EOD_API UInventoryItemBase : public UObject
{
	GENERATED_BODY()
	
	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	UInventoryItemBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	FText InGameName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	FText InGameDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	FName ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	UTexture* Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseInfo)
	bool bStackable;


	///////////////////////////////////////////////////////////////////////////
	//  Behaviour
public:

	/** Event called when player clicks on the inventory item */
	virtual void OnClick(class AEODPlayerController* InPC);

};
