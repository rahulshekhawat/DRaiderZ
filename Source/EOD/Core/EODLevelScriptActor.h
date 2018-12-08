// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "EODLevelScriptActor.generated.h"

class APlayerCharacter;

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
	void OnCombatStarted(APlayerCharacter* PlayerCharacter);

	virtual void OnCombatStarted_Implementation(APlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Combat)
	void OnCombatEnded(APlayerCharacter* PlayerCharacter);

	virtual void OnCombatEnded_Implementation(APlayerCharacter* PlayerCharacter);


};
