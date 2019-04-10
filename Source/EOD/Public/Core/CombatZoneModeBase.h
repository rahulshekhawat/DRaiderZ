// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODGameModeBase.h"

#include "CombatZoneModeBase.generated.h"

class ACombatManager;

/**
 * Base class for game modes used in combat areas
 */
UCLASS()
class EOD_API ACombatZoneModeBase : public AEODGameModeBase
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	ACombatZoneModeBase(const FObjectInitializer& ObjectInitializer);

	/** Used to spawn combat and status effect manager actors */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Updates game state every frame */
	virtual void Tick(float DeltaTime) override;

	// --------------------------------------
	//	Manager Classes
	// --------------------------------------

	FORCEINLINE ACombatManager* GetCombatManager() const;

	UFUNCTION(BlueprintPure, Category = Managers, meta = (DisplayName = "Get Combat Manager"))
	ACombatManager* BP_GetCombatManager() const;

protected:

	/** Blueprint class used for spawning combat manager */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<ACombatManager> CombatManagerClass;

	UPROPERTY(Transient)
	ACombatManager* CombatManager;

};

FORCEINLINE ACombatManager* ACombatZoneModeBase::GetCombatManager() const
{
	return CombatManager;
}
