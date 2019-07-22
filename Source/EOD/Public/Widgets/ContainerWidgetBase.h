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


	///////////////////////////////////////////////////////////////////////////
	//  Behaviour
public:

	virtual void PostManualConstruction(UUserWidget* InParentWidget, UObject* InDataObj);

	virtual void ResetContainer();

	virtual void SetDataObj(UObject* InDataObj);

	virtual bool IsContainerEmpty() const;

	inline UObject* GetDataObj() const { return DataObj.Get(); }

protected:

	UPROPERTY(Transient)
	TWeakObjectPtr<UObject> DataObj;

	/** Sets the display icon of this container */
	UFUNCTION()
	void SetIcon(UTexture* NewIcon);

	UFUNCTION()
	void SetSubText(int32 InCurrentValue, int32 InMaxValue);

	UFUNCTION()
	void SetCooldown(float InCooldown);

};
