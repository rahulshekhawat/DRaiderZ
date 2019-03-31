// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EODGameModeBase.generated.h"

class AEODCharacterBase;
class AStatusEffectsManager;

/**
 * 
 */
UCLASS()
class EOD_API AEODGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	AEODGameModeBase(const FObjectInitializer& ObjectInitializer);

	/** Spawns actors of manager classes */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	// --------------------------------------
	//	Manager Classes
	// --------------------------------------

	FORCEINLINE AStatusEffectsManager* GetStatusEffectsManager() const;

	UFUNCTION(BlueprintPure, Category = Managers, meta = (DisplayName = "Get Status Effects Manager"))
	AStatusEffectsManager* BP_GetStatusEffectsManager() const;

	/** Blueprint class used for spawning female characters */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<AEODCharacterBase> FemalePawnClass;

	/** Blueprint class used for spawning male characters */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<AEODCharacterBase> MalePawnClass;

private:

	UPROPERTY(Transient)
	AStatusEffectsManager* StatusEffectsManager;
		
};


FORCEINLINE AStatusEffectsManager* AEODGameModeBase::GetStatusEffectsManager() const
{
	return StatusEffectsManager;
}
