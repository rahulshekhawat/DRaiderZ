// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HUDWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHUDWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EOD_API IHUDWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD Widget")
    void InitializeHUDWidget();
	virtual void InitializeHUDWidget_Implementation() { ; }
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD Widget")
    void SetPlayerName(const FText& NewName);
	virtual void SetPlayerName_Implementation(const FText& NewName) { ; }
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD Widget")
    void SetPlayerLevel(int32 NewLevel);
	virtual void SetPlayerLevel_Implementation(int32 NewLevel) { ; }
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD Widget")
    void SetPlayerHealth(float CurrentHealth, float MaxHealth);
	virtual void SetPlayerHealth_Implementation(float CurrentHealth, float MaxHealth) { ; }
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD Widget")
    void SetPlayerMana(float CurrentMana, float MaxMana);
	virtual void SetPlayerMana_Implementation(float CurrentMana, float MaxMana) { ; }
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD Widget")
    void SetPlayerStamina(float CurrentStamina, float MaxStamina);
	virtual void SetPlayerStamina_Implementation(float CurrentStamina, float MaxStamina) { ; }
	
};
