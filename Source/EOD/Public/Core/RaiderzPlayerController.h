// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "HUDWidget.h"
#include "GameFramework/PlayerController.h"
#include "RaiderzPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API ARaiderzPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ARaiderzPlayerController();

	inline class UHUDWidget* GetHUDWidget() const { return HUDWidget; }
	
protected:

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Raiderz Widgets")
	class UHUDWidget* HUDWidget;
	
	
};
