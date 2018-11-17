// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/EODCharacterBase.h"
#include "NPCBase.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API ANPCBase : public AEODCharacterBase
{
	GENERATED_BODY()

public:

	ANPCBase(const FObjectInitializer& ObjectInitializer);

	/** Called when a character attempts to interact with this NPC */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	void OnInteract(const AEODCharacterBase* Character);

	virtual void OnInteract_Implementation(const AEODCharacterBase* Character);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	void EnableCustomDepth();

	virtual void EnableCustomDepth_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	void DisableCustomDepth();

	virtual void DisableCustomDepth_Implementation();


};
