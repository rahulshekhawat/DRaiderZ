// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UMainMenuWidget : public UMenuWidgetBase
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* CONTINUE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* NEW_CAMPAIGN;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* NEW_PROFILE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* SURVIVAL;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* MULTIPLAYER;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* SETTINGS;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UUserWidget* QUIT;

};
