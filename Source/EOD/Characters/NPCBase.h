// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Characters/EODCharacterBase.h"
#include "Interactives/InteractionInterface.h"
#include "NPCBase.generated.h"

class UCameraComponent;

/**
 * 
 */
UCLASS()
class EOD_API ANPCBase : public AEODCharacterBase, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ANPCBase(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void OnInteract_Implementation(AEODCharacterBase* Character) override;

	virtual void EnableCustomDepth_Implementation() override;

	virtual void DisableCustomDepth_Implementation() override;

protected:
	/** Used to determine whether we need to enable movement or not for this NPC */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (DisplayName = "NPC Needs Movement"))
	bool bNPCNeedsMovement;

};
