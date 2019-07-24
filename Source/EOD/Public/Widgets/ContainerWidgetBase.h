// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "ContainerWidgetBase.generated.h"

class UDragVisualWidget;
class UTooltipWidget;
class UContainerWidgetBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FContainerMCDelegate, UContainerWidgetBase*, Container);

/**
 * Base class for container widgets
 */
UCLASS()
class EOD_API UContainerWidgetBase : public UUserWidget
{
	GENERATED_BODY()

	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	UContainerWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


	///////////////////////////////////////////////////////////////////////////
	//  Child Widgets
protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UBorder* RootBorder;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UButton* MainButton;

	/** The icon of the item placed in this container */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UImage* ItemImage;

	/** SubText displays the number of items placed in the container */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* SubText;

	/** Displays the remaining cooldown */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* CooldownText;


	///////////////////////////////////////////////////////////////////////////
	//  Mouse Events
public:

	FContainerMCDelegate OnClicked;

protected:

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	virtual void MainButtonClicked();


	///////////////////////////////////////////////////////////////////////////
	//  Behaviour
public:

	virtual void PostManualConstruction(UUserWidget* InParentWidget, UObject* InDataObj);

	virtual void ResetContainer();

	virtual void SetDataObj(UObject* InDataObj);

	virtual bool IsContainerEmpty() const;

	inline UObject* GetDataObj() const { return DataObj.Get(); }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Classes)
	TSubclassOf<UTooltipWidget> TooltipWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Classes)
	TSubclassOf<UDragVisualWidget> DragVisualClass;

	UPROPERTY(Transient)
	TWeakObjectPtr<UObject> DataObj;

	/** Sets the display icon of this container */
	UFUNCTION()
	void SetIcon(UTexture* NewIcon);

	UFUNCTION()
	void SetSubText(int32 InCurrentValue, int32 InMaxValue);

	UFUNCTION()
	void SetCooldown(float InCooldown);

	UFUNCTION()
	UWidget* GetTooltipWidget();

	void InitializeTooltipWidget();

	/** Update tooltip widget description from DataObj */
	virtual void UpdateTooltipWidget();

};
