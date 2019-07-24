// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ContainerWidgetBase.h"
#include "SkillBarContainerWidget.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API USkillBarContainerWidget : public UContainerWidgetBase
{
	GENERATED_BODY()
		
	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	USkillBarContainerWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


	///////////////////////////////////////////////////////////////////////////
	//  Mouse Events
protected:

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


	///////////////////////////////////////////////////////////////////////////
	//  Behaviour
public:

	virtual void SetDataObj(UObject* InDataObj) override;

	virtual void MainButtonClicked() override;

protected:

	/** Update tooltip widget description from DataObj */
	virtual void 
		UpdateTooltipWidget() override;

	///////////////////////////////////////////////////////////////////////////
	//  Utility
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Utility)
	int32 SkillBarIndex;

	
};
