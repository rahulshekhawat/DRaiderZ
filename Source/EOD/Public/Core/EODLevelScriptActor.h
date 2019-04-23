// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "EODLevelScriptActor.generated.h"

class USoundBase;
class AEODCharacterBase;

/**
 * 
 */
UCLASS()
class EOD_API AEODLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AEODLevelScriptActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// --------------------------------------
	//  Combat
	// --------------------------------------

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void OnCombatStarted(AEODCharacterBase* PlayerCharacter);
	virtual void OnCombatStarted_Implementation(AEODCharacterBase* PlayerCharacter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void OnCombatEnded(AEODCharacterBase* PlayerCharacter);
	virtual void OnCombatEnded_Implementation(AEODCharacterBase* PlayerCharacter);

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* CombatMusic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* BackgroundMusic;

	// --------------------------------------
	//  Components
	// --------------------------------------

	static const FName BGMAudioComponentName;

protected:

	/** Audio component for playing sound effects on getting hit */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound)
	UAudioComponent* BGMAudioComponent;



};
