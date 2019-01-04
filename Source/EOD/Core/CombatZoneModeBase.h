// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Core/EODGameModeBase.h"

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
	ACombatZoneModeBase(const FObjectInitializer& ObjectInitializer);

	/** Used to spawn combat and status effect manager actors */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	
	FORCEINLINE ACombatManager* GetCombatManager() const;

	UFUNCTION(BlueprintPure, Category = Managers, meta = (DisplayName = "Get Combat Manager"))
	ACombatManager* BP_GetCombatManager() const;

private:
	UPROPERTY(Transient)
	ACombatManager* CombatManager;

	UPROPERTY(EditDefaultsOnly, Category = Classes, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACombatManager> CombatManagerClass;

};

FORCEINLINE ACombatManager* ACombatZoneModeBase::GetCombatManager() const
{
	return CombatManager;
}
