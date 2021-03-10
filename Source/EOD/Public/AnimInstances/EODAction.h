// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "EODAction.generated.h"

/**
* A struct that defines an action, i.e., a montage animation that can be played by the character
*/
USTRUCT(BlueprintType, meta = (DisplayName = "EOD Action"))
struct EOD_API FEODAction
{
	GENERATED_BODY()
	
	FEODAction() :
        StartSectionName(NAME_None),
        ActionMontage(nullptr)
	{
	}

	bool IsValid() const { return ActionMontage != nullptr; }

	FName GetStartSectionName() const { return StartSectionName; }

	UAnimMontage* GetActionMontage() const { return ActionMontage; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	FName StartSectionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	UAnimMontage* ActionMontage;
};

USTRUCT(BlueprintType)
struct EOD_API FWeaponActionTableRow : public FTableRowBase
{
	GENERATED_BODY()

	/** A map of weapon type tag to BDAction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
	TMap<FGameplayTag, FEODAction> WeaponToActionMap;	
};
