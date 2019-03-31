// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "EODLevelScriptActor.generated.h"

class AEODCharacterBase;

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
	void OnCombatStarted(AEODCharacterBase* PlayerCharacter);

	virtual void OnCombatStarted_Implementation(AEODCharacterBase* PlayerCharacter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Combat)
	void OnCombatEnded(AEODCharacterBase* PlayerCharacter);

	virtual void OnCombatEnded_Implementation(AEODCharacterBase* PlayerCharacter);


};
