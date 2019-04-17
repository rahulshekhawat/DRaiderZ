// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
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

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AInteractiveSkeletalMesh(const FObjectInitializer& ObjectInitializer);

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

	FORCEINLINE USkeletalMeshComponent* GetPrimaryMesh() const { return PrimaryMesh; }

	FORCEINLINE UCameraComponent* GetInteractionCamera() const { return InteractionCamera; }

	static const FName PrimaryMeshComponentName;

	static const FName InteractionCameraComponentName;

private:

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* PrimaryMesh;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* InteractionCamera;


};
