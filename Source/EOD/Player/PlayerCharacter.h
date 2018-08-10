// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Player/EODCharacterBase.h"
#include "PlayerCharacter.generated.h"

class UAnimMontage;
class USkeletalMeshComponent;
class UStaticMeshComponent;

/**
 * PlayerCharacter is the base class for playable characters
 */
UCLASS()
class EOD_API APlayerCharacter : public AEODCharacterBase
{
	GENERATED_BODY()
	
public:

	/** Create and initialize skeletal armor mesh, camera, and inventory components. */
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/** Spawn default weapon(s) */
	virtual void PostInitializeComponents() override;

#if WITH_EDITOR

	/**
	 * Detects changes to EditorArmorID and EditorWeaponID properties.
	 * If the ID changes, it looks up for respective weapon or armor in
	 * data tables and adds them to character.
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/** Updates player states */
	virtual void Tick(float DeltaTime) override;

protected:
	
	/** Initializes player animation references. Creates player HUD widget and adds it to the viewport. */
	virtual void BeginPlay() override;

private:
	
	//~ Begin camera components
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;
	//~ End camera components

	//~ Begin armor components
	//~ @note The default skeletal mesh component inherited from ACharacter class will reference the skeletal mesh for player chest
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
	//~ End armor components
	
	//~ Inventory component
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;
	
	/** A helper function that must be called from constructor. Creates and returns new armor skeletal mesh component */
	USkeletalMeshComponent* CreateNewArmorComponent(const FName Name, const FObjectInitializer& ObjectInitializer);
	

	//~ Begin input handlers
	bool bBlockPressed;
	
	const int CameraZoomRate = 15;

	const int CameraArmMinimumLength = 50;

	const int CameraArmMaximumLength = 500;

	//~ @note Pressing and releasing skill keys are separate events to support charge events (e.g. charge rage)

	/** Handles player pressing a skill key */
	void OnPressingSkillKey(const uint32 SkillButtonIndex);

	/** Handles player releasing a skill key */
	void OnReleasingSkillKey(const uint32 SkillButtonIndex);
	
	/** Move player forward/backward */
	void MoveForward(const float Value);

	/** Move player left/right */
	void MoveRight(const float Value);
	
	void ZoomInCamera();

	void ZoomOutCamera();

	void OnDodge();

	void EnableBlock();
	
	void DisableBlock();

	void OnJump();

	void OnInteract();

	void OnToggleSheath();

	void OnToggleCharacterStatsUI();

	void OnToggleMouseCursor();

	void OnNormalAttack();

	void OnToggleAutoRun();

	void EnableAutoRun();

	void DisableAutoRun();
	//~ End input handlers	

	//~ Begin UI
	/** The blueprint widget class to use for player HUD */
	UPROPERTY(EditDefaultsOnly, Category = CharacterWidgets)
	TSubclassOf<class UHUDWidget> BP_HUDWidget;

	/** Player HUD class reference */
	UPROPERTY(Transient)
	class UHUDWidget* HUDWidget;
	//~ End UI
	
	float MaxPlayerWalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = Gender)
	ECharacterGender Gender;

public:
	
	/**
	 * Returns player controller rotation yaw in -180/180 range.
	 * @note the yaw obtained from Controller->GetControlRotation().Yaw is in 0/360 range, which may not be desirable
	 */
	UFUNCTION(BlueprintCallable, category = PlayerRotationHandler)
	float GetPlayerControlRotationYaw();
	
	/**
	 * Rotate player toward desired yaw based on the rotation rate in given delta time
	 * @param DesiredYaw 	The desired yaw of player character
	 * @param DeltaTime 	The time between last and current tick
	 * @param RotationRate 	Desired rotation rate
	 * @return 				True if character successfully rotates to DesiredYaw (CurrentYaw == DesiredYaw)
	*/
	UFUNCTION(BlueprintCallable, category = PlayerRotationHandler)
	bool DeltaRotatePlayerToDesiredYaw(float DesiredYaw, float DeltaTime, float RotationRate = 600.f);
	
	/**
	 * Returns the expected rotation yaw of character based on current Axis Input.
	 * @warning Only call for locally controlled character otherwise it would lead to crash (intentional)
	 */
	UFUNCTION(BlueprintCallable, category = PlayerRotationHandler)
	float GetRotationYawFromAxisInput();

	FPlayerAnimationReferences* PlayerAnimationReferences;

	/** Animation montage references for currently equipped weapon */
	FPlayerAnimationReferences* EquippedWeaponAnimationReferences;

	/** Animations references for sheathed weapon */
	FPlayerAnimationReferences* SheathedWeaponAnimationReferences;

	/** A reference to player anim instance */
	class UPlayerAnimInstance* PlayerAnimInstance;
	
	/** Updates the 'PlayerAnimationReferences' variable based on current weapon equipped */
	void UpdatePlayerAnimationReferences();

	void UpdateEquippedWeaponAnimationReferences();

	FPlayerAnimationReferences* GetActiveAnimationReferences() const;

	UPROPERTY(Transient)
	class APrimaryWeapon* PrimaryWeapon;
	
	UPROPERTY(Transient)
	class ASecondaryWeapon* SecondaryWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapons)
	FName PrimaryWeaponID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapons)
	FName SecondaryWeaponID;

	void SetCurrentPrimaryWeapon(FName WeaponID);

	void SetCurrentSecondaryWeapon(FName WeaponID);

	void RemovePrimaryWeapon();

	void RemoveSecondaryWeapon();

	void UpdateCurrentWeaponAnimationType();

	bool IsPrimaryWeaponEquippped() const;

	bool IsSecondaryWeaponEquipped() const;
	
	//~ Begin state handlers
	virtual bool CanMove() const override;
	
	virtual bool CanJump() const override;

	virtual bool CanDodge() const override;
	
	virtual bool CanNormalAttack() const;

	bool CanAutoRun() const;
	
	bool IsAutoRunning() const;
	
	void UpdateIdleState(float DeltaTime);

	void UpdateMovement(float DeltaTime);

	void UpdateBlockState(float DeltaTime);

	void UpdateAutoRun(float DeltaTime);
	//~ End state handlers

	// bool bHasActiveiframes;

	// bool bIsBlockingDamage;

	//~ Begin combat events	
	FCombatEvent OnUsingSkill_0;
	FCombatEvent OnUsingSkill_1;
	FCombatEvent OnUsingSkill_2;
	FCombatEvent OnUsingSkill_3;
	FCombatEvent OnUsingSkill_4;
	FCombatEvent OnUsingSkill_5;
	FCombatEvent OnUsingSkill_6;
	FCombatEvent OnUsingSkill_7;
	FCombatEvent OnUsingSkill_8;
	FCombatEvent OnUsingSkill_9;
	//~ End combat events

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Skills)
	uint8 MaxNumberOfSkills;

	TArray<FCombatEvent> EventsOnUsingSkill;

	TArray<FCombatEvent> EventsOnSuccessfulSkillAttack;

	/** [server] Handle melee collision */
	virtual void OnMeleeCollision(UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit);

	/** [server] Apply damage to a character */
	virtual void ApplyEODDamage(AEODCharacterBase* HitCharacter, FEODDamage& EODDamage) override;

	/** [server] Take damage from another character */
	virtual void TakeEODDamage(AEODCharacterBase* Instigator, FEODDamage& EODDamage) override;

private:

	//~ Template functions

	/** Called when player presses a skill key */
	template<uint32 SkillButtonIndex>
	void PressedSkillKey();

	/** Called when player releases a skill key */
	template<uint32 SkillButtonIndex>
	void ReleasedSkillKey();

public:
	
	/** Character movement direction for Idle-Walk-Run state */
	UPROPERTY(Replicated)
	ECharMovementDirection IWR_CharacterMovementDirection;
	
	void SetIWRCharMovementDir(ECharMovementDirection NewDirection);
	
	/** Animations to use based on that is determined by equipped weapon */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentWeaponAnimationToUse)
	EWeaponAnimationType CurrentWeaponAnimationToUse;
	
	void SetCurrentWeaponAnimationToUse(EWeaponAnimationType NewWeaponAnimationType);
	
	/** The relative yaw of character's movement direction from the direction character is facing while blocking */
	UPROPERTY(Replicated)
	float BlockMovementDirectionYaw;

	void SetBlockMovementDirectionYaw(float NewYaw);

	/** Determines whether weapon is currently sheathed or not */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_WeaponSheathed)
	bool bWeaponSheathed;
	
	void SetWeaponSheathed(bool bNewValue);

private:
	
	//~ Begin multiplayer code
	UFUNCTION()
	void OnRep_WeaponSheathed();

	UFUNCTION()
	void OnRep_CurrentWeaponAnimationToUse();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIWRCharMovementDir(ECharMovementDirection NewDirection);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCurrentWeaponAnimationToUse(EWeaponAnimationType NewWeaponAnimationType);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetBlockMovementDirectionYaw(float NewYaw);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetWeaponSheathed(bool bNewValue);
	//~ End multiplayer code

};

template<uint32 SkillButtonIndex>
inline void APlayerCharacter::PressedSkillKey()
{
	OnPressingSkillKey(SkillButtonIndex);
}

template<uint32 SkillButtonIndex>
inline void APlayerCharacter::ReleasedSkillKey()
{
	OnReleasingSkillKey(SkillButtonIndex);
}
