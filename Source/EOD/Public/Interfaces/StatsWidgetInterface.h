// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StatsWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStatsWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EOD_API IStatsWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stats Widget")
    void InitializeStatsWidget();
	virtual void InitializeStatsWidget_Implementation() { ; }
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stats Widget")
    void SetPlayerName(const FText& NewName);
	virtual void SetPlayerName_Implementation(const FText& NewName) { ; }
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stats Widget")
    void SetPlayerLevel(int32 NewLevel);
	virtual void SetPlayerLevel_Implementation(int32 NewLevel) { ; }
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stats Widget")
    void SetPlayerHealth(float CurrentHealth, float MaxHealth);
	virtual void SetPlayerHealth_Implementation(float CurrentHealth, float MaxHealth) { ; }
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stats Widget")
    void SetPlayerMana(float CurrentMana, float MaxMana);
	virtual void SetPlayerMana_Implementation(float CurrentMana, float MaxMana) { ; }
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stats Widget")
    void SetPlayerStamina(float CurrentStamina, float MaxStamina);
	virtual void SetPlayerStamina_Implementation(float CurrentStamina, float MaxStamina) { ; }
	
};
