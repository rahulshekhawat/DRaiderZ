// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "TitleScreenWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class EOD_API UTitleScreenWidget : public UMenuWidgetBase
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UTitleScreenWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	/** Title of the Game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	UTextBlock* TitleText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Child Widget", meta = (BindWidget))
	UTextBlock* CreditText;

};
