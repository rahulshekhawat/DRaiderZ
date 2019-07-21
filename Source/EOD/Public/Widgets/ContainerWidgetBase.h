// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ContainerWidgetBase.generated.h"

/**
 * Base class for container widgets
 */
UCLASS()
class EOD_API UContainerWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UContainerWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	class UBorder* RootBorder;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	class UButton* MainButton;

	/** The icon of the item placed in this container */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	class UImage* ItemImage;

	/** SubText displays the number of items placed in the container */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	class UTextBlock* SubText;

	/** Displays the remaining cooldown */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	class UTextBlock* CooldownText;

public:

	///////////////////////////////////////////////////////////////////////////
	//  Behaviour

	virtual void PostManualConstruction(UUserWidget* InParentWidget, UObject* InContainerData);

	virtual bool IsContainerEmpty() const;

protected:

	UPROPERTY(Transient)
	UObject* ContainerData;


};
