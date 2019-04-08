// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODPreprocessors.h"
#include "EODGlobalNames.h"
#include "EODLibrary.h"
#include "CharacterLibrary.h"
#include "StatusEffectBase.h"
#include "StatsComponentBase.h"
#include "CombatInterface.h"

#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EODCharacterBase.generated.h"

DECLARE_STATS_GROUP(TEXT("EOD"), STATGROUP_EOD, STATCAT_Advanced);

class ARideBase;
class UAnimMontage;
class USkillsComponent;
class UInputComponent;
class UCameraComponent;
class UActiveSkillBase;
class UCharacterStateBase;
class UStatusEffectBase;
class UGameplayEventBase;
class UStatsComponentBase;
class UGameplaySkillsComponent;
class UAudioComponent;
class AEODCharacterBase;

/** Delegate for when a character either enters or leaves combat */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatStateChangedMCDelegate, AEODCharacterBase*, Character);

/** 
 * Delegate for when a gameplay event occurs
 * 
 * Owner = The actor that owns the gameplay event
 * Instigator = The actor that triggered the gameplay event. e.g., if character dodged an enemy attack, then enemy triggered the dodge event
 * Target = The actor that is target for the gameplay event. e.e., the character that dodged the enemy attack
 * GameplayEvent = An event object containing information regarding gameplay event
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGameplayEventMCDelegate,
											  AActor*, Owner,
											  AActor*, Instigator,
											  AActor*, Target,
											  UGameplayEventBase*, GameplayEvent);

/** 
 * Delegate for when a new weapon is equipped by character
 *
 * @param FName					New weapon ID
 * @param FWeaponTableRow*		New Weapon Data
 * @param FName					Old Weapon ID
 * @param FWeaponTableRow*		Old Weapon Data
 */
DECLARE_MULTICAST_DELEGATE_FourParams(FOnWeaponChangedDelegate,
									  FName,
									  FWeaponTableRow*,
									  FName,
									  FWeaponTableRow*);


USTRUCT(BlueprintType)
struct EOD_API FCharacterStateInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly)
	uint8 SubStateIndex;

	/**
	 * This needs to be updated every time character changes state.
	 * This is done to force the replication of character state info even if the character immediately reverts back to original state.
	 */
	UPROPERTY(BlueprintReadOnly)
	uint8 NewReplicationIndex;

	FCharacterStateInfo()
	{
		CharacterState = ECharacterState::IdleWalkRun;
		SubStateIndex = 0;
		NewReplicationIndex = 0;
	}

	FCharacterStateInfo(ECharacterState State) : CharacterState(State)
	{
		SubStateIndex = 0;
	}

	FCharacterStateInfo(ECharacterState State, uint8 SSIndex) : CharacterState(State), SubStateIndex(SSIndex)
	{
	}

	bool operator==(const FCharacterStateInfo& OtherStateInfo)
	{
		return this->CharacterState == OtherStateInfo.CharacterState &&
			this->SubStateIndex == OtherStateInfo.SubStateIndex;
	}

	bool operator!=(const FCharacterStateInfo& OtherStateInfo)
	{
		return this->CharacterState != OtherStateInfo.CharacterState ||
			this->SubStateIndex != OtherStateInfo.SubStateIndex;
	}

	void operator=(const FCharacterStateInfo& OtherStateInfo)
	{
		this->CharacterState = OtherStateInfo.CharacterState;
		this->SubStateIndex = OtherStateInfo.SubStateIndex;
		this->NewReplicationIndex = OtherStateInfo.NewReplicationIndex;
	}

	FString ToString() const
	{
		FString String = FString("Character State: ") + EnumToString<ECharacterState>(FString("ECharacterState"), CharacterState, FString("Invalid Class")) +
			FString(", SubStateIndex: ") + FString::FromInt(SubStateIndex);
		return String;
	}
};

/**
 * An abstract base class to handle the behavior of in-game characters.
 * All in-game characters must inherit from this class.
 */
UCLASS(Abstract)
class EOD_API AEODCharacterBase : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	/** Sets default values for this character's properties */
	AEODCharacterBase(const FObjectInitializer& ObjectInitializer);

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostInitializeComponents() override;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Updates character state every frame */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Called when this Pawn is possessed. Only called on the server (or in standalone).
	 * @param NewController The controller possessing this pawn
	 */
	virtual void PossessedBy(AController* NewController) override;

	/** Called when our Controller no longer possesses us. Only called on the server (or in standalone). */
	virtual void UnPossessed() override;

	/** Called when the Pawn is being restarted (usually by being possessed by a Controller). Called on both server and owning client. */
	virtual void Restart() override;

public:

	// --------------------------------------
	//  Character States
	// --------------------------------------

	/** Returns true if character is idling */
	FORCEINLINE bool IsIdle() const;

	/** Returns true if character is moving around */
	FORCEINLINE bool IsMoving() const;

	/** Returns true if character is in idle-walk-run state */
	FORCEINLINE bool IsIdleOrMoving() const;

	/** Determines if the character is in dodge state. Used to trigger dodge animation */
	FORCEINLINE bool IsDodging() const;

	/** Determines if the character is dodging incoming damage */
	FORCEINLINE bool IsDodgingDamage() const;

	/** Returns true if character is currently casting a spell */
	FORCEINLINE bool IsCastingSpell() const;

	/** Returns true if character is using any skill */
	FORCEINLINE bool IsUsingAnySkill() const;

	/** Returns true if character has just been hit */
	FORCEINLINE bool HasBeenHit() const;

	FORCEINLINE bool IsWeaponSheathed() const;

	/**
	 * Detemines if the character is in block state. Used to trigger block animation.
	 * @note there is a slight delay between when the block animation is triggered and when the character actually starts blocking damage
	 */
	FORCEINLINE bool IsBlocking() const;

	/**
	 * Determines if the character is actively blocking any incoming damage
	 * @note there is a slight delay between when the block animation is triggered and when the character actually starts blocking damage
	 */
	FORCEINLINE bool IsBlockingDamage() const;

	/** Returns true if character is jumping */
	FORCEINLINE bool IsJumping() const;

	/** Returns true if character is using a normal attack */
	FORCEINLINE bool IsNormalAttacking() const;

	//~ @todo
	/** Returns true if character is using skill at SkillIndex */
	FORCEINLINE bool IsUsingSkill(FName SkillID) const;

	/** Returns true if character has just been hit */
	UFUNCTION(BlueprintPure, Category = CharacterStatus, meta = (DisplayName = "Has Been Hit"))
	bool BP_HasBeenHit() const;

	/** Returns true if character can dodge */
	virtual bool CanDodge() const;

	/** Start dodging */
	virtual void StartDodge();

	/** Cancel dodging */
	virtual void CancelDodge();

	/** Finish dodging */
	virtual void FinishDodge();

	/** Returns true if character can guard against incoming attacks */
	virtual bool CanGuardAgainstAttacks() const;

	/** Enter guard state */
	virtual void StartBlockingAttacks();

	/** Leave guard state */
	virtual void StopBlockingAttacks();
	
	/** Returns true if character can jump */
	virtual bool CanJump() const;

	/** Event called when the jump animation starts playing */
	virtual void OnJumpAnimationStart();

	/** Event called when the jump animation finishes playing */
	virtual void OnJumpAnimationFinish();

	/** Returns true if character can move */
	virtual bool CanMove() const;

	/** Reset character state to Idle-Walk-Run */
	virtual void ResetState();

	/** Returns true if character can use normal attack */
	virtual bool CanNormalAttack() const;

	/** Start normal attacks */
	virtual void StartNormalAttack();

	/** Cancel normal attacks */
	virtual void CancelNormalAttack();

	/** Finish normal attacks and reset back to Idle-Walk-Run */
	virtual void FinishNormalAttack();

	/** Trigger interaction with an NPC or an in-game interactive object */
	UFUNCTION(BlueprintCallable, Category = "Character Interaction")
	virtual void TriggerInteraction();

	UFUNCTION(BlueprintCallable, Category = "Character Interaction")
	virtual void StartInteraction();

	UFUNCTION(BlueprintCallable, Category = "Character Interaction")
	virtual void UpdateInteraction();

	UFUNCTION(BlueprintCallable, Category = "Character Interaction")
	virtual void StopInteraction();

	/** Put or remove weapon inside sheath */
	virtual void ToggleSheathe();

	virtual void StartWeaponSwitch();

	virtual void CancelWeaponSwitch();

	virtual void FinishWeaponSwitch();

	/** Plays BlockAttack animation on blocking an incoming attack */
	virtual void PlayAttackBlockedAnimation();

	UPROPERTY(ReplicatedUsing = OnRep_CharacterStateInfo)
	FCharacterStateInfo CharacterStateInfo;

protected:

	/** Timer handle to call FinishDodge() */
	FTimerHandle FinishDodgeTimerHandle;

	/** Timer handle to call FinishWeaponSwitch() */
	FTimerHandle FinishWeaponSwitchTimerHandle;

	/** Updates whether player controller is currently trying to move or not */
	inline void UpdatePCTryingToMove();

	/** Updates the direction character is walking in */
	inline void UpdateCharacterMovementDirection();

	/** Resets tick dependent data. Intended to be called at the beginning of tick function */
	virtual void ResetTickDependentData();

	/** Updates character rotation every frame */
	virtual void UpdateRotation(float DeltaTime);

	/** Updates character movement every frame */
	virtual void UpdateMovement(float DeltaTime);

	/** Updates character normal attck state every frame if the character wants to normal attack */
	virtual void UpdateNormalAttackState(float DeltaTime);

public:

	// --------------------------------------
	//  Combat
	// --------------------------------------

	/**
	 * [server + local]
	 * Enables immunity frames after a given Delay for a given Duration on the server copy of this character.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat System", meta = (DisplayName = "Trigger iFrames"))
	void TriggeriFrames(float Duration = 0.4f, float Delay = 0.f);

	/** Returns the type of weapon currently equipped by this character */
	virtual EWeaponType GetEquippedWeaponType() const { return EWeaponType::None; }

	/** Enables blocking of incoming attacks */
	UFUNCTION()
	void EnableDamageBlocking();

	/** Disables blocking of incoming attacks */
	UFUNCTION()
	void DisableDamageBlocking();

	/** Set whether character is engaged in combat or not */
	UFUNCTION(BlueprintCallable, Category = "Combat System")
	virtual void SetInCombat(const bool bValue) { bInCombat = bValue; };

	/** Returns true if character is engaged in combat */
	FORCEINLINE bool IsInCombat() const { return bInCombat; }

	/** Returns true if character is engaged in combat */
	UFUNCTION(BlueprintPure, Category = "Combat System", meta = (DisplayName = "Is In Combat"))
	bool BP_IsInCombat() const;

protected:

	/** Enables immunity frames for a given duration */
	UFUNCTION()
	void EnableiFrames(float Duration = 0.f);

	/** Disables immunity frames */
	UFUNCTION()
	void DisableiFrames();

	/** Enable damage blocking */
	inline void StartBlockingDamage(float Delay = 0.2f);

	/** Disable damage blocking */
	inline void StopBlockingDamage();

	/** [server + local] Sets whether this character's weapon is sheathed or not */
	inline void SetWeaponSheathed(bool bNewValue);

	FTimerHandle DodgeImmunityTimerHandle;

	FTimerHandle DamageBlockingTimerHandle;

	FTimerHandle CrowdControlTimerHandle;

	/** Determines whether character is currently engaged in combat or not */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Combat System")
	uint32 bInCombat : 1;

	/** Set this to true to enable God Mode */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat System")
	uint32 bGodMode : 1;

	/** Time in seconds after which iFrames are triggered after initiating dodge */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat System|Constants")
	float DodgeImmunityTriggerDelay;

	/** Time in seconds for which iFrames stay active during dodge */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat System|Constants")
	float DodgeImmunityDuration;

	/** Time in seconds by which the damage blocking is delayed after character enters block state */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat System|Constants")
	float DamageBlockTriggerDelay;

private:

	/** Determines if invincibility frames are active */
	UPROPERTY(Transient)
	uint32 bActiveiFrames : 1;

	/** Determines if character is blocking any incoming damage */
	UPROPERTY(Transient)
	uint32 bBlockingDamage : 1;

	/** Determines whether weapon is currently sheathed or not */
	UPROPERTY(ReplicatedUsing = OnRep_WeaponSheathed)
	bool bWeaponSheathed;

public:

	// --------------------------------------
	//  Combat Interface
	// --------------------------------------

	virtual TSharedPtr<FAttackInfo> GetAttackInfoPtr() const override;

	virtual void SetAttackInfoFromActiveSkill(UActiveSkillBase* ActiveSkill) override;

	virtual void ResetAttackInfo() override;

	/** [server] Receive an attack on server */
	virtual TSharedPtr<FAttackResponse> ReceiveAttack(
		AActor* HitInstigator,
		ICombatInterface* InstigatorCI,
		const TSharedPtr<FAttackInfo>& AttackInfoPtr,
		const FHitResult& DirectHitResult,
		const bool bLineHitResultFound,
		const FHitResult& LineHitResult) override;

	/** Returns the actual damage received by this character */
	virtual float GetActualDamage(
		AActor* HitInstigator,
		ICombatInterface* InstigatorCI,
		const TSharedPtr<FAttackInfo>& AttackInfoPtr,
		const bool bCritHit,
		const bool bAttackBlocked) override;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_LastReceivedHit)
	FReceivedHitInfo LastReceivedHit;

	TSharedPtr<FAttackInfo> CurrentAttackInfoPtr;

public:

	// --------------------------------------
	//  Movement and Rotation
	// --------------------------------------

	FORCEINLINE ECharMovementDirection GetCharacterMovementDirection() const { return CharacterMovementDirection; }

	FORCEINLINE float GetBlockMovementDirectionYaw() const { return BlockMovementDirectionYaw; }

	FORCEINLINE bool IsRunning() const { return bIsRunning; }

	FORCEINLINE bool IsPCTryingToMove() const { return bPCTryingToMove; }

	/** [server + local] Change character max walk speed */
	inline void SetWalkSpeed(const float WalkSpeed);

	/** [Server + local] Set whether character should use controller rotation yaw or not */
	inline void SetUseControllerRotationYaw(const bool bNewBool);

	/** Move character forward/backward */
	virtual void MoveForward(const float Value);

	/** Move character left/right */
	virtual void MoveRight(const float Value);

	/** [server + local] Sets whether current character state allows movement */
	inline void SetCharacterStateAllowsMovement(bool bNewValue);

	/** [local] Sets whether current character state allows movement */
	inline void SetCharacterStateAllowsMovement_Local(bool bNewValue);

	/** [local] Sets whether current character allows rotation */
	inline void SetCharacterStateAllowsRotation(bool bValue);

	/**
	 * [server + local] Set character rotation yaw over network
	 * @note Do not use this for consecutive rotation change
	 */
	inline void SetCharacterRotationYaw(const float NewRotationYaw);

	/**
	 * [server + local] Set character rotation over network
	 * @note Do not use this for consecutive rotation change
	 */
	inline void SetCharacterRotation(const FRotator NewRotation);

	/** [server + client] Change character rotation. Do not use this for consecutive rotation change */
	UFUNCTION(BlueprintCallable, Category = "Rotation", meta = (DisplayName = "Set Character Rotation"))
	void BP_SetCharacterRotation(const FRotator NewRotation);

	/** [server + client] Set whether character should use controller rotation yaw or not */
	UFUNCTION(BlueprintCallable, Category = "Rotation", meta = (DisplayName = "Set Use Controller Rotation Yaw"))
	void BP_SetUseControllerRotationYaw(const bool bNewBool);

	/** [server + local] Change character max walk speed */
	UFUNCTION(BlueprintCallable, Category = "Movement", meta = (DisplayName = "Set Walk Speed"))
	void BP_SetWalkSpeed(const float WalkSpeed);

protected:

	/** [server + local] Sets the current character movement direction */
	inline void SetCharacterMovementDirection(ECharMovementDirection NewDirection);

	/** [server + local] Set the yaw for player's movement direction relative to player's forward direction */
	inline void SetBlockMovementDirectionYaw(float NewYaw);

	/** [server + local] Sets whether a player controller is currently trying to move this character or not */
	inline void SetPCTryingToMove(bool bNewValue);

	/** [server + local] Sets whether this character is running or not */
	inline void SetIsRunning(bool bNewValue);
	
	//~ @todo see if it's possible to use bCharacterStateAllowsMovement without needing replication
	/** This boolean is used to determine if the character can move even if it's not in 'IdleWalkRun' state. e.g., moving while casting spell. */
	UPROPERTY(Transient, Replicated)
	uint32 bCharacterStateAllowsMovement : 1;

	UPROPERTY(Transient)
	uint32 bCharacterStateAllowsRotation : 1;

	UPROPERTY(Replicated)
	float MovementSpeedModifier;

	/** Max speed of character when it's walking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Constants")
	float DefaultWalkSpeed;

	/** Max speed of character when it's running */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Constants")
	float DefaultRunSpeed;

	/** Max speed of character when it's moving while blocking attacks */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Constants")
	float DefaultWalkSpeedWhileBlocking;

private:

	/**
	 * The direction character is trying to move relative to it's controller rotation
	 * If the character is controlled by player, it is determined by the movement keys pressed by player
	 */
	UPROPERTY(Replicated)
	ECharMovementDirection CharacterMovementDirection;

	/** The relative yaw of character's movement direction from the direction character is facing while blocking */
	UPROPERTY(Replicated)
	float BlockMovementDirectionYaw;

	UPROPERTY(Replicated)
	uint32 bIsRunning : 1;

	/**
	 * This boolean determines whether player is trying to move or not
	 * i.e., if this character is possessed by a player controller, is player pressing the movement keys
	 */
	UPROPERTY(Replicated)
	uint32 bPCTryingToMove : 1;

public:

	// --------------------------------------
	//  Input Handling
	// --------------------------------------

	/** Cached value of player's forward axis input */
	UPROPERTY()
	float ForwardAxisValue;

	/** Cached value of player's right axis input */
	UPROPERTY()
	float RightAxisValue;

	/** Sets whether character wants to guard or not */
	FORCEINLINE void SetWantsToGuard(bool bNewValue) { bWantsToGuard = bNewValue; }

	FORCEINLINE	void SetWantsToNormalAttack(bool bNewValue) { bWantsToNormalAttack = bNewValue; }

	/** Returns the yaw that this pawn wants to rotate to based on the movement input from player */
	UFUNCTION(BlueprintCallable, Category = "Input|Rotation", meta = (DisplayName = "Get Rotation Yaw From Axis Input"))
	float BP_GetRotationYawFromAxisInput();

	/** Returns the yaw that this pawn wants to rotate to based on the movement input from player */
	inline float GetRotationYawFromAxisInput();

	/** Updates the DesiredCustomRotationYaw in pawn's movement component to the DesiredRotationYawFromAxisInput */
	void InitiateRotationToYawFromAxisInput();

	/** Zoom in player camera */
	inline void ZoomInCamera();

	/** Zooms out player camera */
	inline void ZoomOutCamera();

	/** Event called when forward key is pressed */
	virtual void OnPressedForward();

	/** Event called when backward key is pressed */
	virtual void OnPressedBackward();

	/** Event called when forward key is released */
	virtual void OnReleasedForward();

	/** Event called when backward key is relased */
	virtual void OnReleasedBackward();

protected:

	/** Determines whether the cached value of DesiredRotationYawFromAxisInput was updated this frame */
	uint32 bDesiredRotationYawFromAxisInputUpdated : 1;

	/** Cached value of the yaw that this pawn wants to rotate to this frame based on the movement input from player */
	UPROPERTY(Transient)
	float DesiredRotationYawFromAxisInput;

	/** Determines whether the character wants to guard */
	UPROPERTY(Transient)
	uint32 bWantsToGuard : 1;

	/** Determines whether the character wants to normal attack */
	UPROPERTY(Transient)
	uint32 bWantsToNormalAttack : 1;

	/** Calculates and returns rotation yaw from axis input */
	inline float CalculateRotationYawFromAxisInput() const;

private:

	// --------------------------------------
	//  User Interface
	// --------------------------------------

	void BindUIDelegates();

	void UnbindUIDelegates();

public:

	// --------------------------------------
	//  Effects
	// --------------------------------------

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Effects")
	void CreateGhostTrail();
	virtual void CreateGhostTrail_Implementation();

	// --------------------------------------
	//  Utility
	// --------------------------------------

	/**
	 * Returns controller rotation yaw in -180/180 range.
	 * @note the yaw obtained from Controller->GetControlRotation().Yaw is in 0/360 range, which may not be desirable
	 */
	UFUNCTION(BlueprintCallable, Category = "Utility", meta = (DisplayName = "Get Controller Rotation Yaw"))
	float BP_GetControllerRotationYaw() const;

	UFUNCTION(BlueprintCallable, category = Rotation)
	float GetOrientationYawToActor(const AActor* TargetActor);

	/**
	 * Rotate a character toward desired yaw based on the rotation rate in a given delta time (Precision based)
	 * @param DesiredYaw 	The desired yaw of character in degrees
	 * @param DeltaTime 	The time between last and current tick
	 * @param Precision		Yaw difference in degrees that will be used to determine success condition
	 * @param RotationRate 	Rotation rate to use for yaw rotation in degrees
	 * @return 				True if character successfully rotates to DesiredYaw (CurrentYaw == DesiredYaw)
	 */
	UFUNCTION(BlueprintCallable, category = Rotation, meta = (DeprecatedFunction))
	bool DeltaRotateCharacterToDesiredYaw(float DesiredYaw, float DeltaTime, float Precision = 1e-3f, float RotationRate = 600.f);

	/** Displays status effect text on player screen */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Utility")
	void CreateAndDisplayTextOnPlayerScreen(const FString& Message, const FLinearColor& TextColor, const FVector& TextPosition);
	virtual void CreateAndDisplayTextOnPlayerScreen_Implementation(const FString& Message, const FLinearColor& TextColor, const FVector& TextPosition);

	/** Get the prefix string for player gender */
	inline FString GetGenderPrefix() const;

	/** Get the prefix string for equipped weapon type */
	inline FString GetEquippedWeaponPrefix() const;

	inline FString GetWeaponPrefix(EWeaponType WeaponType) const;

	/**
	 * Returns controller rotation yaw in -180/180 range.
	 * @note the yaw obtained from Controller->GetControlRotation().Yaw is in 0/360 range, which may not be desirable
	 */
	inline float GetControllerRotationYaw() const;

	/** [server] Display status effect message on player screen */
	inline void DisplayTextOnPlayerScreen(const FString& Message, const FLinearColor& TextColor, const FVector& TextPosition);

	/** [server + local] Plays an animation montage and changes character state over network */
	inline void NetPlayAnimMontage(UAnimMontage* MontageToPlay, FName SectionToPlay);

	/** [server + client] Set the next montage section to play for a given animation montage */
	inline void SetNextMontageSection(FName CurrentSection, FName NextSection, UAnimMontage* Montage = nullptr);

	/** Called when an animation montage is blending out out to clean up, reset, or change any state variables */
	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	/** Called when an animation montage is ending to clean up, reset, or change any state variables */
	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	// --------------------------------------
	//  Save/Load System
	// --------------------------------------

	/** Saves current character state */
	UFUNCTION(BlueprintCallable, Category = "Save/Load System")
	virtual void SaveCharacterState() { ; }

	// --------------------------------------
	//  Ride System
	// --------------------------------------

	/** Reference to the rideable character that this character may be currently riding */
	UPROPERTY(Replicated)
	ARideBase* CurrentRide;

	/** Spawns a rideable character and mounts this character on the rideable character */
	UFUNCTION(BlueprintCallable, Category = "Ride System")
	void SpawnAndMountRideableCharacter(TSubclassOf<ARideBase> RideCharacterClass);

	/** Called when this character successfully mounts a rideable character */
	void OnMountingRide(ARideBase* RideCharacter);

	// --------------------------------------
	//  Components
	// --------------------------------------

	FORCEINLINE USpringArmComponent* GetCameraBoomComponent() const { return CameraBoomComponent; }

	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }

	FORCEINLINE UGameplaySkillsComponent* GetGameplaySkillsComponent() const { return SkillManager; }

	static const FName CameraComponentName;

	static const FName SpringArmComponentName;

	static const FName GameplaySkillsComponentName;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Constants")
	int32 CameraZoomRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Constants")
	int CameraArmMinimumLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Constants")
	int CameraArmMaximumLength;

	/** Audio component for playing hit effect sounds */
	UPROPERTY(Transient)
	UAudioComponent* HitAudioComponent;

private:

	/** Spring arm for camera */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoomComponent;

	/** Camera */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	//~ Skill bar component - manages skill bar (for player controlled character) and skills of character
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGameplaySkillsComponent* SkillManager;

public:

	// --------------------------------------
	//	Pseudo Constants : Variables that aren't supposed to be modified post creation
	// --------------------------------------

	/** Returns character faction */
	FORCEINLINE EFaction GetFaction() const;

	/** In game faction of your character */
	UPROPERTY(EditDefaultsOnly, Category = RequiredInfo)
	EFaction Faction;

	/** Player gender : determines the animations and armor meshes to use. */
	UPROPERTY(EditDefaultsOnly, Category = RequiredInfo)
	ECharacterGender Gender;

	// --------------------------------------
	//  Gameplay
	// --------------------------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	FGameplayTagContainer GameplayTagContainer;

	/** Returns true if character is alive */
	UFUNCTION(BlueprintPure, Category = "Gameplay")
	virtual bool IsAlive() const;

	/** Returns true if character is dead */
	UFUNCTION(BlueprintPure, Category = "Gameplay")
	virtual bool IsDead() const;

	/** Returns true if character can respawn */
	UFUNCTION(BlueprintPure, Category = "Gameplay")
	virtual bool CanRespawn() const;

	/** Returns true if this character requires healing (low on HP) */
	UFUNCTION(BlueprintPure, Category = "Gameplay")
	virtual bool NeedsHealing() const;

	/** Returns true if this character is healing anyone */
	UFUNCTION(BlueprintPure, Category = "Gameplay")
	virtual bool IsHealing() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void InitiateDeathSequence();
	virtual void InitiateDeathSequence_Implementation();

	/**
	 * Kills this character
	 * @param CauseOfDeath - The reason for death of this character
	 * @param Instigator - The character that instigated the death of this character (if any)
	 */
	virtual void Die(ECauseOfDeath CauseOfDeath, AActor* EventInstigator = nullptr, AController* EventOwner = nullptr);

	// --------------------------------------
	//  Skill System
	// --------------------------------------

	/** Returns the ID of skill that character is currently using. Returns NAME_None if character is not using any skill */
	FORCEINLINE FName GetCurrentActiveSkillID() const;

	/** Returns the ID of skill that character is using currently. Returns NAME_None if character is not using any skill */
	UFUNCTION(BlueprintPure, Category = "Skill System", meta = (DisplayName = "Get Current Active Skill ID"))
	FName BP_GetCurrentActiveSkillID() const;

	//  Set the ID of the skill that is currently being used
	FORCEINLINE void SetCurrentActiveSkillID(FName SkillID);

	/** Returns the ID of skill that character is using currently. Returns NAME_None if character is not using any skill */
	UFUNCTION(BlueprintCallable, Category = "Skill System", meta = (DisplayName = "Set Current Active Skill ID"))
	void BP_SetCurrentActiveSkillID(FName SkillID);

	/** Returns the skill that character is currently using. Returns nullptr if character is not using any skill */
	FORCEINLINE UGameplaySkillBase* GetCurrentActiveSkill() const;

	FORCEINLINE void SetCurrentActiveSkill(UGameplaySkillBase* Skill);

	/** Returns true if character can use any skill at all */
	UFUNCTION(BlueprintPure, Category = "Skill System")
	virtual bool CanUseAnySkill() const;

	UFUNCTION(BlueprintPure, Category = "Skill System")
	virtual bool CanUseSkill(FName SkillID, UGameplaySkillBase* Skill = nullptr);

	/** Returns the last used skill */
	FORCEINLINE FLastUsedSkillInfo GetLastUsedSkill();

	/** Returns the last used skill */
	UFUNCTION(BlueprintPure, Category = "Skill System", meta = (DisplayName = "Get Last Used Skill"))
	FLastUsedSkillInfo BP_GetLastUsedSkill();

	UFUNCTION(BlueprintPure, Category = "Skill System")
	UGameplaySkillBase* GetSkill(FName SkillID) const;

	/** [AI] Returns the melee attack skill that is more appropriate to use in current state against the given enemy */
	UFUNCTION(BlueprintPure, Category = "Skill System")
	virtual FName GetMostWeightedMeleeSkillID(const AEODCharacterBase* TargetCharacter) const;

	/**
	 * Determines and returns the status of a skill
	 * Returns EEODTaskStatus::Active if character is currently using the skill
	 * Returns EEODTaskStatus::Finished if character has finished using the skill
	 * Returns EEODTaskStatus::Aborted if the skill was aborted before completion
	 * Returns EEODTaskStatus::Inactive if the character is using or have used a different skill
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill System")
	virtual EEODTaskStatus CheckSkillStatus(FName SkillID);

	/**
	 * Use a skill and play it's animation
	 * This method is primarily intended to be used by AI characters
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill System")
	bool UseSkill(FName SkillID, UGameplaySkillBase* Skill = nullptr);
	virtual bool UseSkill_Implementation(FName SkillID, UGameplaySkillBase* Skill = nullptr);

private:

	/** SkillID of skill that this character is currently using */
	UPROPERTY()
	FName CurrentActiveSkillID;

	/** The skill that this character is currently using */
	UPROPERTY(Transient)
	UGameplaySkillBase* CurrentActiveSkill;

	/** Information of last used skill */
	UPROPERTY()
	FLastUsedSkillInfo LastUsedSkillInfo;

public:

	// --------------------------------------
	//  Status Effects
	// --------------------------------------

	/**
	 * Applies status effect on the character
	 * Handles activation of particle effects and sounds of the status effect (e.g. burning)
	 */
	virtual void ApplyStatusEffect(const UStatusEffectBase* StatusEffect);

	/**
	 * Removes status effect from the character
	 * Handles deactivation of particle effects and sounds of the status effect (e.g. burning)
	 */
	virtual void RemoveStatusEffect(const UStatusEffectBase* StatusEffect);

	// --------------------------------------
	//  Crowd Control Effects
	// --------------------------------------

	/** Returns true if character can flinch */
	virtual bool CanFlinch() const;

	/** Returns true if character can stun */
	virtual bool CanStun() const;

	/** Returns true if character can get knocked down */
	virtual bool CanKnockdown() const;

	/** Returns true if character can get knocked back */
	virtual bool CanKnockback() const;

	/** Returns true if character can be frozed/crystalized */
	virtual bool CanFreeze() const;

	/** Returns true if character can be interrupted */
	virtual bool CanInterrupt() const;

	/** Flinch this character (visual feedback) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Flinch"))
	bool CCEFlinch(const float BCAngle);
	virtual bool CCEFlinch_Implementation(const float BCAngle);

	/** Interrupt this character's current action */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Interrupt"))
	bool CCEInterrupt(const float BCAngle);
	virtual bool CCEInterrupt_Implementation(const float BCAngle);

	/** Applies stun to this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Stun"))
	bool CCEStun(const float Duration);
	virtual bool CCEStun_Implementation(const float Duration);

	/** Removes 'stun' crowd control effect from this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Remove Stun"))
	void CCERemoveStun();
	virtual void CCERemoveStun_Implementation();

	/** Freeze this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Freeze"))
	bool CCEFreeze(const float Duration);
	virtual bool CCEFreeze_Implementation(const float Duration);

	/** Removes 'freeze' crowd control effect from this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Unfreeze"))
	void CCEUnfreeze();
	virtual void CCEUnfreeze_Implementation();

	/** Knockdown this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Knockdown"))
	bool CCEKnockdown(const float Duration);
	virtual bool CCEKnockdown_Implementation(const float Duration);

	/** Removes 'knock-down' crowd control effect from this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE End Knockdown"))
	void CCEEndKnockdown();
	virtual void CCEEndKnockdown_Implementation();

	/** Knockback this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Knockback"))
	bool CCEKnockback(const float Duration, const FVector & ImpulseDirection);
	virtual bool CCEKnockback_Implementation(const float Duration, const FVector& ImpulseDirection);

	/** Plays stun animation */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CrowdControlEffect|Animations")
	void PlayStunAnimation();
	virtual void PlayStunAnimation_Implementation();

	/** Stops stun animation */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CrowdControlEffect|Animations")
	void StopStunAnimation();
	virtual void StopStunAnimation_Implementation();

	/** Simulates the knock back effect */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CrowdControlEffect|Movement")
	void PushBack(const FVector& ImpulseDirection);
	virtual void PushBack_Implementation(const FVector& ImpulseDirection);

	// --------------------------------------
	//  Gameplay Events
	// --------------------------------------

	FOnGameplayEventMCDelegate OnReceivingHit;

	FOnGameplayEventMCDelegate OnSuccessfulHit;

	FOnGameplayEventMCDelegate OnSuccessfulMagickAttack;

	FOnGameplayEventMCDelegate OnSuccessfulPhysicalAttack;

	FOnGameplayEventMCDelegate OnUnsuccessfulHit;

	FOnGameplayEventMCDelegate OnCriticalHit;

	FOnGameplayEventMCDelegate OnKillingEnemy;

	FOnGameplayEventMCDelegate OnFullHealth;

	FOnGameplayEventMCDelegate OnDamageAtFullHealth;

	FOnGameplayEventMCDelegate OnLowHealth;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gameplay Events")
	FOnCombatStateChangedMCDelegate OnInitiatingCombat;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gameplay Events")
	FOnCombatStateChangedMCDelegate OnLeavingCombat;

	/** Called on dodging an enemy attack */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gameplay Event")
	FOnGameplayEventMCDelegate OnDodgingAttack;

	/** Called on blocking an enemy attack */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gameplay Event")
	FOnGameplayEventMCDelegate OnBlockingAttack;

	/** Called on deflecting an enemy attack */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gameplay Event")
	FOnGameplayEventMCDelegate OnDeflectingAttack;

	// --------------------------------------
	//  Materials
	// --------------------------------------

	/** Temporarily trigger 'Target_Switch' material parameter to make the character glow */
	inline void SetOffTargetSwitch(float Duration = 0.1f);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	float TargetSwitchDuration;

	FTimerHandle TargetSwitchTimerHandle;

	virtual void TurnOnTargetSwitch();

	virtual void TurnOffTargetSwitch();

	// --------------------------------------
	//  Network
	// --------------------------------------

	UFUNCTION()
	virtual void OnRep_WeaponSheathed();

	UFUNCTION()
	virtual void OnRep_CharacterState(ECharacterState OldState);

	UFUNCTION()
	virtual void OnRep_CharacterStateInfo(const FCharacterStateInfo& OldStateInfo);

	UFUNCTION()
	virtual void OnRep_LastReceivedHit(const FReceivedHitInfo& OldHitInfo);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Dodge(uint8 DodgeIndex, float RotationYaw);
	virtual void Server_Dodge_Implementation(uint8 DodgeIndex, float RotationYaw);
	virtual bool Server_Dodge_Validate(uint8 DodgeIndex, float RotationYaw);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartBlockingAttacks();
	virtual void Server_StartBlockingAttacks_Implementation();
	virtual bool Server_StartBlockingAttacks_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopBlockingAttacks();
	virtual void Server_StopBlockingAttacks_Implementation();
	virtual bool Server_StopBlockingAttacks_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_NormalAttack(uint8 AttackIndex);
	virtual void Server_NormalAttack_Implementation(uint8 AttackIndex);
	virtual bool Server_NormalAttack_Validate(uint8 AttackIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SpawnAndMountRideableCharacter(TSubclassOf<ARideBase> RideCharacterClass);
	virtual void Server_SpawnAndMountRideableCharacter_Implementation(TSubclassOf<ARideBase> RideCharacterClass);
	virtual bool Server_SpawnAndMountRideableCharacter_Validate(TSubclassOf<ARideBase> RideCharacterClass);

	UFUNCTION(Client, Unreliable)
	void Client_DisplayTextOnPlayerScreen(const FString& Message, const FLinearColor& TextColor, const FVector& TextPosition);
	virtual void Client_DisplayTextOnPlayerScreen_Implementation(const FString& Message, const FLinearColor& TextColor, const FVector& TextPosition);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetBlockMovementDirectionYaw(float NewYaw);
	virtual void Server_SetBlockMovementDirectionYaw_Implementation(float NewYaw);
	virtual bool Server_SetBlockMovementDirectionYaw_Validate(float NewYaw);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartBlockingDamage(float Delay);
	virtual void Server_StartBlockingDamage_Implementation(float Delay);
	virtual bool Server_StartBlockingDamage_Validate(float Delay);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopBlockingDamage();
	virtual void Server_StopBlockingDamage_Implementation();
	virtual bool Server_StopBlockingDamage_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TriggeriFrames(float Duration, float Delay);
	virtual void Server_TriggeriFrames_Implementation(float Duration, float Delay);
	virtual bool Server_TriggeriFrames_Validate(float Duration, float Delay);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetWeaponSheathed(bool bNewValue);
	virtual void Server_SetWeaponSheathed_Implementation(bool bNewValue);
	virtual bool Server_SetWeaponSheathed_Validate(bool bNewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIsRunning(bool bValue);
	virtual void Server_SetIsRunning_Implementation(bool bValue);
	virtual bool Server_SetIsRunning_Validate(bool bValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharacterStateAllowsMovement(bool bNewValue);
	virtual void Server_SetCharacterStateAllowsMovement_Implementation(bool bNewValue);
	virtual bool Server_SetCharacterStateAllowsMovement_Validate(bool bNewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetPCTryingToMove(bool bNewValue);
	virtual void Server_SetPCTryingToMove_Implementation(bool bNewValue);
	virtual bool Server_SetPCTryingToMove_Validate(bool bNewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharMovementDir(ECharMovementDirection NewDirection);
	virtual void Server_SetCharMovementDir_Implementation(ECharMovementDirection NewDirection);
	virtual bool Server_SetCharMovementDir_Validate(ECharMovementDirection NewDirection);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetWalkSpeed(float WalkSpeed);
	virtual void Server_SetWalkSpeed_Implementation(float WalkSpeed);
	virtual bool Server_SetWalkSpeed_Validate(float WalkSpeed);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharacterRotation(FRotator NewRotation);
	virtual void Server_SetCharacterRotation_Implementation(FRotator NewRotation);
	virtual bool Server_SetCharacterRotation_Validate(FRotator NewRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharacterRotationYaw(float NewRotationYaw);
	virtual void Server_SetCharacterRotationYaw_Implementation(float NewRotationYaw);
	virtual bool Server_SetCharacterRotationYaw_Validate(float NewRotationYaw);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetUseControllerRotationYaw(bool bNewBool);
	virtual void Server_SetUseControllerRotationYaw_Implementation(bool bNewBool);
	virtual bool Server_SetUseControllerRotationYaw_Validate(bool bNewBool);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetNextMontageSection(FName CurrentSection, FName NextSection, UAnimMontage* Montage = nullptr);
	virtual void Server_SetNextMontageSection_Implementation(FName CurrentSection, FName NextSection, UAnimMontage* Montage = nullptr);
	virtual bool Server_SetNextMontageSection_Validate(FName CurrentSection, FName NextSection, UAnimMontage* Montage = nullptr);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetNextMontageSection(FName CurrentSection, FName NextSection, UAnimMontage* Montage = nullptr);
	virtual void Multicast_SetNextMontageSection_Implementation(FName CurrentSection, FName NextSection, UAnimMontage* Montage = nullptr);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayAnimMontage(UAnimMontage* MontageToPlay, FName SectionToPlay);
	virtual void Server_PlayAnimMontage_Implementation(UAnimMontage* MontageToPlay, FName SectionToPlay);
	virtual bool Server_PlayAnimMontage_Validate(UAnimMontage* MontageToPlay, FName SectionToPlay);

	UFUNCTION(NetMultiCast, Reliable)
	void Multicast_PlayAnimMontage(UAnimMontage* MontageToPlay, FName SectionToPlay);
	virtual void Multicast_PlayAnimMontage_Implementation(UAnimMontage* MontageToPlay, FName SectionToPlay);

	friend class AEODPlayerController;
	friend class UCharacterStateBase;
	
};

inline void AEODCharacterBase::StartBlockingDamage(float Delay)
{
	if (Role < ROLE_Authority)
	{
		Server_StartBlockingDamage(Delay);
	}
	else
	{
		UWorld* World = GetWorld();
		check(World);
		World->GetTimerManager().SetTimer(DamageBlockingTimerHandle, this, &AEODCharacterBase::EnableDamageBlocking, Delay, false);
	}
}

inline void AEODCharacterBase::StopBlockingDamage()
{
	if (Role < ROLE_Authority)
	{
		Server_StopBlockingDamage();
	}
	else
	{
		DisableDamageBlocking();
	}
}

inline void AEODCharacterBase::SetWeaponSheathed(bool bNewValue)
{
	bWeaponSheathed = bNewValue;
	if (Role < ROLE_Authority)
	{
		Server_SetWeaponSheathed(bNewValue);
	}
}

inline void AEODCharacterBase::SetWalkSpeed(const float WalkSpeed)
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	const float ErrorTolerance = 0.1f;
	if (MoveComp && !FMath::IsNearlyEqual(MoveComp->MaxWalkSpeed, WalkSpeed, ErrorTolerance))
	{
		MoveComp->MaxWalkSpeed = WalkSpeed;
		if (Role < ROLE_Authority)
		{
			Server_SetWalkSpeed(WalkSpeed);
		}
	}
}

inline void AEODCharacterBase::SetUseControllerRotationYaw(const bool bNewBool)
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp && MoveComp->bUseControllerDesiredRotation != bNewBool)
	{
		MoveComp->bUseControllerDesiredRotation = bNewBool;
		if (Role < ROLE_Authority)
		{
			Server_SetUseControllerRotationYaw(bNewBool);
		}
	}
}

inline void AEODCharacterBase::SetCharacterStateAllowsMovement(bool bNewValue)
{
	if (bCharacterStateAllowsMovement != bNewValue)
	{
		bCharacterStateAllowsMovement = bNewValue;
		if (Role < ROLE_Authority)
		{
			Server_SetCharacterStateAllowsMovement(bNewValue);
		}
	}
}

inline void AEODCharacterBase::SetCharacterStateAllowsMovement_Local(bool bNewValue)
{
	bCharacterStateAllowsMovement = bNewValue;
}

inline void AEODCharacterBase::SetCharacterStateAllowsRotation(bool bValue)
{
	bCharacterStateAllowsRotation = bValue;
}

inline void AEODCharacterBase::SetCharacterMovementDirection(ECharMovementDirection NewDirection)
{
	if (CharacterMovementDirection != NewDirection)
	{
		CharacterMovementDirection = NewDirection;
		if (Role < ROLE_Authority)
		{
			Server_SetCharMovementDir(NewDirection);
		}
	}
}

inline void AEODCharacterBase::SetBlockMovementDirectionYaw(float NewYaw)
{
	const float AngleTolerance = 1e-3f;
	if (!FMath::IsNearlyEqual(NewYaw, BlockMovementDirectionYaw, AngleTolerance))
	{
		BlockMovementDirectionYaw = NewYaw;
		if (Role < ROLE_Authority)
		{
			Server_SetBlockMovementDirectionYaw(NewYaw);
		}
	}
}

inline void AEODCharacterBase::SetPCTryingToMove(bool bNewValue)
{
	if (bPCTryingToMove != bNewValue)
	{
		bPCTryingToMove = bNewValue;
		if (Role < ROLE_Authority)
		{
			Server_SetPCTryingToMove(bNewValue);
		}
	}
}

inline void AEODCharacterBase::SetIsRunning(bool bNewValue)
{
	if (bIsRunning != bNewValue)
	{
		bIsRunning = bNewValue;
		if (Role < ROLE_Authority)
		{
			Server_SetIsRunning(bNewValue);
		}
	}
}

inline float AEODCharacterBase::GetControllerRotationYaw() const
{
	return (Controller ? FMath::UnwindDegrees(Controller->GetControlRotation().Yaw) : 0.0f);
}

inline void AEODCharacterBase::DisplayTextOnPlayerScreen(const FString& Message, const FLinearColor& TextColor, const FVector& TextPosition)
{
	Client_DisplayTextOnPlayerScreen(Message, TextColor, TextPosition);
}

inline void AEODCharacterBase::NetPlayAnimMontage(UAnimMontage* MontageToPlay, FName SectionToPlay)
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageToPlay);
		AnimInstance->Montage_JumpToSection(SectionToPlay, MontageToPlay);
	}
	Server_PlayAnimMontage(MontageToPlay, SectionToPlay);
}

inline void AEODCharacterBase::SetNextMontageSection(FName CurrentSection, FName NextSection, UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance)
	{
		AnimInstance->Montage_SetNextSection(CurrentSection, NextSection, Montage);
	}
	Server_SetNextMontageSection(CurrentSection, NextSection, Montage);
}

inline void AEODCharacterBase::UpdatePCTryingToMove()
{
	if (ForwardAxisValue == 0 && RightAxisValue == 0)
	{
		SetPCTryingToMove(false);
	}
	else
	{
		SetPCTryingToMove(true);
	}
}

inline void AEODCharacterBase::UpdateCharacterMovementDirection()
{
	if (ForwardAxisValue == 0 && RightAxisValue == 0)
	{
		if (CharacterMovementDirection != ECharMovementDirection::None)
		{
			SetCharacterMovementDirection(ECharMovementDirection::None);
		}
	}
	else
	{
		if (ForwardAxisValue == 0)
		{
			if (RightAxisValue > 0 && CharacterMovementDirection != ECharMovementDirection::R)
			{
				SetCharacterMovementDirection(ECharMovementDirection::R);
			}
			else if (RightAxisValue < 0 && CharacterMovementDirection != ECharMovementDirection::L)
			{
				SetCharacterMovementDirection(ECharMovementDirection::L);
			}
		}
		else
		{
			if (ForwardAxisValue > 0 && CharacterMovementDirection != ECharMovementDirection::F)
			{
				SetCharacterMovementDirection(ECharMovementDirection::F);
			}
			else if (ForwardAxisValue < 0 && CharacterMovementDirection != ECharMovementDirection::B)
			{
				SetCharacterMovementDirection(ECharMovementDirection::B);
			}
		}
	}
}

inline float AEODCharacterBase::GetRotationYawFromAxisInput()
{
	if (!bDesiredRotationYawFromAxisInputUpdated)
	{
		bDesiredRotationYawFromAxisInputUpdated = true;
		DesiredRotationYawFromAxisInput = CalculateRotationYawFromAxisInput();
	}

	return DesiredRotationYawFromAxisInput;
}

inline float AEODCharacterBase::CalculateRotationYawFromAxisInput() const
{
	float ResultingRotation = GetActorRotation().Yaw;
	float ControlRotationYaw = GetControllerRotationYaw();
	if (ForwardAxisValue == 0)
	{
		if (RightAxisValue > 0)
		{
			ResultingRotation = ControlRotationYaw + 90.f;
		}
		else if (RightAxisValue < 0)
		{
			ResultingRotation = ControlRotationYaw - 90.f;
		}
	}
	else
	{
		if (ForwardAxisValue > 0)
		{
			float DeltaAngle = FMath::RadiansToDegrees(FMath::Atan2(RightAxisValue, ForwardAxisValue));
			ResultingRotation = ControlRotationYaw + DeltaAngle;
		}
		else if (ForwardAxisValue < 0)
		{
			float DeltaAngle = FMath::RadiansToDegrees(FMath::Atan2(-RightAxisValue, -ForwardAxisValue));
			ResultingRotation = ControlRotationYaw + DeltaAngle;
		}
	}
	return FMath::UnwindDegrees(ResultingRotation);
}

inline void AEODCharacterBase::ZoomInCamera()
{
	if (CameraBoomComponent && CameraBoomComponent->TargetArmLength >= CameraArmMinimumLength)
	{
		CameraBoomComponent->TargetArmLength -= CameraZoomRate;
	}
}

inline void AEODCharacterBase::ZoomOutCamera()
{
	if (CameraBoomComponent && CameraBoomComponent->TargetArmLength <= CameraArmMaximumLength)
	{
		CameraBoomComponent->TargetArmLength += CameraZoomRate;
	}
}

FORCEINLINE bool AEODCharacterBase::IsIdle() const
{
	return (CharacterStateInfo.CharacterState == ECharacterState::IdleWalkRun && GetVelocity().Size() == 0);
}

FORCEINLINE bool AEODCharacterBase::IsMoving() const
{
	return (CharacterStateInfo.CharacterState == ECharacterState::IdleWalkRun && GetVelocity().Size() != 0);
}

FORCEINLINE bool AEODCharacterBase::IsIdleOrMoving() const
{
	return CharacterStateInfo.CharacterState == ECharacterState::IdleWalkRun;
}

FORCEINLINE bool AEODCharacterBase::IsJumping() const
{
	return CharacterStateInfo.CharacterState == ECharacterState::Jumping;
}

FORCEINLINE bool AEODCharacterBase::IsDodging() const
{
	return CharacterStateInfo.CharacterState == ECharacterState::Dodging;
}

FORCEINLINE bool AEODCharacterBase::IsDodgingDamage() const
{
	return bActiveiFrames;
}

FORCEINLINE bool AEODCharacterBase::IsBlocking() const
{
	return CharacterStateInfo.CharacterState == ECharacterState::Blocking;
}

FORCEINLINE bool AEODCharacterBase::IsBlockingDamage() const
{
	return bBlockingDamage;
}

FORCEINLINE bool AEODCharacterBase::IsCastingSpell() const
{
	return CharacterStateInfo.CharacterState == ECharacterState::CastingSpell;
}

FORCEINLINE bool AEODCharacterBase::IsNormalAttacking() const
{
	return CharacterStateInfo.CharacterState == ECharacterState::Attacking;
}

FORCEINLINE bool AEODCharacterBase::IsUsingAnySkill() const
{
	return CharacterStateInfo.CharacterState == ECharacterState::UsingActiveSkill;
}

FORCEINLINE bool AEODCharacterBase::IsUsingSkill(FName SkillID) const
{
	return CharacterStateInfo.CharacterState == ECharacterState::UsingActiveSkill;
}

FORCEINLINE bool AEODCharacterBase::HasBeenHit() const
{
	return CharacterStateInfo.CharacterState == ECharacterState::GotHit;
}

FORCEINLINE bool AEODCharacterBase::IsWeaponSheathed() const
{
	return bWeaponSheathed;
}

inline void AEODCharacterBase::SetOffTargetSwitch(float Duration)
{
	TurnOnTargetSwitch();
	UWorld* World = GetWorld();
	if (Duration > 0.f && World)
	{
		World->GetTimerManager().SetTimer(TargetSwitchTimerHandle, this, &AEODCharacterBase::TurnOffTargetSwitch, Duration, false);
	}
}

FORCEINLINE EFaction AEODCharacterBase::GetFaction() const
{
	return Faction;
}

FORCEINLINE FName AEODCharacterBase::GetCurrentActiveSkillID() const
{
	return CurrentActiveSkillID;
}

FORCEINLINE void AEODCharacterBase::SetCurrentActiveSkillID(FName SkillID)
{
	CurrentActiveSkillID = SkillID;
}

FORCEINLINE UGameplaySkillBase* AEODCharacterBase::GetCurrentActiveSkill() const
{
	return CurrentActiveSkill;
}

FORCEINLINE void AEODCharacterBase::SetCurrentActiveSkill(UGameplaySkillBase* Skill)
{
	CurrentActiveSkill = Skill;
}

FORCEINLINE FLastUsedSkillInfo AEODCharacterBase::GetLastUsedSkill()
{
	return LastUsedSkillInfo;
}

inline FString AEODCharacterBase::GetGenderPrefix() const
{
	return (Gender == ECharacterGender::Male) ? FString("M_") : FString("F_");
}

inline FString AEODCharacterBase::GetEquippedWeaponPrefix() const
{
	EWeaponType WepType = GetEquippedWeaponType();
	return GetWeaponPrefix(WepType);
}

inline FString AEODCharacterBase::GetWeaponPrefix(EWeaponType WeaponType) const
{
	switch (WeaponType)
	{
	case EWeaponType::GreatSword:
		return FString("GS_");
		break;
	case EWeaponType::WarHammer:
		return FString("WH_");
		break;
	case EWeaponType::LongSword:
		return FString("LS_");
		break;
	case EWeaponType::Mace:
		return FString("MC_");
		break;
	case EWeaponType::Dagger:
		return FString("DG_");
		break;
	case EWeaponType::Staff:
		return FString("ST_");
		break;
	case EWeaponType::Shield:
	case EWeaponType::None:
	default:
		return FString("");
		break;
	}
}

inline void AEODCharacterBase::SetCharacterRotationYaw(const float NewRotationYaw)
{
	const FRotator CurrentRotation = GetActorRotation();
	const float ErrorTolerance = 1e-3f;
	if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, NewRotationYaw, ErrorTolerance))
	{
		SetActorRotation(FRotator(CurrentRotation.Pitch, NewRotationYaw, CurrentRotation.Roll));
		if (Role < ROLE_Authority)
		{
			Server_SetCharacterRotationYaw(NewRotationYaw);
		}
	}
}

inline void AEODCharacterBase::SetCharacterRotation(const FRotator NewRotation)
{
	// @note Following line of code has been commented out intentionally and this function can no longer be used for consecutive rotation change.
	// GetCharacterMovement()->FlushServerMoves();

	const FRotator CurrentRotation = GetActorRotation();
	const float ErrorTolerance = 1e-3f;
	if (!CurrentRotation.Equals(NewRotation, ErrorTolerance))
	{
		SetActorRotation(NewRotation);
		if (Role < ROLE_Authority)
		{
			Server_SetCharacterRotation(NewRotation);
		}
	}
}
