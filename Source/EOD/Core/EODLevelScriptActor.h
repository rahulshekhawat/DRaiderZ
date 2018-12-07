// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "EODLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API AEODLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:

	AEODLevelScriptActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Combat)
	void OnCombatStarted();

	virtual void OnCombatStarted_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Combat)
	void OnCombatEnded();

	virtual void OnCombatEnded_Implementation();


};
