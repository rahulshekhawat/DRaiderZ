// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactives/InteractiveActor.h"
#include "InteractiveStaticMesh.generated.h"

class UStaticMeshComponent;
class UCameraComponent;

/**
 * InteractiveSkeletalMesh is a type of InteractiveActor where the root component is a static mesh
 */
UCLASS()
class EOD_API AInteractiveStaticMesh : public AInteractiveActor
{
	GENERATED_BODY()

public:

	AInteractiveStaticMesh(const FObjectInitializer& ObjectInitializer);

	virtual void EnableCustomDepth_Implementation() override;

	virtual void DisableCustomDepth_Implementation() override;

private:

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RootMesh;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* InteractionCamera;
	
};
