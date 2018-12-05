// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Interactives/InteractiveActor.h"
#include "InteractiveSkeletalMesh.generated.h"

class USkeletalMeshComponent;
class UCameraComponent;

/**
 * InteractiveSkeletalMesh is a type of InteractiveActor where the root component is a skeletal mesh
 */
UCLASS()
class EOD_API AInteractiveSkeletalMesh : public AInteractiveActor
{
	GENERATED_BODY()
	
public:

	AInteractiveSkeletalMesh(const FObjectInitializer& ObjectInitializer);

	virtual void EnableCustomDepth_Implementation() override;

	virtual void DisableCustomDepth_Implementation() override;

private:

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RootMesh;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* InteractionCamera;


};
