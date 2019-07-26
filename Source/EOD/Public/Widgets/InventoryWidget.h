// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODLibrary.h"

#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UImage;
class UScrollBox;
class UGridPanel;

/**
 * InventoryWidget is the visual representation of player inventory
 */
UCLASS()
class EOD_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	UInventoryWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


	///////////////////////////////////////////////////////////////////////////
	//  Utility
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Utility)
	void AddContainer(class UInventoryContainerWidget* InvContainer);
	virtual void AddContainer_Implementation(class UInventoryContainerWidget* InvContainer);

	UFUNCTION()
	void AddItem(const FInventoryItem& Item);

	UFUNCTION()
	void RemoveItem(const FInventoryItem& Item);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Containers)
	TArray<class UInventoryContainerWidget*> InvContainers;


	
};
