// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/PrimaryWeapon.h"
#include "Weapons/SecondaryWeapon.h"
#include "Player/EODCharacterBase.h"
#include "Components/SkillsComponent.h"

#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "PlayerCharacter.generated.h"

class UHUDWidget;
class UAudioComponent;
class UAnimMontage;
class UPlayerAnimInstance;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class USkillsComponent;


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
	UFUNCTION(BlueprintCallable, Category = "EOD Character")
	void SavePlayerState();

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

	//~ Skills component - manages both skill bar and skill tree of player
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkillsComponent* SkillsComponent;

	//~ Audio component
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AudioComponent;
	
	/** [Constructor Only] A helper function that creates and returns new armor skeletal mesh component */
	USkeletalMeshComponent* CreateNewArmorComponent(const FName Name, const FObjectInitializer& ObjectInitializer);
	
public:

	/** Determines if this character should be rotated toward DesiredSmoothRotationYaw */
	UPROPERTY(Transient)
	bool bRotateSmoothly;

	/** The yaw to which the character needs to be rotated smoothly */
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

	/** Returns true if character can use any skill at all */
	virtual bool CanUseAnySkill() const;

	/** Returns true if character can use a particular skill */
	virtual bool CanUseSkill(FSkillTableRow* Skill);

	/** Returns true if character can auto run */
	bool CanAutoRun() const;

	/** Returns true if player is on auto run */
	bool IsAutoRunning() const;

	/** Returns true if primary weapon is equipped */
	bool IsPrimaryWeaponEquippped() const;

	/** Returns true if secondary weapon is equipped */
	bool IsSecondaryWeaponEquipped() const;

	bool IsFastRunning() const;

	FORCEINLINE bool IsWeaponSheathed() const;

	/** Returns primary weapon actor */
	FORCEINLINE APrimaryWeapon* GetPrimaryWeapon() const;

	/** Returns secondary weapon actor */
	FORCEINLINE ASecondaryWeapon* GetSecondaryWeapon() const;

	/** Returns the weapon type of primary weapon */
	FORCEINLINE EWeaponType GetEquippedWeaponType() const;

	/** Returns HUD widget */
	FORCEINLINE UHUDWidget* GetHUDWidget() const;

	FORCEINLINE ECharacterGender GetCharacterGender() const;

	/** Returns HUD widget */
	UFUNCTION(BlueprintPure, Category = UI, meta = (DisplayName = "Get HUD Widget"))
	UHUDWidget* BP_GetHUDWidget() const;

	/** [server + client] Interrupt this character's current action */
	virtual bool Interrupt(const float BCAngle) override;

	/** [server + client] Flinch this character. This is nothing more than a visual feedback to getting attacked */
	virtual bool Flinch(const float BCAngle) override;

	/** [server + client] Applies stun to this character */
	virtual bool Stun(const float Duration) override;

	/** [client] Removes 'stun' crowd control effect from this character */
	virtual void EndStun() override;

	/** [server + client] Freeze this character */
	virtual bool Freeze(const float Duration) override;

	/** [client] Removes 'freeze' crowd control effect from this character */
	virtual void EndFreeze() override;

	/** [server + client] Knockdown this character */
	virtual bool Knockdown(const float Duration) override;

	/** [client] Removes 'knock-down' crowd control effect from this character */
	virtual void EndKnockdown() override;

	/** [server + client] Knockback this character */
	virtual bool Knockback(const float Duration, const FVector& ImpulseDirection) override;

	virtual void BlockAttack() override;

	/** Replace primary weapon with a new weapon */
	void SetCurrentPrimaryWeapon(const FName WeaponID);

	/** Replace secondary weapon with a new weapon */
	void SetCurrentSecondaryWeapon(const FName WeaponID);

	/** Removes primary weapon if it is currently equipped */
	void RemovePrimaryWeapon();

	/** Removes secondary weapon if it is currently equipped */
	void RemoveSecondaryWeapon();

	/** Plays stun animation */
	UFUNCTION(BlueprintImplementableEvent, Category = Animations)
	void PlayStunAnimation();

	/** Stops stun animation */
	UFUNCTION(BlueprintImplementableEvent, Category = Animations)
	void StopStunAnimation();

	/** Simulates the knock back effect */
	UFUNCTION(BlueprintImplementableEvent, Category = Motion)
	void PushPlayer(FVector ImpulseDirection); // @todo const parameter?

	/** [server + client] Change idle-walk-run direction of character */
	inline void SetIWRCharMovementDir(ECharMovementDirection NewDirection);

	/** [server + client] Set the yaw for player's movement direction relative to player's forward direction */
	void SetBlockMovementDirectionYaw(float NewYaw);

	/** [server + client] Change player's weapon sheath state */
	void SetWeaponSheathed(bool bNewValue);

	/** Event called when a new normal attack section starts playing */
	void OnNormalAttackSectionStart(FName SectionName);

	/**
	 * Returns player controller rotation yaw in -180/180 range.
	 * @note the yaw obtained from Controller->GetControlRotation().Yaw is in 0/360 range, which may not be desirable
	 */
	UFUNCTION(BlueprintCallable, category = "EOD Character")
	float GetPlayerControlRotationYaw();

	/**
	 * Returns the expected rotation yaw of character based on current Axis Input.
	 * @warning Only call for locally controlled character otherwise it would lead to crash (intentional)
	 */
	UFUNCTION(BlueprintCallable, category = "EOD Character")
	float GetRotationYawFromAxisInput();

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayStatusMessage(const FString& Message);

	/** Called on an animation montage blending out to clean up, reset, or change any state variables */
	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted) override;

	/** Called on an animation montage ending to clean up, reset, or change any state variables */
	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) override;

	virtual void TurnOnTargetSwitch() override;

	virtual void TurnOffTargetSwitch() override;

	FORCEINLINE void SetOffSmoothRotation(float DesiredYaw);

	FORCEINLINE void OnSkillGroupAddedToSkillBar(const FString& SkillGroup);

	FORCEINLINE void OnSkillGroupRemovedFromSkillBar(const FString& SkillGroup);

	inline void EnableFastRun();

	inline void DisableFastRun();

private:

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

	UPROPERTY(Transient)
	bool bSkillAllowsMovement;

	/** Player HUD class reference */
	UPROPERTY(Transient)
	UHUDWidget* HUDWidget;

	/** The blueprint widget class to use for player HUD */
	UPROPERTY(EditDefaultsOnly, Category = RequiredInfo)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	/** Player gender : determines the animations and armor meshes to use. */
	UPROPERTY(EditDefaultsOnly, Category = RequiredInfo)
	ECharacterGender Gender;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Skills, meta = (AllowPrivateAccess = "true"))
	uint8 MaxNumberOfSkills;

	/** Data table containing player animation references */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UDataTable* PlayerAnimationReferencesDataTable;

	/** Determines whether weapon is currently sheathed or not */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_WeaponSheathed)
	bool bWeaponSheathed;

	/** Animations for this player when it has a weapon equipped */
	FPlayerAnimationReferencesTableRow* UnequippedWeaponAnimationReferences;

	/** Animations for this player when it does not have a weapon equipped */
	FPlayerAnimationReferencesTableRow* EquippedWeaponAnimationReferences;

	TSharedPtr<FStreamableHandle> EquippedWeaponAnimationsStreamableHandle;

	TSharedPtr<FStreamableHandle> UnequippedWeaponAnimationsStreamableHandle;

	FPlayerAnimationReferencesTableRow* GetActiveAnimationReferences() const;

	FName GetAnimationReferencesRowID(EWeaponType WeaponType, ECharacterGender CharGender);

	TSharedPtr<FStreamableHandle> LoadAnimationReferences(FPlayerAnimationReferencesTableRow* AnimationReferences);

	void UnloadUnequippedWeaponAnimationReferences();

	void LoadUnequippedWeaponAnimationReferences();

	void UnloadEquippedWeaponAnimationReferences();

	void LoadEquippedWeaponAnimationReferences();

	/** This indicates the base maximum value of player's normal movement speed without any status effects */
	UPROPERTY(Category = Movement, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float BaseNormalMovementSpeed;

	//~ @todo test special movement speed (current value has been set untested and set on a guess)
	/** This indicates the base maximum value of player's special movement speed without any status effects */
	UPROPERTY(Category = Weapons, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float BaseSpecialMovementSpeed;

	/** This indicates the base maximum value of player's movement speed when it's blocking damage */
	UPROPERTY(Category = Weapons, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float BaseBlockMovementSpeed;

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
	inline void EnableBlock();

	/** Disable damage blocking */
	inline void DisableBlock();

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

	void OnPressedNormalAttack();

	void OnReleasedNormalAttack();

	void UpdateIdleState(float DeltaTime);

	void UpdateMovement(float DeltaTime);

	void UpdateBlockState(float DeltaTime);

	void UpdateFastMovementState(float DeltaTime);

	void UpdateAutoRun(float DeltaTime);

	/** Enable or disable auto run */
	void OnToggleAutoRun();

	inline void EnableAutoRun();

	inline void DisableAutoRun();

	FORCEINLINE void StopNormalAttacking();

	void DisableForwardPressed();

	void DisableBackwardPressed();

	FName GetNextNormalAttackSectionName(const FName& CurrentSection) const;

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

private:
	
	//~ Begin multiplayer code
	UFUNCTION()
	void OnRep_WeaponSheathed();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIWRCharMovementDir(ECharMovementDirection NewDirection);
	
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

inline void APlayerCharacter::EnableAutoRun()
{
	SetCharacterState(ECharacterState::AutoRun);
	SetUseControllerRotationYaw(true);
}

inline void APlayerCharacter::DisableAutoRun()
{
	// @todo put a test to check if the player is really auto running

	SetCharacterState(ECharacterState::IdleWalkRun);
	SetUseControllerRotationYaw(false);
}

FORCEINLINE bool APlayerCharacter::IsWeaponSheathed() const
{
	return bWeaponSheathed;
}

FORCEINLINE APrimaryWeapon* APlayerCharacter::GetPrimaryWeapon() const
{
	return PrimaryWeapon;
}

FORCEINLINE ASecondaryWeapon* APlayerCharacter::GetSecondaryWeapon() const
{
	return SecondaryWeapon;
}

FORCEINLINE EWeaponType APlayerCharacter::GetEquippedWeaponType() const
{
	return PrimaryWeapon->WeaponType;
}

FORCEINLINE UHUDWidget* APlayerCharacter::GetHUDWidget() const
{
	return HUDWidget;
}

FORCEINLINE ECharacterGender APlayerCharacter::GetCharacterGender() const
{
	return Gender;
}

FORCEINLINE void APlayerCharacter::SetOffSmoothRotation(float DesiredYaw)
{
	bRotateSmoothly = true;
	DesiredSmoothRotationYaw = DesiredYaw;
}

FORCEINLINE void APlayerCharacter::OnSkillGroupAddedToSkillBar(const FString& SkillGroup)
{
	SkillsComponent->OnSkillGroupAddedToSkillBar(SkillGroup);
}

FORCEINLINE void APlayerCharacter::OnSkillGroupRemovedFromSkillBar(const FString& SkillGroup)
{
	SkillsComponent->OnSkillGroupRemovedFromSkillBar(SkillGroup);
}

inline void APlayerCharacter::SetIWRCharMovementDir(ECharMovementDirection NewDirection)
{
	IWR_CharacterMovementDirection = NewDirection;

	if (Role < ROLE_Authority)
	{
		Server_SetIWRCharMovementDir(NewDirection);
	}
}

inline void APlayerCharacter::EnableBlock()
{
	if (IsAutoRunning())
	{
		DisableAutoRun();
	}

	if (IsNormalAttacking())
	{
		StopNormalAttacking();
	}

	SetCharacterState(ECharacterState::Blocking);
	SetUseControllerRotationYaw(true);
	SetWalkSpeed(BaseBlockMovementSpeed * GetStatsComponent()->GetMovementSpeedModifier());

	FTimerHandle TimerDelegate;
	GetWorld()->GetTimerManager().SetTimer(BlockTimerHandle, this, &APlayerCharacter::EnableDamageBlocking, DamageBlockTriggerDelay, false);
}

inline void APlayerCharacter::DisableBlock()
{
	SetUseControllerRotationYaw(false);
	SetCharacterState(ECharacterState::IdleWalkRun);
	DisableDamageBlocking();
}

inline void APlayerCharacter::EnableFastRun()
{
	if (IsIdleOrMoving())
	{
		SetCharacterState(ECharacterState::SpecialMovement);
	}
}

inline void APlayerCharacter::DisableFastRun()
{
	if (IsFastRunning())
	{
		SetCharacterState(ECharacterState::IdleWalkRun);
	}
}

