// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	//~ Begin Child Widgets
public:
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
	//~ End Child Widgets

};
