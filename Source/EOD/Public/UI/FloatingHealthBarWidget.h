// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingHealthBarWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class EOD_API UFloatingHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFloatingHealthBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UFUNCTION()
	void UpdateHealth(int32 BaseHealth, int32 MaxHealth, int32 CurrentHealth);

	
};
