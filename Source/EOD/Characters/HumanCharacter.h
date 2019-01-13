// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Weapons/PrimaryWeapon.h"
#include "EOD/Statics/CharacterLibrary.h"
#include "EOD/Characters/EODCharacterBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "HumanCharacter.generated.h"


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

	virtual void Tick(float DeltaTime) override;

	/** Initializes player animation references. Creates player HUD widget and adds it to the viewport. */
	virtual void BeginPlay() override;

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;

	//~
public:
	/** Saves current player state */
	virtual void SaveCharacterState() override;
	//~

	//~ Begin Components
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

	// For playing hit sound effects
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AudioComponent;

	/** [Constructor Only] A helper function that creates and returns new armor skeletal mesh component */
	USkeletalMeshComponent* CreateNewArmorComponent(const FName Name, const FObjectInitializer& ObjectInitializer);

	FORCEINLINE void SetMasterPoseComponentForMeshes();
	//~ End Components


	//~ Begin Animation Handling
public:
	inline FPlayerAnimationReferencesTableRow* GetActiveAnimationReferences() const;

private:
	/** Animations for this player when it has a weapon equipped */
	FPlayerAnimationReferencesTableRow* EquippedWeaponAnimationReferences;

	/** Animations for this player when it does not have a weapon equipped */
	FPlayerAnimationReferencesTableRow* UnequippedWeaponAnimationReferences;
	//~ End Animation Handling


	//~ Begin Weapon System
public:
	/** Returns the weapon type of primary weapon */
	FORCEINLINE EWeaponType GetEquippedWeaponType() const;

private:
	/** An actor for primary weapon equipped by the player */
	UPROPERTY(Transient)
	APrimaryWeapon* PrimaryWeapon;
	//~ End Weapon System


	//~ Begin Character Action Handling
public:
	/** Returns true if character can dodge */
	virtual bool CanDodge() const;

	virtual bool StartDodge() override;

	virtual bool StopDodge() override;

	virtual void EnableCharacterGuard() override;

	virtual void DisableCharacterGuard() override;
	//~ End Character Action Handling

	//~ Begin Input Handling
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
	//~ End Input Handling


	//~ Begin Materials
public:
	virtual void TurnOnTargetSwitch() override;

	virtual void TurnOffTargetSwitch() override;
	//~ End Materials


	//~ Begin Network Code
private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartDodge();

	UFUNCTION(NetMultiCast, Reliable)
	void Multicast_StartDodge();
	//~ End Network Code
	


};

FORCEINLINE void AHumanCharacter::SetMasterPoseComponentForMeshes()
{
	if (IsValid(GetMesh()))
	{
		if (IsValid(Hair))		{ Hair->SetMasterPoseComponent(GetMesh()); }
		if (IsValid(HatItem))	{ HatItem->SetMasterPoseComponent(GetMesh()); }
		if (IsValid(FaceItem))	{ FaceItem->SetMasterPoseComponent(GetMesh()); }
		if (IsValid(Chest))		{ Chest->SetMasterPoseComponent(GetMesh()); }
		if (IsValid(Hands))		{ Hands->SetMasterPoseComponent(GetMesh()); }
		if (IsValid(Legs))		{ Legs->SetMasterPoseComponent(GetMesh()); }
		if (IsValid(Feet))		{ Feet->SetMasterPoseComponent(GetMesh()); }
	}
}

inline FPlayerAnimationReferencesTableRow* AHumanCharacter::GetActiveAnimationReferences() const
{
	if (IsWeaponSheathed() || GetEquippedWeaponType() == EWeaponType::None)
	{
		return UnequippedWeaponAnimationReferences;
	}
	return EquippedWeaponAnimationReferences;
}

EWeaponType AHumanCharacter::GetEquippedWeaponType() const
{
	return PrimaryWeapon ? PrimaryWeapon->GetWeaponType() : EWeaponType::None;
}
