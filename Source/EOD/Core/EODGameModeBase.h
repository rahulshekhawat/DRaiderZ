// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EODGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API AEODGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	AEODGameModeBase(const FObjectInitializer& ObjectInitializer);

	/** Used to spawn combat and status effect manager actors */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	
	
	
};
