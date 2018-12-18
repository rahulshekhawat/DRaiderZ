// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Characters/EODCharacterBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "HumanCharacter.generated.h"

class AController;
class USkillsComponent;
class UAudioComponent;
class USkeletalMeshComponent;

/**
 * 
 */
UCLASS()
class EOD_API AHumanCharacter : public AEODCharacterBase
{
	GENERATED_BODY()

public:
	AHumanCharacter(const FObjectInitializer& ObjectInitializer);

	/** Called to bind functionality to input that is specific to this character */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Spawn default weapon(s) */
	virtual void PostInitializeComponents() override;

	virtual void PostInitProperties() override;

	virtual void Tick(float DeltaTime) override;

	/** Initializes player animation references. Creates player HUD widget and adds it to the viewport. */
	virtual void BeginPlay() override;

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;


	////////////////////////////////////////////////////////////////////////////////
	// COMPONENTS
	////////////////////////////////////////////////////////////////////////////////
private:
	//~ @note The default skeletal mesh component inherited from ACharacter class will reference the skeletal mesh for player face

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Hair;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* HatItem;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FaceItem;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Chest;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Hands;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Legs;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Feet;

	/** [Constructor Only] A helper function that creates and returns new armor skeletal mesh component */
	USkeletalMeshComponent* CreateNewArmorComponent(const FName Name, const FObjectInitializer& ObjectInitializer);

	FORCEINLINE void SetMasterPoseComponentForMeshes();


	////////////////////////////////////////////////////////////////////////////////
	// ACTIONS
	////////////////////////////////////////////////////////////////////////////////
public:
	virtual bool StartDodging() override;

	virtual bool StopDodging() override;

	virtual bool StartBlockingAttacks() override;

	virtual bool StopBlockingAttacks() override;

	////////////////////////////////////////////////////////////////////////////////
	// INPUT
	////////////////////////////////////////////////////////////////////////////////
private:
	void OnPressedForward();

	void OnPressedBackward();

	void OnReleasedForward();

	void OnReleasedBackward();

	bool bForwardPressed;

	bool bBackwardPressed;

	/** Timer handle needed for executing SP normal attacks */
	FTimerHandle SPAttackTimerHandle;

	UFUNCTION()
	void DisableForwardPressed();

	UFUNCTION()
	void DisableBackwardPressed();


	////////////////////////////////////////////////////////////////////////////////
	// MATERIALS
	////////////////////////////////////////////////////////////////////////////////
public:
	virtual void TurnOnTargetSwitch() override;

	virtual void TurnOffTargetSwitch() override;


};

FORCEINLINE void AHumanCharacter::SetMasterPoseComponentForMeshes()
{
	if (GetMesh())
	{
		Hair->SetMasterPoseComponent(GetMesh());
		HatItem->SetMasterPoseComponent(GetMesh());
		FaceItem->SetMasterPoseComponent(GetMesh());
		Chest->SetMasterPoseComponent(GetMesh());
		Hands->SetMasterPoseComponent(GetMesh());
		Legs->SetMasterPoseComponent(GetMesh());
		Feet->SetMasterPoseComponent(GetMesh());
	}
}
