// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "EODGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UEODGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

public:
	void ShowLoadingScreen();

	void HideLoadingScreen();


};
