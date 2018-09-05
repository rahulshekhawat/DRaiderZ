// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Player/EODCharacterBase.h"
#include "PlayerCharacter.generated.h"

class UHUDWidget;
class UAnimMontage;
class APrimaryWeapon;
class ASecondaryWeapon;
class UPlayerAnimInstance;
class UStaticMeshComponent;
class USkeletalMeshComponent;

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

	virtual void PostInitProperties() override;

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

	/** Initializes player animation references. Creates player HUD widget and adds it to the viewport. */
	virtual void BeginPlay() override;

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;

private:
	
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;

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
	
	//~ Inventory component
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;
	
	/** A helper function that must be called from constructor. Creates and returns new armor skeletal mesh component */
	USkeletalMeshComponent* CreateNewArmorComponent(const FName Name, const FObjectInitializer& ObjectInitializer);
	
public:

	/** Returns true if character can move */
	virtual bool CanMove() const override;

	/** Returns true if character can jump */
	virtual bool CanJump() const override;

	/** Returns true if character can dodge */
	virtual bool CanDodge() const override;

	/** Returns true if character can use normal attack */
	virtual bool CanNormalAttack() const override;

	/** Returns true if character can auto run */
	bool CanAutoRun() const;

	/** Returns true if player is on auto run */
	bool IsAutoRunning() const;

	/** Returns true if primary weapon is equipped */
	bool IsPrimaryWeaponEquippped() const;

	/** Returns true if secondary weapon is equipped */
	bool IsSecondaryWeaponEquipped() const;

	APrimaryWeapon* GetPrimaryWeapon() const;

	ASecondaryWeapon* GetSecondaryWeapon() const;

	/** Replace primary weapon with a new weapon */
	void SetCurrentPrimaryWeapon(const FName WeaponID);

	/** Replace secondary weapon with a new weapon */
	void SetCurrentSecondaryWeapon(const FName WeaponID);

	/** Removes primary weapon if it is currently equipped */
	void RemovePrimaryWeapon();

	/** Removes secondary weapon if it is currently equipped */
	void RemoveSecondaryWeapon();

	/** [server + client] Change idle-walk-run direction of character */
	void SetIWRCharMovementDir(ECharMovementDirection NewDirection);

	//~ DEPRECATED
	/** [server + client] Change current weapon animation to use */
	void SetCurrentWeaponAnimationToUse(EWeaponAnimationType NewWeaponAnimationType);

	/** [server + client] Set the yaw for player's movement direction relative to player's forward direction */
	void SetBlockMovementDirectionYaw(float NewYaw);

	/** [server + client] Change player's weapon sheath state */
	void SetWeaponSheathed(bool bNewValue);

	/**
	 * Returns player controller rotation yaw in -180/180 range.
	 * @note the yaw obtained from Controller->GetControlRotation().Yaw is in 0/360 range, which may not be desirable
	 */
	UFUNCTION(BlueprintCallable, category = Rotation)
	float GetPlayerControlRotationYaw();

	/**
	 * Returns the expected rotation yaw of character based on current Axis Input.
	 * @warning Only call for locally controlled character otherwise it would lead to crash (intentional)
	 */
	UFUNCTION(BlueprintCallable, category = Rotation)
	float GetRotationYawFromAxisInput();

	/** [server] Handle melee collision */
	virtual void OnMeleeCollision(UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit);

	/** [server] Apply damage to a character */
	virtual void ApplyEODDamage(AEODCharacterBase* HitCharacter, FEODDamage& EODDamage) override;

	/** [server] Take damage from another character */
	virtual void TakeEODDamage(AEODCharacterBase* HitInstigator, FEODDamage& EODDamage) override;

	/** Called on an animation montage blending out to clean up, reset, or change any state variables */
	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted) override;

	/** Called on an animation montage ending to clean up, reset, or change any state variables */
	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) override;

	void UpdateEquippedWeaponAnimationReferences(const EWeaponType EquippedWeaponType);

	void UpdateCurrentWeaponAnimationType();

private:

	const int CameraZoomRate = 15;

	const int CameraArmMinimumLength = 50;

	const int CameraArmMaximumLength = 500;

	// bool bHasActiveiframes;

	// bool bIsBlockingDamage;

	TArray<FCombatEvent> EventsOnUsingSkill;

	TArray<FCombatEvent> EventsOnSuccessfulSkillAttack;

	UPROPERTY(Transient)
	APrimaryWeapon* PrimaryWeapon;

	UPROPERTY(Transient)
	ASecondaryWeapon* SecondaryWeapon;

	UPROPERTY(Category = Weapons, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName PrimaryWeaponID;

	UPROPERTY(Category = Weapons, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName SecondaryWeaponID;

	UPROPERTY(Transient)
	bool bBlockPressed;

	UPROPERTY(Transient)
	bool bNormalAttackPressed;

	/** Player HUD class reference */
	UPROPERTY(Transient)
	class UHUDWidget* HUDWidget;

	/** The blueprint widget class to use for player HUD */
	UPROPERTY(EditDefaultsOnly, Category = RequiredInfo)
	TSubclassOf<class UHUDWidget> BP_HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = RequiredInfo)
	ECharacterGender Gender;

	UPROPERTY(Category = Skills, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint8 MaxNumberOfSkills;

	/** Determines whether weapon is currently sheathed or not */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_WeaponSheathed)
	bool bWeaponSheathed;

	/** A reference to player anim instance */
	UPlayerAnimInstance* PlayerAnimInstance;

	/** Animation montage references for currently equipped weapon */
	FPlayerAnimationReferences* EquippedWeaponAnimationReferences;

	/** Animations references for sheathed weapon */
	FPlayerAnimationReferences* SheathedWeaponAnimationReferences;

	/** This indicates the base maximum value of player's normal movement speed without any status effects */
	const float BaseNormalMovementSpeed = 400;

	//~ @todo test special movement speed (current value has been set untested and set on a guess)

	/** This indicates the base maximum value of player's special movement speed without any status effects */
	const float BaseSpecialMovementSpeed = 600;

	/** This indicates the base maximum value of player's movement speed when it's blocking damage */
	const float BaseBlockMovementSpeed = 150;

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

	/** Sets the boolean used to enable block to true */
	void EnableBlock();
	
	/** Sets the boolean used to enable block to false */
	void DisableBlock();

	/** Event triggered when player presses the jump key */
	void OnJump();

	/** Interacts with NPC or an in-game interactable object */
	void OnInteract();

	/** Put or remove weapon inside sheath */
	void OnToggleSheathe();

	/** Display or hide character stats UI */
	void OnToggleCharacterStatsUI();

	/** Display or hide mouse cursor */
	void OnToggleMouseCursor();

	void OnNormalAttackKeyPressed();

	void OnNormalAttackKeyReleased();

	void UpdateIdleState(float DeltaTime);

	void UpdateMovement(float DeltaTime);

	void UpdateBlockState(float DeltaTime);

	void UpdateAutoRun(float DeltaTime);

	/** Enable or disable auto run */
	void OnToggleAutoRun();

	void EnableAutoRun();

	void DisableAutoRun();

	FPlayerAnimationReferences* GetActiveAnimationReferences() const;

	/** Called when player presses a skill key */
	template<uint32 SkillButtonIndex>
	void PressedSkillKey();

	/** Called when player releases a skill key */
	template<uint32 SkillButtonIndex>
	void ReleasedSkillKey();

public:

	/** The relative yaw of character's movement direction from the direction character is facing while blocking */
	UPROPERTY(Replicated)
	float BlockMovementDirectionYaw;

	/** Character movement direction for Idle-Walk-Run state */
	UPROPERTY(Replicated)
	ECharMovementDirection IWR_CharacterMovementDirection;

	/** Animations to use based on that is determined by equipped weapon */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentWeaponAnimationToUse)
	EWeaponAnimationType CurrentWeaponAnimationToUse;

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
