// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VocationInfoWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class EOD_API UVocationInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UVocationInfoWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* WeaponsListText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* CharacteristicsListText;

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info Text")
	FString WeaponsList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info Text")
	FString CharacteristicsList;

};
