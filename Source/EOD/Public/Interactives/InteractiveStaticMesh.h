// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
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

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AInteractiveStaticMesh(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// --------------------------------------
	//  Interaction Interface
	// --------------------------------------

	virtual void EnableCustomDepth_Implementation() override;

	virtual void DisableCustomDepth_Implementation() override;

	// --------------------------------------
	//  Components
	// --------------------------------------

	FORCEINLINE	UStaticMeshComponent* GetPrimaryMesh() const { return PrimaryMesh; }

	FORCEINLINE UCameraComponent* GetInteractionCamera() const { return InteractionCamera; }

	static const FName PrimaryMeshComponentName;

	static const FName InteractionCameraComponentName;

private:

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PrimaryMesh;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* InteractionCamera;
	
};
