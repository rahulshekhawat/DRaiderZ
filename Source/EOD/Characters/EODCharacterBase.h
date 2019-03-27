// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/Statics/EODGlobalNames.h"
#include "EOD/Statics/EODLibrary.h"
#include "EOD/Statics/CharacterLibrary.h"
#include "EOD/StatusEffects/StatusEffectBase.h"
#include "EOD/Characters/Components/StatsComponentBase.h"

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
class UCharacterStateBase;
// class AEODPlayerController;
// class AEODAIControllerBase;
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
class EOD_API AEODCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	/** Sets default values for this character's properties */
	AEODCharacterBase(const FObjectInitializer& ObjectInitializer);

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

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

	UPROPERTY(ReplicatedUsing = OnRep_CharacterStateInfo)
	FCharacterStateInfo CharacterStateInfo;

	/** Returns true if character can dodge */
	virtual bool CanDodge() const;

	/** Start dodging */
	virtual void StartDodge();

	/** Cancel dodging */
	virtual void CancelDodge();

	/** Finish dodging */
	virtual void FinishDodge();

	FTimerHandle FinishDodgeTimerHandle;


	// --------------------------------------
	//	Combat
	// --------------------------------------

	/**
	 * [server + local]
	 * Enables immunity frames after a given Delay for a given Duration on the server copy of this character.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat System", meta = (DisplayName = "Trigger iFrames"))
	void TriggeriFrames(float Duration = 0.4f, float Delay = 0.f);

	/** Returns the type of weapon currently equipped by this character */
	virtual EWeaponType GetEquippedWeaponType() const { return EWeaponType::None; }

protected:

	/** Enables immunity frames for a given duration */
	UFUNCTION()
	void EnableiFrames(float Duration = 0.f);

	/** Disables immunity frames */
	UFUNCTION()
	void DisableiFrames();

	inline void ActivateGuard();

	inline void DeactivateGuard();

	/** [local] Transition to character guard animation */
	UFUNCTION()
	virtual void EnableCharacterGuard();

	/** [local] Transition out of character guard animation */
	UFUNCTION()
	virtual void DisableCharacterGuard();

	inline void StartBlockingDamage(float Delay = 0.2f);

	inline void StopBlockingDamage();

	/** Determines whether character is currently engaged in combat or not */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Combat System")
	bool bInCombat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat System|Constants")
	float DodgeImmunityTriggerDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat System|Constants")
	float DodgeImmunityDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat System|Constants")
	float DamageBlockTriggerDelay;

private:

	/** Determines if invincibility frames are active */
	UPROPERTY(Transient)
	uint32 bActiveiFrames : 1;

	/** Determines if character is blocking any incoming damage */
	UPROPERTY(Transient)
	uint32 bBlockingDamage : 1;

	/** Determines whether the character has it's guard up */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_GuardActive)
	uint32 bGuardActive : 1;

	/** Determines whether the character wants to guard right now */
	UPROPERTY(Transient)
	uint32 bWantsToGuard : 1;

protected:

	// --------------------------------------
	//	Character Stats
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Stats|Constants")
	int DodgeStaminaCost;

public:

	// --------------------------------------
	//	Movement
	// --------------------------------------

	FORCEINLINE ECharMovementDirection GetCharacterMovementDirection() const { return CharacterMovementDirection; }

	FORCEINLINE float GetBlockMovementDirectionYaw() const { return BlockMovementDirectionYaw; }

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

	/** [local] Sets whether current character allows rotation */
	inline void SetCharacterStateAllowsRotation(bool bValue);

protected:
	/** [server + local] Sets the current character movement direction */
	inline void SetCharacterMovementDirection(ECharMovementDirection NewDirection);

	/** [server + local] Set the yaw for player's movement direction relative to player's forward direction */
	inline void SetBlockMovementDirectionYaw(float NewYaw);

	//~ @todo see if it's possible to use bCharacterStateAllowsMovement without needing replication
	/** This boolean is used to determine if the character can move even if it's not in 'IdleWalkRun' state. e.g., moving while casting spell. */
	UPROPERTY(Transient, Replicated)
	uint32 bCharacterStateAllowsMovement : 1;

	UPROPERTY(Transient)
	uint32 bCharacterStateAllowsRotation : 1;

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

public:

	// --------------------------------------
	//	Input Handling
	// --------------------------------------

	/** Sets whether character wants to guard or not */
	FORCEINLINE void SetWantsToGuard(bool bNewValue) { bWantsToGuard = bNewValue; }

	/** Returns the yaw that this pawn wants to rotate to based on the movement input from player */
	UFUNCTION(BlueprintCallable, Category = "Input|Rotation", meta = (DisplayName = "Get Rotation Yaw From Axis Input"))
	float BP_GetRotationYawFromAxisInput();

	/** Returns the yaw that this pawn wants to rotate to based on the movement input from player */
	inline float GetRotationYawFromAxisInput();

	/** Updates the DesiredCustomRotationYaw in pawn's movement component to the DesiredRotationYawFromAxisInput */
	void InitiateRotationToYawFromAxisInput();

	virtual void Jump() override;

	/** Zoom in player camera */
	inline void ZoomInCamera();

	/** Zooms out player camera */
	inline void ZoomOutCamera();

protected:
	/** Determines whether the cached value of DesiredRotationYawFromAxisInput was updated this frame */
	uint32 bDesiredRotationYawFromAxisInputUpdated : 1;

	/** Cached value of the yaw that this pawn wants to rotate to this frame based on the movement input from player */
	UPROPERTY(Transient)
	float DesiredRotationYawFromAxisInput;

	//~ @todo DEPRECATED remove
	/** Determines whether guard key is pressed or not */
	// UPROPERTY(Transient)
	// uint32 bGuardKeyPressed : 1;

private:
	/** Calculates and returns rotation yaw from axis input */
	inline float CalculateRotationYawFromAxisInput() const;


public:

	// --------------------------------------
	//	User Interface
	// --------------------------------------

protected:

	// UPROPERTY(EditDefaultsOnly, Category = "UI")
		// TSubclassOf<USkillBa>

private:

	void BindUIDelegates();

	void UnbindUIDelegates();


	////////////////////////////////////////////////////////////////////////////////
	// Utility
public:
	/**
	 * Returns controller rotation yaw in -180/180 range.
	 * @note the yaw obtained from Controller->GetControlRotation().Yaw is in 0/360 range, which may not be desirable
	 */
	UFUNCTION(BlueprintCallable, Category = "Utility", meta = (DisplayName = "Get Controller Rotation Yaw"))
	float BP_GetControllerRotationYaw() const;

	/**
	 * Returns controller rotation yaw in -180/180 range.
	 * @note the yaw obtained from Controller->GetControlRotation().Yaw is in 0/360 range, which may not be desirable
	 */
	inline float GetControllerRotationYaw() const;

	/** [server + local] Plays an animation montage and changes character state over network */
	inline void PlayAnimationMontage(UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);


	////////////////////////////////////////////////////////////////////////////////
	// Save/load
public:
	/** Saves current character state */
	UFUNCTION(BlueprintCallable, Category = "EOD Character")
	virtual void SaveCharacterState() { ; }


	////////////////////////////////////////////////////////////////////////////////
	// Update character state
protected:
	/** Resets tick dependent data. Intended to be called at the beginning of tick function */
	virtual void ResetTickDependentData();

	/** Updates whether player controller is currently trying to move or not */
	inline void UpdatePCTryingToMove();

	/** Updates character rotation every frame */
	virtual void UpdateRotation(float DeltaTime);

	/** Updates character movement every frame */
	virtual void UpdateMovement(float DeltaTime);

	/** Updates character fall state every frame if the character is falling */
	virtual void UpdateFallState(float DeltaTime);

	/** Updates character normal attck state every frame if the character wants to normal attack */
	virtual void UpdateNormalAttackState(float DeltaTime);

	/** Updates character guard state every frame if the character wants to guard */
	virtual void UpdateGuardState(float DeltaTime);

	/** Updates character movement every frame */
	virtual void UpdateMovementState(float DeltaTime);


	////////////////////////////////////////////////////////////////////////////////
	// Ride System
public:
	/** Reference to the rideable character that this character may be currently riding */
	// UPROPERTY(ReplicatedUsing = OnRep_CurrentRide)
	UPROPERTY(Replicated)
	ARideBase* CurrentRide;

	/** Spawns a rideable character and mounts this character on the rideable character */
	UFUNCTION(BlueprintCallable, Category = "Ride System")
	void SpawnAndMountRideableCharacter(TSubclassOf<ARideBase> RideCharacterClass);

	/** Called when this character successfully mounts a rideable character */
	void OnMountingRide(ARideBase* RideCharacter);


	////////////////////////////////////////////////////////////////////////////////
	// Components
public:
	FORCEINLINE USpringArmComponent* GetCameraBoomComponent() const { return CameraBoomComponent; }

	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }

	FORCEINLINE UGameplaySkillsComponent* GetGameplaySkillsComponent() const { return SkillManager; }

	FORCEINLINE UStatsComponentBase* GetCharacterStatsComponent() const { return CharacterStatsComponent; }

	FORCEINLINE USphereComponent* GetInteractionSphereComponent() const { return InteractionSphereComponent; }

	inline void EnableInteractionSphere();

	inline void DisableInteractionSphere();

	static FName CameraComponentName;

	static FName SpringArmComponentName;

	static FName GameplaySkillsComponentName;

	static FName CharacterStatsComponentName;

	static FName InteractionSphereComponentName;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Constants")
	int32 CameraZoomRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Constants")
	int CameraArmMinimumLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Constants")
	int CameraArmMaximumLength;

private:
	/** Spring arm for camera */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoomComponent;

	/** Camera */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	/** StatsComp contains and manages the stats info of this character */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStatsComponentBase* CharacterStatsComponent;

	//~ Skill bar component - manages skill bar (for player controlled character) and skills of character
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGameplaySkillsComponent* SkillManager;

	/** Sphere component used to detect interactives objects */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* InteractionSphereComponent;

	/** Audio component for playing hit effect sounds */
	UPROPERTY(Transient)
	UAudioComponent* HitAudioComponent;


public:

	// --------------------------------------
	//	Character States
	// --------------------------------------

	/** Character state determines the current action character is doing */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CharacterState)
	ECharacterState CharacterState;

	// --------------------------------------
	//	Pseudo Constants : Variables that aren't supposed to be modified post creation
	// --------------------------------------

	/** Player gender : determines the animations and armor meshes to use. */
	UPROPERTY(EditDefaultsOnly, Category = RequiredInfo)
	ECharacterGender Gender;

	/** Get the prefix string for player gender */
	inline FString GetGenderPrefix() const;


	////////////////////////////////////////////////////////////////////////////////
	// ACTIONS
	////////////////////////////////////////////////////////////////////////////////
public:
	UFUNCTION(BlueprintCallable, Category = "EOD Character Actions")
	virtual void TriggerInteraction();

	UFUNCTION(BlueprintCallable, Category = "EOD Character Actions")
	virtual void StartInteraction();

	UFUNCTION(BlueprintCallable, Category = "EOD Character Actions")
	virtual void UpdateInteraction();

	UFUNCTION(BlueprintCallable, Category = "EOD Character Actions")
	virtual void StopInteraction();

	/** Put or remove weapon inside sheath */
	UFUNCTION(BlueprintCallable, Category = "EOD Character Actions")
	virtual void ToggleSheathe();
	
	virtual void StartNormalAttack();

	virtual void StopNormalAttack();

	virtual void PlayToggleSheatheAnimation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	FGameplayTagContainer GameplayTagContainer;

	////////////////////////////////////////////////////////////////////////////////
	// CHARACTER STATE
	////////////////////////////////////////////////////////////////////////////////
private:
	UPROPERTY(Replicated)
	bool bIsRunning;

	/**
	 * This boolean determines whether player is trying to move or not
	 * i.e., if this character is possessed by a player controller, is player pressing the movement keys
	 */
	UPROPERTY(Replicated)
	bool bPCTryingToMove;
	
	/** Determines whether weapon is currently sheathed or not */
	UPROPERTY(ReplicatedUsing = OnRep_WeaponSheathed)
	bool bWeaponSheathed;

protected:
	/** [server + local] Sets whether this character's weapon is sheathed or not */
	FORCEINLINE void SetWeaponSheathed(bool bNewValue)
	{
		bWeaponSheathed = bNewValue;
		PlayToggleSheatheAnimation();
		if (Role < ROLE_Authority)
		{
			Server_SetWeaponSheathed(bNewValue);
		}
	}

	/** [server + local] Sets whether this character is running or not */
	FORCEINLINE void SetIsRunning(bool bNewValue)
	{
		bIsRunning = bNewValue;
		if (Role < ROLE_Authority)
		{
			Server_SetIsRunning(bNewValue);
		}
	}

	/** [server + local] Sets whether a player controller is currently trying to move this character or not */
	FORCEINLINE void SetPCTryingToMove(bool bNewValue)
	{
		bPCTryingToMove = bNewValue;
		if (Role < ROLE_Authority)
		{
			Server_SetPCTryingToMove(bNewValue);
		}
	}

	/** [server + local] Sets whether this character has it's guard up against incoming attacks or not */
	FORCEINLINE void SetGuardActive(bool bNewValue)
	{
		bGuardActive = bNewValue;
		if (bGuardActive)
		{
			EnableCharacterGuard();
		}
		else
		{
			DisableCharacterGuard();
		}
		if (Role < ROLE_Authority)
		{
			Server_SetGuardActive(bNewValue);
		}
	}

	inline void UpdateCharacterMovementDirection();

public:
	FORCEINLINE bool IsWeaponSheathed() const { return bWeaponSheathed; }

	FORCEINLINE bool IsRunning() const { return bIsRunning; }

	FORCEINLINE bool IsPCTryingToMove() const { return bPCTryingToMove; }

	FORCEINLINE bool IsGuardActive() const { return bGuardActive; }


	////////////////////////////////////////////////////////////////////////////////
	// INPUT
	////////////////////////////////////////////////////////////////////////////////
private:
	UPROPERTY(Transient)
	bool bNormalAttackKeyPressed;

public:
	/** Cached value of player's forward axis input */
	UPROPERTY()
	float ForwardAxisValue;

	/** Cached value of player's right axis input */
	UPROPERTY()
	float RightAxisValue;

	FORCEINLINE void SetNormalAttackKeyPressed(bool bNewValue)
	{
		bNormalAttackKeyPressed = bNewValue;
	}

	FORCEINLINE bool IsNormalAttackKeyPressed()const { return bNormalAttackKeyPressed; }

	virtual void OnPressedForward();

	virtual void OnPressedBackward();

	virtual void OnReleasedForward();

	virtual void OnReleasedBackward();

public:
	/** Returns true if character is alive */
	FORCEINLINE bool IsAlive() const;

	/** Returns true if character is dead */
	FORCEINLINE bool IsDead() const;

	/** Returns true if character is dead */
	UFUNCTION(BlueprintPure, Category = CharacterStatus, meta = (DisplayName = "Is Dead"))
	bool BP_IsDead() const;
	
	/** Returns true if character is idling */
	FORCEINLINE bool IsIdle() const;

	/** Returns true if character is moving around */
	FORCEINLINE bool IsMoving() const;

	/** Returns true if character is in idle-walk-run state */
	FORCEINLINE bool IsIdleOrMoving() const;

	/** Returns true if character is jumping */
	FORCEINLINE bool IsJumping() const;

	/** Determines if the character is in dodge state. Used to trigger dodge animation */
	FORCEINLINE bool IsDodging() const;
	
	/** Determines if the character is dodging incoming damage */
	FORCEINLINE bool IsDodgingDamage() const;

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

	/** Returns true if character is currently casting a spell */
	FORCEINLINE bool IsCastingSpell() const;
	
	/** Returns true if character is using a normal attack */
	FORCEINLINE bool IsNormalAttacking() const;

	/** Returns true if character is using any skill */
	FORCEINLINE bool IsUsingAnySkill() const;

	/** Returns true if character is using skill at SkillIndex */
	FORCEINLINE bool IsUsingSkill(FName SkillID) const;

	/** Returns true if character has just been hit */
	FORCEINLINE bool HasBeenHit() const;

	/** Returns true if character has just been hit */
	UFUNCTION(BlueprintPure, Category = CharacterStatus, meta = (DisplayName = "Has Been Hit"))
	bool BP_HasBeenHit() const;

	/** Returns true if character can move */
	UFUNCTION(BlueprintCallable, Category = CharacterState)
	virtual bool CanMove() const;
	
	/** Returns true if character can jump */
	virtual bool CanJump() const;

	/** Returns true if character can guard against incoming attacks */
	virtual bool CanGuardAgainstAttacks() const;

	/** Returns true if character can block */
	virtual bool CanBlock() const;

	/** Returns true if character can respawn */
	virtual bool CanRespawn() const;

	/** Returns true if character can use normal attack */
	virtual bool CanNormalAttack() const;
	
	/** Returns true if character can use any skill at all */
	UFUNCTION(BlueprintCallable, Category = CharacterState)
	virtual bool CanUseAnySkill() const;

	/** Returns true if character can use a particular skill */
	virtual bool CanUseSkill(FSkillTableRow* Skill);
	
	/** Returns true if character can flinch */
	FORCEINLINE bool CanFlinch() const;

	/** Returns true if character can stun */
	FORCEINLINE bool CanStun() const;

	/** Returns true if character can get knocked down */
	FORCEINLINE bool CanKnockdown() const;

	/** Returns true if character can get knocked back */
	FORCEINLINE bool CanKnockback() const;

	/** Returns true if character can be frozed/crystalized */
	FORCEINLINE bool CanFreeze() const;

	/** Returns true if character can be interrupted */
	FORCEINLINE bool CanInterrupt() const;

	/** Returns true if this character requires healing (low on HP) */
	FORCEINLINE bool NeedsHealing() const;

	/** Returns true if this character requires healing (low on HP) */
	UFUNCTION(BlueprintPure, Category = CharacterStatus, meta = (DisplayName = "Needs Healing"))
	bool BP_NeedsHealing() const;

	/** Returns true if this character is healing anyone */
	virtual bool IsHealing() const;

	/** Flinch this character (visual feedback) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Flinch"))
	bool CCEFlinch(const float BCAngle);

	/** Flinch this character (visual feedback) */
	virtual bool CCEFlinch_Implementation(const float BCAngle);

	/** Interrupt this character's current action */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Interrupt"))
	bool CCEInterrupt(const float BCAngle);

	/** Interrupt this character's current action */
	virtual bool CCEInterrupt_Implementation(const float BCAngle);

	/** Applies stun to this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Stun"))
	bool CCEStun(const float Duration);

	/** Applies stun to this character */
	virtual bool CCEStun_Implementation(const float Duration);

	/** Removes 'stun' crowd control effect from this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Remove Stun"))
	void CCERemoveStun();

	/** Removes 'stun' crowd control effect from this character */
	virtual void CCERemoveStun_Implementation();

	/** Freeze this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Freeze"))
	bool CCEFreeze(const float Duration);

	/** Freeze this character */
	virtual bool CCEFreeze_Implementation(const float Duration);

	/** Removes 'freeze' crowd control effect from this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Unfreeze"))
	void CCEUnfreeze();

	/** Removes 'freeze' crowd control effect from this character */
	virtual void CCEUnfreeze_Implementation();

	/** Knockdown this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Knockdown"))
	bool CCEKnockdown(const float Duration);

	/** Knockdown this character */
	virtual bool CCEKnockdown_Implementation(const float Duration);

	/** Removes 'knock-down' crowd control effect from this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE End Knockdown"))
	void CCEEndKnockdown();

	/** Removes 'knock-down' crowd control effect from this character */
	virtual void CCEEndKnockdown_Implementation();

	/** Knockback this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect, meta = (DisplayName = "CCE Knockback"))
	bool CCEKnockback(const float Duration, const FVector& ImpulseDirection);

	/** Knockback this character */
	virtual bool CCEKnockback_Implementation(const float Duration, const FVector& ImpulseDirection);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = PlayerStatus)
	void InitiateDeathSequence();

	virtual void InitiateDeathSequence_Implementation();

	/** Applies stun to this character */
	virtual bool Stun(const float Duration);

	/** Removes 'stun' crowd control effect from this character */
	virtual void EndStun();

	/** Freeze this character */
	virtual bool Freeze(const float Duration);

	/** Removes 'freeze' crowd control effect from this character */
	virtual void EndFreeze();

	/** Knockdown this character */
	virtual bool Knockdown(const float Duration);

	/** Removes 'knock-down' crowd control effect from this character */
	virtual void EndKnockdown();

	/** Knockback this character */
	virtual bool Knockback(const float Duration, const FVector& ImpulseDirection);

	/** Plays BlockAttack animation on blocking an incoming attack */
	virtual void BlockAttack();

	/** Plays stun animation */
	UFUNCTION(BlueprintImplementableEvent, Category = "Animations|CrowdControlEffect")
	void PlayStunAnimation();

	/** Stops stun animation */
	UFUNCTION(BlueprintImplementableEvent, Category = "Animations|CrowdControlEffect")
	void StopStunAnimation();

	/** Simulates the knock back effect */
	UFUNCTION(BlueprintImplementableEvent, Category = "Motion|CrowdControlEffect")
	void PushBack(const FVector& ImpulseDirection); // @todo const parameter?

	/** Enables blocking of incoming attacks */
	UFUNCTION()
	void EnableDamageBlocking();

	/** Disables blocking of incoming attacks */
	UFUNCTION()
	void DisableDamageBlocking();

	/**
	 * Called on dodging an enemy attack
	 * @param AttackInstigator Enemy character whose incoming damage this character dodged
	 */
	// void OnSuccessfulDodge(AEODCharacterBase* AttackInstigator);
	// void SuccessfulDodge(AEODCharacterBase* AttackInstigator);
	// void DodgedAttack(AEODCharacterBase* AttackInstigator);

	/**
	 * Called on successfully blocking an enemy attack
	 * @param AttackInstigator Enemy character whose incoming damage this character blocked
	 */
	// void OnSuccessfulBlock(AEODCharacterBase* AttackInstigator);

	/**
	 * Called on getting an attack of this character blocked by an enemy
	 * @param AttackBlocker Enemy character that blocked this character's attack
	 */
	// void OnAttackDeflected(AEODCharacterBase* AttackBlocker, bool bSkillIgnoresBlock);

	/** Temporarily trigger 'Target_Switch' material parameter to make the character glow */
	FORCEINLINE void SetOffTargetSwitch();

	/** Set whether character is engaged in combat or not */
	UFUNCTION(BlueprintCallable, Category = "EOD Character")
	virtual void SetInCombat(const bool bValue) { bInCombat = bValue; };
	
	/** Returns true if character is engaged in combat */
	FORCEINLINE bool IsInCombat() const { return bInCombat; }

	UFUNCTION(BlueprintPure, Category = "EOD Character", meta = (DisplayName = "Is In Combat"))
	bool BP_IsInCombat() const;

	/** Sets current state of character */
	FORCEINLINE void SetCharacterState(const ECharacterState NewState)
	{
		CharacterState = NewState;
		// Character state is no longer replicated
		/*
		if (Role < ROLE_Authority)
		{
			Server_SetCharacterState(NewState);
		}
		*/
	}

	/** Sets current state of character */
	UFUNCTION(BlueprintCallable, Category = "EOD Character", meta = (DisplayName = "Set Character State"))
	void BP_SetCharacterState(const ECharacterState NewState);

	/** Get current state of character */
	FORCEINLINE ECharacterState GetCharacterState() const;

	UFUNCTION(BlueprintPure, Category = "EOD Character", meta = (DisplayName = "Get Character State"))
	ECharacterState BP_GetCharacterState() const;

	/** [server + local] Change character max walk speed */
	UFUNCTION(BlueprintCallable, Category = "EOD Character", meta = (DisplayName = "Set Walk Speed"))
	void BP_SetWalkSpeed(const float WalkSpeed);

	/**
	 * [server + local] Change character rotation.
	 * Note : Do not use this for consecutive rotation change 
	 */
	FORCEINLINE void SetCharacterRotation(const FRotator NewRotation)
	{
		// Following line of code has been commented out intentionally and this function can no longer be used for consecutive rotation change.
		// GetCharacterMovement()->FlushServerMoves();
		SetActorRotation(NewRotation);
		if (Role < ROLE_Authority)
		{
			Server_SetCharacterRotation(NewRotation);
		}
	}

	/** [server + client] Change character rotation. Do not use this for consecutive rotation change */
	UFUNCTION(BlueprintCallable, Category = "EOD Character", meta = (DisplayName = "Set Character Rotation"))
	void BP_SetCharacterRotation(const FRotator NewRotation);

	/** [server + client] Set whether character should use controller rotation yaw or not */
	UFUNCTION(BlueprintCallable, Category = "EOD Character", meta = (DisplayName = "Set Use Controller Rotation Yaw"))
	void BP_SetUseControllerRotationYaw(const bool bNewBool);

	/** Returns character faction */
	FORCEINLINE EFaction GetFaction() const;

	inline FSkillTableRow* GetSkill(FName SkillID, const FString& ContextString = FString("AEODCharacterBase::GetSkill(), character skill lookup")) const;

	/**
	 * Use a skill and play it's animation
	 * This method is primarily intended to be used by AI characters
	 */
	// UFUNCTION(BlueprintCallable, Category = Skills)
	// virtual bool UseSkill(FName SkillID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CrowdControlEffect)
	bool UseSkill(FName SkillID);

	virtual bool UseSkill_Implementation(FName SkillID);

	/**
	 * Determines and returns the status of a skill
	 * Returns EEODTaskStatus::Active if character is currently using the skill
	 * Returns EEODTaskStatus::Finished if character has finished using the skill
	 * Returns EEODTaskStatus::Aborted if the skill was aborted before completion
	 * Returns EEODTaskStatus::Inactive if the character is using or have used a different skill
	 */
	UFUNCTION(BlueprintCallable, Category = Skills)
	virtual EEODTaskStatus CheckSkillStatus(FName SkillID);

	/** [AI] Returns the melee attack skill that is more appropriate to use in current state against the given enemy */
	UFUNCTION(BlueprintCallable, Category = Skills)
	virtual FName GetMostWeightedMeleeSkillID(const AEODCharacterBase* TargetCharacter) const;

	/** Returns the ID of skill that character is currently using. Returns NAME_None if character is not using any skill */
	FORCEINLINE FName GetCurrentActiveSkillID() const;

	/** Returns the skill that character is currently using. Returns nullptr if character is not using any skill */
	FORCEINLINE FSkillTableRow* GetCurrentActiveSkill() const;

	/** Returns the ID of skill that character is using currently. Returns NAME_None if character is not using any skill */
	UFUNCTION(BlueprintPure, Category = Skills, meta = (DisplayName = "Get Current Active Skill ID"))
	FName BP_GetCurrentActiveSkillID() const;

	//  Set the ID of the skill that is currently being used
	FORCEINLINE void SetCurrentActiveSkillID(FName SkillID)
	{
		CurrentActiveSkillID = SkillID;
	}

	FORCEINLINE void SetCurrentActiveSkill(FSkillTableRow* Skill)
	{
		CurrentActiveSkill = Skill;
	}

	/** Returns the ID of skill that character is using currently. Returns NAME_None if character is not using any skill */
	UFUNCTION(BlueprintCallable, Category = Skills, meta = (DisplayName = "Set Current Active Skill ID"))
	void BP_SetCurrentActiveSkillID(FName SkillID);

	/** Returns the last used skill */
	FORCEINLINE FLastUsedSkillInfo& GetLastUsedSkill();

	/** Returns the last used skill */
	UFUNCTION(BlueprintPure, Category = Skills, meta = (DisplayName = "Get Last Used Skill"))
	FLastUsedSkillInfo& BP_GetLastUsedSkill();

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

	/** Called when an animation montage is blending out out to clean up, reset, or change any state variables */
	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	/** Called when an animation montage is ending to clean up, reset, or change any state variables */
	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	// Play an animation montage locally
	inline void PlayAnimationMontage(UAnimMontage* MontageToPlay, FName SectionToPlay);

	//~ @note UFUNCTIONs don't allow function overloading
	/** [server + client] Plays an animation montage and changes character state over network */
	UFUNCTION(BlueprintCallable, Category = Animations, meta = (DisplayName = "Play Animation Montage Over Network"))
	void BP_PlayAnimationMontage(UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);

	/** [server + client] Set the next montage section to play for a given animation montage */
	void SetNextMontageSection(FName CurrentSection, FName NextSection);
	// void SetNextMontageSection(UAnimMontage* Montage, FName CurrentSection, FName NextSection);

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

	/**
	 * Kills this character 
	 * @param CauseOfDeath - The reason for death of this character
	 * @param Instigator - The character that instigated the death of this character (if any)
	 */
	virtual void Die(ECauseOfDeath CauseOfDeath, AEODCharacterBase* InstigatingChar = nullptr);

	UFUNCTION(BlueprintCallable, category = Rotation)
	float GetOrientationYawToActor(AActor* TargetActor);

private:
	/** In game faction of your character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EOD Character", meta = (AllowPrivateAccess = "true"))
	EFaction Faction;

	/** SkillID of skill that this character is currently using */
	UPROPERTY(Transient)
	FName CurrentActiveSkillID;

	/** The skill that this character is currently using */
	FSkillTableRow* CurrentActiveSkill;

	/** Information of last used skill */
	UPROPERTY(Transient)
	FLastUsedSkillInfo LastUsedSkillInfo;

protected:

	UPROPERTY(Transient, BlueprintReadWrite)
	bool bSkillAllowsMovement;

	UPROPERTY(Transient, BlueprintReadWrite)
	bool bUsingUniqueSkill;

	UPROPERTY()
	FCharacterStateData CharacterStateData;

	/** Data table for character skills */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Skills)
	UDataTable* SkillsDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Rotation)
	float CharacterRotationPrecision;

	/** True if character is in God Mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOD Character")
	bool bGodMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOD Character")
	float TargetSwitchDuration;

public:

	/** Called on dodging an enemy attack */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gameplay Event")
	FOnGameplayEventMCDelegate OnDodgingAttack;

	/** Called on blocking an enemy attack */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gameplay Event")
	FOnGameplayEventMCDelegate OnBlockingAttack;

	/** Called on deflecting an enemy attack */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Gameplay Event")
	FOnGameplayEventMCDelegate OnDeflectingAttack;

	//~
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

	// FOnGameplayEventMCDelegate OnEnteringCombat;

	// FOnGameplayEventMCDelegate OnLeavingCombat;
	//~ 

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Combat)
	FOnCombatStateChangedMCDelegate OnInitiatingCombat;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Combat)
	FOnCombatStateChangedMCDelegate OnLeavingCombat;

	/** [Local] Display status effect message on player screen */
	FORCEINLINE void DisplayTextOnPlayerScreen(const FString& Message, const FLinearColor& TextColor, const FVector& TextPosition)
	{
		Client_DisplayTextOnPlayerScreen(Message, TextColor, TextPosition);
	}

	/** Displays status effect text on player screen */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "EOD Character")
	void CreateAndDisplayTextOnPlayerScreen(const FString& Message, const FLinearColor& TextColor, const FVector& TextPosition);

protected:

	FTimerHandle TargetSwitchTimerHandle;

	FTimerHandle DodgeImmunityTimerHandle;

	FTimerHandle BlockTimerHandle;

	FTimerHandle CrowdControlTimerHandle;

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CombatEvents)
	virtual void TurnOnTargetSwitch();

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CombatEvents)
	virtual void TurnOffTargetSwitch();

protected:

	// --------------------------------------
	//  Network
	// --------------------------------------

	UFUNCTION()
	void OnRep_WeaponSheathed();

	UFUNCTION()
	void OnRep_GuardActive();

	UFUNCTION()
	void OnRep_CharacterStateInfo(const FCharacterStateInfo& OldStateInfo);

	// DEPRECATED
	UFUNCTION()
	void OnRep_CharacterState(ECharacterState OldState);

	UFUNCTION()
	void OnRep_ServerCharacterState(FName LastState);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Dodge(uint8 DodgeIndex, float RotationYaw);

	virtual void Server_Dodge_Implementation(uint8 DodgeIndex, float RotationYaw);

	virtual bool Server_Dodge_Validate(uint8 DodgeIndex, float RotationYaw);

	UFUNCTION(NetMultiCast, Reliable)
	void Multicast_Dodge(uint8 DodgeIndex, float RotationYaw);

	virtual void Multicast_Dodge_Implementation(uint8 DodgeIndex, float RotationYaw);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SpawnAndMountRideableCharacter(TSubclassOf<ARideBase> RideCharacterClass);

	UFUNCTION(Client, Unreliable)
	void Client_DisplayTextOnPlayerScreen(const FString& Message, const FLinearColor& TextColor, const FVector& TextPosition);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetBlockMovementDirectionYaw(float NewYaw);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartBlockingDamage(float Delay);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopBlockingDamage();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TriggeriFrames(float Duration, float Delay);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetGuardActive(bool bValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetWeaponSheathed(bool bNewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIsRunning(bool bValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharacterStateAllowsMovement(bool bNewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetPCTryingToMove(bool bNewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharMovementDir(ECharMovementDirection NewDirection);

	// DEPRECATED
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharacterState(ECharacterState NewState);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetWalkSpeed(float WalkSpeed);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharacterRotation(FRotator NewRotation);

	UFUNCTION(NetMultiCast, Reliable)
	void Multicast_SetCharacterRotation(FRotator NewRotation);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetUseControllerRotationYaw(bool bNewBool);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetNextMontageSection(FName CurrentSection, FName NextSection);
	
	UFUNCTION(NetMultiCast, Reliable)
	void Multicast_SetNextMontageSection(FName CurrentSection, FName NextSection);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayAnimationMontage(UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);
	
	UFUNCTION(NetMultiCast, Reliable)
	void MultiCast_PlayAnimationMontage(UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);


	friend class AEODPlayerController;
	friend class UCharacterStateBase;
	
};

inline void AEODCharacterBase::ActivateGuard()
{
	SetGuardActive(true);
	StartBlockingDamage(DamageBlockTriggerDelay);
}

inline void AEODCharacterBase::DeactivateGuard()
{
	SetGuardActive(false);
	StopBlockingDamage();
}

inline void AEODCharacterBase::StartBlockingDamage(float Delay)
{
	if (Role < ROLE_Authority)
	{
		Server_StartBlockingDamage(Delay);
	}
	else
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(BlockTimerHandle, this, &AEODCharacterBase::EnableDamageBlocking, Delay, false);
		}
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

inline void AEODCharacterBase::SetWalkSpeed(const float WalkSpeed)
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp && MoveComp->MaxWalkSpeed != WalkSpeed)
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
	bCharacterStateAllowsMovement = bNewValue;
	if (Role < ROLE_Authority)
	{
		Server_SetCharacterStateAllowsMovement(bNewValue);
	}
}

inline void AEODCharacterBase::SetCharacterStateAllowsRotation(bool bValue)
{
	bCharacterStateAllowsRotation = bValue;
}

inline void AEODCharacterBase::SetCharacterMovementDirection(ECharMovementDirection NewDirection)
{
	CharacterMovementDirection = NewDirection;
	if (Role < ROLE_Authority)
	{
		Server_SetCharMovementDir(NewDirection);
	}
}

inline void AEODCharacterBase::SetBlockMovementDirectionYaw(float NewYaw)
{
	BlockMovementDirectionYaw = NewYaw;
	if (Role < ROLE_Authority)
	{
		Server_SetBlockMovementDirectionYaw(NewYaw);
	}
}

inline float AEODCharacterBase::GetControllerRotationYaw() const
{
	return (Controller ? FMath::UnwindDegrees(Controller->GetControlRotation().Yaw) : 0.0f);
}

inline void AEODCharacterBase::UpdatePCTryingToMove()
{
	if (ForwardAxisValue == 0 && RightAxisValue == 0)
	{
		if (bPCTryingToMove)
		{
			SetPCTryingToMove(false);
		}
	}
	else
	{
		if (!bPCTryingToMove)
		{
			SetPCTryingToMove(true);
		}
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

inline void AEODCharacterBase::EnableInteractionSphere()
{
	if (IsValid(InteractionSphereComponent))
	{
		InteractionSphereComponent->Activate();
		InteractionSphereComponent->SetCollisionProfileName(CollisionProfileNames::OverlapAllDynamic);
	}
}

inline void AEODCharacterBase::DisableInteractionSphere()
{
	if (IsValid(InteractionSphereComponent))
	{
		InteractionSphereComponent->Deactivate();
		InteractionSphereComponent->SetCollisionProfileName(CollisionProfileNames::NoCollision);
	}
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

FORCEINLINE bool AEODCharacterBase::IsAlive() const
{
	return IsValid(CharacterStatsComponent) ? CharacterStatsComponent->GetCurrentHealth() > 0 : true;
}

FORCEINLINE bool AEODCharacterBase::IsDead() const
{
	return IsValid(CharacterStatsComponent) ? CharacterStatsComponent->GetCurrentHealth() <= 0 : false;
}

FORCEINLINE bool AEODCharacterBase::IsIdle() const
{
	return (CharacterState == ECharacterState::IdleWalkRun && GetVelocity().Size() == 0);
}

FORCEINLINE bool AEODCharacterBase::IsMoving() const
{
	return (CharacterState == ECharacterState::IdleWalkRun && GetVelocity().Size() != 0);
}

FORCEINLINE bool AEODCharacterBase::IsIdleOrMoving() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

FORCEINLINE bool AEODCharacterBase::IsJumping() const
{
	return CharacterState == ECharacterState::Jumping;
}

FORCEINLINE bool AEODCharacterBase::IsDodging() const
{
	return CharacterState == ECharacterState::Dodging;
}

FORCEINLINE bool AEODCharacterBase::IsDodgingDamage() const
{
	return bActiveiFrames;
}

FORCEINLINE bool AEODCharacterBase::IsBlocking() const
{
	return CharacterState == ECharacterState::Blocking;
}

FORCEINLINE bool AEODCharacterBase::IsBlockingDamage() const
{
	return bBlockingDamage;
}

FORCEINLINE bool AEODCharacterBase::IsCastingSpell() const
{
	return CharacterState == ECharacterState::CastingSpell;
}

FORCEINLINE bool AEODCharacterBase::IsNormalAttacking() const
{
	// return CharacterState == ECharacterState::Attacking && GetCurrentActiveSkillID() != NAME_None;
	return CharacterState == ECharacterState::Attacking;
}

FORCEINLINE bool AEODCharacterBase::IsUsingAnySkill() const
{
	return CharacterState == ECharacterState::UsingActiveSkill && GetCurrentActiveSkillID() != NAME_None;
}

FORCEINLINE bool AEODCharacterBase::IsUsingSkill(FName SkillID) const
{
	return CharacterState == ECharacterState::UsingActiveSkill && GetCurrentActiveSkillID() == SkillID;
}

FORCEINLINE bool AEODCharacterBase::HasBeenHit() const
{
	return CharacterState == ECharacterState::GotHit;
}

FORCEINLINE bool AEODCharacterBase::CanFlinch() const
{
	if (IsValid(CharacterStatsComponent))
	{
		return IsAlive() && !CharacterStatsComponent->HasCrowdControlImmunity(ECrowdControlEffect::Flinch);
	}
	return true;
}

FORCEINLINE bool AEODCharacterBase::CanStun() const
{
	if (IsValid(CharacterStatsComponent))
	{
		return IsAlive() && !CharacterStatsComponent->HasCrowdControlImmunity(ECrowdControlEffect::Stunned);
	}
	return true;
}

FORCEINLINE bool AEODCharacterBase::CanKnockdown() const
{
	if (IsValid(CharacterStatsComponent))
	{
		return IsAlive() && !CharacterStatsComponent->HasCrowdControlImmunity(ECrowdControlEffect::KnockedDown);
	}
	return true;
}

FORCEINLINE bool AEODCharacterBase::CanKnockback() const
{
	if (IsValid(CharacterStatsComponent))
	{
		return IsAlive() && !CharacterStatsComponent->HasCrowdControlImmunity(ECrowdControlEffect::KnockedBack);
	}
	return true;
}

FORCEINLINE bool AEODCharacterBase::CanFreeze() const
{
	if (IsValid(CharacterStatsComponent))
	{
		return IsAlive() && !CharacterStatsComponent->HasCrowdControlImmunity(ECrowdControlEffect::Crystalized);
	}
	return true;
}

FORCEINLINE bool AEODCharacterBase::CanInterrupt() const
{
	if (IsValid(CharacterStatsComponent))
	{
		return IsAlive() && !CharacterStatsComponent->HasCrowdControlImmunity(ECrowdControlEffect::Interrupt);
	}
	return true;
}

FORCEINLINE bool AEODCharacterBase::NeedsHealing() const
{
	return IsValid(CharacterStatsComponent) ? CharacterStatsComponent->IsLowOnHealth() : false;
}

FORCEINLINE void AEODCharacterBase::SetOffTargetSwitch()
{
	TurnOnTargetSwitch();
}

FORCEINLINE ECharacterState AEODCharacterBase::GetCharacterState() const
{
	return CharacterState;
}

FORCEINLINE EFaction AEODCharacterBase::GetFaction() const
{
	return Faction;
}

inline FSkillTableRow* AEODCharacterBase::GetSkill(FName SkillID, const FString& ContextString) const
{
	FSkillTableRow* Skill = nullptr;
	if (IsValid(SkillsDataTable))
	{
		Skill = SkillsDataTable->FindRow<FSkillTableRow>(SkillID, ContextString);
	}
	return Skill;
}

FORCEINLINE FName AEODCharacterBase::GetCurrentActiveSkillID() const
{
	return CurrentActiveSkillID;
}

FORCEINLINE FSkillTableRow* AEODCharacterBase::GetCurrentActiveSkill() const
{
	return CurrentActiveSkill;
}

FORCEINLINE FLastUsedSkillInfo& AEODCharacterBase::GetLastUsedSkill()
{
	return LastUsedSkillInfo;
}

inline void AEODCharacterBase::PlayAnimationMontage(UAnimMontage* MontageToPlay, FName SectionToPlay)
{
	if (IsValid(GetMesh()) && IsValid(GetMesh()->GetAnimInstance()))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay, MontageToPlay);
	}
}

inline void AEODCharacterBase::PlayAnimationMontage(UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	if (IsValid(GetMesh()) && IsValid(GetMesh()->GetAnimInstance()))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay, MontageToPlay);
		CharacterState = NewState;
	}
	Server_PlayAnimationMontage(MontageToPlay, SectionToPlay, NewState);
}

inline FString AEODCharacterBase::GetGenderPrefix() const
{
	return (Gender == ECharacterGender::Male) ? FString("M_") : FString("F_");
}
