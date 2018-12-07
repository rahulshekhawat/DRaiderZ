// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "StatusEffectsManager.generated.h"

class UStatusEffectBase;
class AEODCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatusEffectEvent, UStatusEffectBase*, StatusEffect, AEODCharacterBase*, Character);

/**
 * 
 */
UCLASS()
class EOD_API AStatusEffectsManager : public AInfo
{
	GENERATED_BODY()
	
public:

	AStatusEffectsManager(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	// @note map should be between status effect class and character
	// UPROPERTY()
	// TMap<UStatusEffectBase*, AEODCharacterBase*> StatusEffectToCharacterMap;

	UPROPERTY()
	TMap<TSubclassOf<UStatusEffectBase>, AEODCharacterBase*> StatusEffectClassToCharacterMap;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Status Effects")
	FStatusEffectEvent StatusEffectActivated;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Status Effects")
	FStatusEffectEvent StatusEffectDeactivated;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Status Effects")
	FStatusEffectEvent StatusEffectTriggered;

	

};
