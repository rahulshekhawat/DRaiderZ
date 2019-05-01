// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interactives/InteractiveActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "InteractiveHumanActor.generated.h"

class UCapsuleComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class EOD_API AInteractiveHumanActor : public AInteractiveActor
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AInteractiveHumanActor(const FObjectInitializer& ObjectInitializer);

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// --------------------------------------
	//  Psuedo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material Colors")
	FLinearColor ClothOverrideColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material Colors")
	FLinearColor HairOverrideColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material Colors")
	FLinearColor HatItemOverrideColor;

	// --------------------------------------
	//  Interaction Interface
	// --------------------------------------

	/** This event is called to highlight interactive actor's meshes */
	virtual void EnableCustomDepth_Implementation() override;

	/** This event is called to disable highlight on interactive actor's meshes */
	virtual void DisableCustomDepth_Implementation() override;

	/** This event is called when a (player) character interacts with the underlying actor */
	virtual void OnInteract_Implementation(AEODCharacterBase* Character) override;

	// --------------------------------------
	//  Components
	// --------------------------------------

	static const FName CapsuleComponentName;

	static const FName HairComponentName;

	static const FName HatItemComponentName;

	static const FName FaceComponentName;

	static const FName FaceItemComponentName;

	static const FName ChestComponentName;

	static const FName HandsComponentName;

	static const FName LegsComponentName;

	static const FName FeetComponentName;

	static const FName InteractionCameraComponentName;

	FORCEINLINE USkeletalMeshComponent* GetHair() const { return Hair; }

	FORCEINLINE USkeletalMeshComponent* GetHatItem() const { return HatItem; }

	FORCEINLINE USkeletalMeshComponent* GetFace() const { return Face; }

	FORCEINLINE USkeletalMeshComponent* GetFaceItem() const { return FaceItem; }

	FORCEINLINE USkeletalMeshComponent* GetChest() const { return Chest; }
	
	FORCEINLINE USkeletalMeshComponent* GetHands() const { return Hands; }

	FORCEINLINE USkeletalMeshComponent* GetLegs() const { return Legs; }
	
	FORCEINLINE USkeletalMeshComponent* GetFeet() const { return Feet; }

	FORCEINLINE UCameraComponent* GetInteractionCamera() const { return InteractionCamera; }

private:

	/** The CapsuleComponent being used for collision detection */
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	//~ @note The default skeletal mesh component inherited from ACharacter class will reference the skeletal mesh for player face

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Hair;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* HatItem;
	
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Face;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FaceItem;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Chest;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Hands;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Legs;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Feet;
	
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* InteractionCamera;

	/** [Constructor Only] A helper function that creates and returns new armor skeletal mesh component */
	USkeletalMeshComponent* CreateNewArmorComponent(const FName Name, const FObjectInitializer& ObjectInitializer);

	inline void SetMasterPoseComponentForMeshes();

};

inline void AInteractiveHumanActor::SetMasterPoseComponentForMeshes()
{
	if (Face)
	{
		if (Hair) { Hair->SetMasterPoseComponent(Face); }
		if (HatItem) { HatItem->SetMasterPoseComponent(Face); }
		if (FaceItem) { FaceItem->SetMasterPoseComponent(Face); }
		if (Chest) { Chest->SetMasterPoseComponent(Face); }
		if (Hands) { Hands->SetMasterPoseComponent(Face); }
		if (Legs) { Legs->SetMasterPoseComponent(Face); }
		if (Feet) { Feet->SetMasterPoseComponent(Face); }
	}
}
