// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UMenuWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UMenuWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Utility")
	void AddMenuToScreen();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Utility")
	void RemoveMenuFromScreen();


};
