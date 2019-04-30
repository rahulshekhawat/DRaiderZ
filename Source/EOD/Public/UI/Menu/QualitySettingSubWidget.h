// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QualitySettingSubWidget.generated.h"

class UScrollButtonWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQualityLevelSelectedMCDelegate, int32, QualityLevelSelected);

/**
 * 
 */
UCLASS()
class EOD_API UQualitySettingSubWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UQualitySettingSubWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Low;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Medium;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* High;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Epic;
	
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UScrollButtonWidget* Cinematic;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void SetCustomParentWidget(UUserWidget* NewParent);
	
protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Utility")
	UUserWidget* CustomParentWidget;

public:

	// --------------------------------------
	//  Events
	// --------------------------------------

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FQualityLevelSelectedMCDelegate OnQualitySelected;

protected:

	UFUNCTION()
	void HandleLowButtonClicked();

	UFUNCTION()
	void HandleMediumButtonClicked();

	UFUNCTION()
	void HandleHighButtonClicked();

	UFUNCTION()
	void HandleEpicButtonClicked();

	UFUNCTION()
	void HandleCinematicButtonClicked();


};
