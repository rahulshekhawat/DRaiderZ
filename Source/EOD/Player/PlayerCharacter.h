// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Player/EODCharacterBase.h"
#include "PlayerCharacter.generated.h"

class UHUDWidget;
class USkillBarWidget;
class UAnimMontage;
class APrimaryWeapon;
class ASecondaryWeapon;
class UPlayerAnimInstance;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;

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

	/** Saves current player state */
	FORCEINLINE void SavePlayerState();

	/** Saves current player state */
	UFUNCTION(BlueprintCallable, Category = SaveSystem, meta = (DisplayName = "Save Player State"))
	void BP_SavePlayerState();

private:
	
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCamera;

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
	UInventoryComponent* InventoryComponent;
	
	/** A helper function that must only be called from constructor. Creates and returns new armor skeletal mesh component */
	USkeletalMeshComponent* CreateNewArmorComponent(const FName Name, const FObjectInitializer& ObjectInitializer);
	
public:

	UPROPERTY(Transient)
	bool bRotateSmoothly;

	UPROPERTY(Transient)
	float DesiredSmoothRotationYaw;

	/** Returns true if character can move */
	virtual bool CanMove() const override;

	/** Returns true if character can jump */
	virtual bool CanJump() const override;

	/** Returns true if character can dodge */
	virtual bool CanDodge() const override;

	/** Returns true if character can block */
	virtual bool CanBlock() const override;

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

	FORCEINLINE APrimaryWeapon* GetPrimaryWeapon() const;

	FORCEINLINE ASecondaryWeapon* GetSecondaryWeapon() const;

	FORCEINLINE EWeaponType GetEquippedWeaponType() const;

	FORCEINLINE UHUDWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintPure, Category = UI, meta = (DisplayName = "Get HUD Widget"))
	UHUDWidget* BP_GetHUDWidget() const;

	/** [server + client] Interrupt this character's current action */
	virtual void Interrupt(const EHitDirection InterruptDirection) override;

	/** [server + client] Flinch this character. This is nothing more than a visual feedback to getting attacked */
	virtual void Flinch(const EHitDirection FlinchDirection) override;

	/** [server + client] Applies stun to this character */
	virtual void Stun(const float Duration) override;

	/** [client] Removes 'stun' crowd control effect from this character */
	virtual void EndStun() override;

	/** [server + client] Freeze this character */
	virtual void Freeze(const float Duration) override;

	/** [client] Removes 'freeze' crowd control effect from this character */
	virtual void EndFreeze() override;

	/** [server + client] Knockdown this character */
	virtual void Knockdown(const float Duration) override;

	/** [client] Removes 'knock-down' crowd control effect from this character */
	virtual void EndKnockdown() override;

	/** [server + client] Knockback this character */
	virtual void Knockback(const float Duration, const FVector& Impulse) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = Animations)
	void PlayStunAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = Animations)
	void StopStunAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = Motion)
	void PushPlayer(FVector PushDirection);

	/** Replace primary weapon with a new weapon */
	void SetCurrentPrimaryWeapon(const FName WeaponID);

	/** Replace secondary weapon with a new weapon */
	void SetCurrentSecondaryWeapon(const FName WeaponID);

	/** Removes primary weapon if it is currently equipped */
	void RemovePrimaryWeapon();

	/** Removes secondary weapon if it is currently equipped */
	void RemoveSecondaryWeapon();

	// FORCEINLINE void OnSecondaryWeaponFailedToEquip(FName WeaponID, FWeaponTableRow * NewWeaponData);

	/** [server + client] Change idle-walk-run direction of character */
	void SetIWRCharMovementDir(ECharMovementDirection NewDirection);

	//~ DEPRECATED
	/** [server + client] Change current weapon animation to use */
	void SetCurrentWeaponAnimationToUse(EWeaponAnimationType NewWeaponAnimationType);

	/** [server + client] Set the yaw for player's movement direction relative to player's forward direction */
	void SetBlockMovementDirectionYaw(float NewYaw);

	/** [server + client] Change player's weapon sheath state */
	void SetWeaponSheathed(bool bNewValue);

	// virtual void SetCurrentActiveSkill(FName SkillID) override;

	void AddSkill(FName SkillID, uint8 SkillLevel);

	FORCEINLINE FPlayerSkillTableRow* GetSkill(FName SkillID);

	virtual void OnNormalAttackSectionStart(FName SectionName) override;

	void CleanupNormalAttackSectionToSkillMap();

	void UpdateNormalAttackSectionToSkillMap(EWeaponType NewWeaponType);

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
	// virtual void OnMeleeCollision(UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit);

	/** [server] Apply damage to a character */
	// virtual int32 ApplyEODDamage(FEODDamage& EODDamage) override;

	// virtual FEODDamageResult ApplyEODDamage(AEODCharacterBase* InstigatingChar, const FEODDamage& EODDamage, const FHitResult& CollisionHitResult) override;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayReceivedDamage(int32 DamageValue);

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayStatusMessage(const FString& Message);

	/** Called on an animation montage blending out to clean up, reset, or change any state variables */
	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted) override;

	/** Called on an animation montage ending to clean up, reset, or change any state variables */
	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) override;

	void UpdateEquippedWeaponAnimationReferences(const EWeaponType EquippedWeaponType);

	void UpdateCurrentWeaponAnimationType();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Constants)
	int StaminaCost_Dodge;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Constants)
	float Dodge_iFrameStartTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Constants)
	float Dodge_iFrameEndTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Constants)
	float BlockDelay;

private:

	/** Data table for player skills */
	UPROPERTY(EditDefaultsOnly, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* DataTable_Skills;

	const int CameraZoomRate = 15;

	const int CameraArmMinimumLength = 50;

	const int CameraArmMaximumLength = 500;

	TArray<FCombatEvent> EventsOnUsingSkill;

	TArray<FCombatEvent> EventsOnSuccessfulSkillAttack;

	/** Timer handle needed for executing SP normal attacks */
	FTimerHandle SPAttackTimerHandle;

	UPROPERTY(Transient)
	APrimaryWeapon* PrimaryWeapon;

	UPROPERTY(Transient)
	ASecondaryWeapon* SecondaryWeapon;

	UPROPERTY(Category = Weapons, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName PrimaryWeaponID;

	UPROPERTY(Category = Weapons, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName SecondaryWeaponID;

	UPROPERTY(Transient)
	bool bForwardPressed;

	UPROPERTY(Transient)
	bool bBackwardPressed;

	UPROPERTY(Transient)
	bool bBlockPressed;

	UPROPERTY(Transient)
	bool bNormalAttackPressed;

	/** Player HUD class reference */
	UPROPERTY(Transient)
	UHUDWidget* HUDWidget;

	/** The blueprint widget class to use for player HUD */
	UPROPERTY(EditDefaultsOnly, Category = RequiredInfo)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

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

	void OnPressedForward();

	void OnPressedBackward();

	void OnReleasedForward();

	void OnReleasedBackward();

	/** Sets the boolean used to enable block to true */
	void OnPressedBlock();
	
	/** Sets the boolean used to enable block to false */
	void OnReleasedBlock();

	void OnPressedEscape();

	/** Enable damage blocking */
	FORCEINLINE void EnableBlock();

	/** Disable damage blocking */
	FORCEINLINE void DisableBlock();

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

	void OnToggleSkillTree();

	void OnPressedNormalAttack();

	void OnReleasedNormalAttack();

	void UpdateIdleState(float DeltaTime);

	void UpdateMovement(float DeltaTime);

	void UpdateBlockState(float DeltaTime);

	void UpdateAutoRun(float DeltaTime);

	/** Enable or disable auto run */
	void OnToggleAutoRun();

	FORCEINLINE void EnableAutoRun();

	FORCEINLINE void DisableAutoRun();

	void DisableForwardPressed();

	void DisableBackwardPressed();

	FPlayerAnimationReferences* GetActiveAnimationReferences() const;

	FName GetNextNormalAttackSectionName(const FName& CurrentSection) const;

	// EWeaponType GetCurrentEquippedWeaponType()

	void InitializeSkills(TArray<FName> UnlockedSKillsID);

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
	void OnRep_CurrentWeaponAnimationToUse(EWeaponAnimationType OldAnimationType);

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
