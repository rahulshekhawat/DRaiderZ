// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/EODLevelScriptActor.h"
#include "MainMenuLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API AMainMenuLevelScriptActor : public AEODLevelScriptActor
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AMainMenuLevelScriptActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void OnSwitchingToMainMenu();
	
protected:

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	USoundBase* MainMenuMusic;


};
