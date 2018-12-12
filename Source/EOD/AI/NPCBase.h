// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Player/EODCharacterBase.h"
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

	virtual void OnInteract_Implementation(AEODCharacterBase* Character) override;
	// virtual void OnInteract_Implementation(const AEODCharacterBase* Character, UUserWidget* DialogueWidget) override;

	virtual void EnableCustomDepth_Implementation() override;

	virtual void DisableCustomDepth_Implementation() override;

private:

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* InteractionCamera;


};
