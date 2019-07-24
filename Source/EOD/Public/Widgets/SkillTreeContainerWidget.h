// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ContainerWidgetBase.h"
#include "SkillTreeContainerWidget.generated.h"

class UButton;
class USkillTreeWidget;
/**
 * 
 */
UCLASS()
class EOD_API USkillTreeContainerWidget : public UContainerWidgetBase
{
	GENERATED_BODY()

	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	USkillTreeContainerWidget(const FObjectInitializer& ObjectInitializer);

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

	inline void SetParentSkillTreeWidget(USkillTreeWidget* ParentWidget);

	virtual void SetDataObj(UObject* InDataObj) override;

	void EnableUpgradeButton();

	void DisableUpgradeButton();

	FContainerMCDelegate OnUpgradeButtonClicked;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UButton* UpgradeButton;

	UPROPERTY(Transient)
	USkillTreeWidget* ParentTreeWidget;

	UFUNCTION()
	virtual void UpgradeButtonClicked();
	
};

inline void USkillTreeContainerWidget::SetParentSkillTreeWidget(USkillTreeWidget* ParentWidget)
{
	ParentTreeWidget = ParentWidget;
}
