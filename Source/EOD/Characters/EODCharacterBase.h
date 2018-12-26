// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/EODLibrary.h"
#include "EOD/Statics/CharacterLibrary.h"
#include "EOD/StatusEffects/StatusEffectBase.h"
#include "EOD/Characters/Components/StatsComponentBase.h"

#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EODCharacterBase.generated.h"

class UAnimMontage;
class USkillsComponent;
class UInputComponent;
class UCameraComponent;
// class AEODPlayerController;
// class AEODAIControllerBase;
class UStatusEffectBase;
class UGameplayEventBase;
class UStatsComponentBase;

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
 * An abstract base class to handle the behavior of in-game characters.
 * All in-game characters must inherit from this class.
 */
UCLASS(Abstract)
class EOD_API AEODCharacterBase : public ACharacter
{
	GENERATED_BODY()
		
public:
	/** Sets default values for this character's properties */
	AEODCharacterBase(const FObjectInitializer& ObjectInitializer);

	/** Updates character state every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;


	////////////////////////////////////////////////////////////////////////////////
	// EOD CHARACTER
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

	/**
	 * The direction character is trying to move relative to it's controller rotation
	 * If the character is controlled by player, it is determined by the movement keys pressed by player
	 */
	UPROPERTY(Replicated)
	ECharMovementDirection CharacterMovementDirection;

	/** Enables immunity frames for a given duration */
	UFUNCTION()
	void EnableiFrames(float Duration = 0.f);

	/** Disables immunity frames */
	UFUNCTION()
	void DisableiFrames();

protected:
	UPROPERTY()
	float DesiredRotationYawFromAxisInput;

	/**
	 * This bool is used to determine if the character can move even if it's not in 'IdleWalkRun' state.
	 * e.g., moving while casting spell.
	 */
	UPROPERTY(Replicated)
	bool bCharacterStateAllowsMovement;

	/** [server + local]  */
	FORCEINLINE void SetCharacterStateAllowsMovement(bool bNewValue)
	{
		bCharacterStateAllowsMovement = bNewValue;
		if (Role < ROLE_Authority)
		{
			Server_SetCharacterStateAllowsMovement(bNewValue);
		}
	}

	/** [server + local] */
	FORCEINLINE void SetIsRunning(bool bValue)
	{
		bIsRunning = bValue;
		if (Role < ROLE_Authority)
		{
			Server_SetIsRunning(bValue);
		}
	}

	/** [server + local] */
	FORCEINLINE void SetCharacterMovementDirection(ECharMovementDirection NewDirection)
	{
		CharacterMovementDirection = NewDirection;
		if (Role < ROLE_Authority)
		{
			Server_SetCharMovementDir(NewDirection);
		}
	}

	/** [server + local] */
	FORCEINLINE void SetPCTryingToMove(bool bNewValue)
	{
		bPCTryingToMove = bNewValue;
		if (Role < ROLE_Authority)
		{
			Server_SetPCTryingToMove(bNewValue);
		}
	}

public:
	FORCEINLINE bool IsRunning() const { return bIsRunning; }

	FORCEINLINE bool IsPCTryingToMove() const { return bPCTryingToMove; }

	FORCEINLINE ECharMovementDirection GetCharacterMovementDirection() const { return CharacterMovementDirection; }

	inline float GetRotationYawFromAxisInput() const;

	/** Returns the expected rotation yaw of character based on current Axis Input */
	UFUNCTION(BlueprintCallable, Category = "EOD Character", meta = (DisplayName = "Get Rotation Yaw From Axis Input"))
	float BP_GetRotationYawFromAxisInput() const;

	/**
	 * Returns controller rotation yaw in -180/180 range.
	 * @note the yaw obtained from Controller->GetControlRotation().Yaw is in 0/360 range, which may not be desirable
	 */
	FORCEINLINE float GetControllerRotationYaw() const
	{
		if (GetController())
		{
			return FMath::UnwindDegrees(GetController()->GetControlRotation().Yaw);
		}
		return 0.f;
	}

	/**
	 * Returns controller rotation yaw in -180/180 range.
	 * @note the yaw obtained from Controller->GetControlRotation().Yaw is in 0/360 range, which may not be desirable
	 */
	UFUNCTION(BlueprintCallable, Category = "EOD Character", meta = (DisplayName = "Get Controller Rotation Yaw"))
	float BP_GetControllerRotationYaw() const;

	/**
	 * [server + local] 
	 * Enables immunity frames after a given Delay for a given Duration on the server copy of this character.
	 */
	FORCEINLINE void TriggeriFrames(float Duration = 0.4f, float Delay = 0.f)
	{
		if (Role < ROLE_Authority)
		{
			Server_TriggeriFrames(Duration, Delay);
		}
		else
		{
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, FName("EnableiFrames"), Duration);
			GetWorld()->GetTimerManager().SetTimer(DodgeImmunityTimerHandle, TimerDelegate, Delay, false);
		}
	}

	/**
	 * [server + local]
	 * Enables immunity frames after a given Delay for a given Duration on the server copy of this character.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOD Character", meta = (DisplayName = "Trigger iFrames"))
	void BP_TriggeriFrames(float Duration = 0.4f, float Delay = 0.f);

protected:
	/** Max speed of character when it's walking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EOD Character")
	float DefaultWalkSpeed;

	/** Max speed of character when it's running */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EOD Character")
	float DefaultRunSpeed;

	/** Max speed of character when it's moving while blocking attacks */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EOD Character")
	float DefaultWalkSpeedWhileBlocking;

	/** [server + local] Plays an animation montage and changes character state over network */
	inline void PlayAnimationMontage(UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);

	////////////////////////////////////////////////////////////////////////////////
	// COMPONENTS
	////////////////////////////////////////////////////////////////////////////////
public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

	FORCEINLINE USkillsComponent* GetSkillsComponent() const { return SkillsComponent; }

	FORCEINLINE USphereComponent* GetInteractionSphere() const { return InteractionSphere; }

	FORCEINLINE void EnableInteractionSphere();

	FORCEINLINE void DisableInteractionSphere();

	FORCEINLINE void ZoomInCamera();

	FORCEINLINE void ZoomOutCamera();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	int32 CameraZoomRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	int CameraArmMinimumLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	int CameraArmMaximumLength;

private:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	/** StatsComp contains and manages the stats info of this character */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStatsComponentBase* StatsComp;

	//~ Skills component - manages skills of character
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkillsComponent* SkillsComponent;

	//~ Sphere component used to detect interactive objects
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* InteractionSphere;


	////////////////////////////////////////////////////////////////////////////////
	// ACTIONS
	////////////////////////////////////////////////////////////////////////////////
public:
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	virtual bool StartDodging();

	UFUNCTION(BlueprintCallable, Category = "Character Action")
	virtual bool StopDodging();

	UFUNCTION(BlueprintCallable, Category = "Character Action")
	virtual bool StartBlockingAttacks();

	UFUNCTION(BlueprintCallable, Category = "Character Action")
	virtual bool StopBlockingAttacks();

	UFUNCTION(BlueprintCallable, Category = "Character Action")
	virtual void TriggerInteraction();

	UFUNCTION(BlueprintCallable, Category = "Character Action")
	virtual void StartInteraction();

	UFUNCTION(BlueprintCallable, Category = "Character Action")
	virtual void UpdateInteraction();

	UFUNCTION(BlueprintCallable, Category = "Character Action")
	virtual void StopInteraction();

	/** Put or remove weapon inside sheath */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	virtual void ToggleSheathe();
	
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	virtual void StartNormalAttacking();

	UFUNCTION(BlueprintCallable, Category = "Character Action")
	virtual void StopNormalAttacking();

	virtual void PlayToggleSheatheAnimation();


	////////////////////////////////////////////////////////////////////////////////
	// CHARACTER STATE
	////////////////////////////////////////////////////////////////////////////////
private:
	/** Determines whether weapon is currently sheathed or not */
	UPROPERTY(ReplicatedUsing = OnRep_WeaponSheathed)
	bool bWeaponSheathed;

protected:
	/** [server + local] Change player's weapon sheath state */
	FORCEINLINE void SetWeaponSheathed(bool bNewValue)
	{
		bWeaponSheathed = bNewValue;
		PlayToggleSheatheAnimation();
		if (Role < ROLE_Authority)
		{
			Server_SetWeaponSheathed(bNewValue);
		}
	}

	inline void UpdatePCTryingToMove();

	inline void UpdateCharacterMovementDirection();

	void UpdateDesiredYawFromAxisInput();

	void UpdateMovement(float DeltaTime);

public:
	/** Cached value of player's forward axis input */
	UPROPERTY()
	float ForwardAxisValue;

	/** Cached value of player's right axis input */
	UPROPERTY()
	float RightAxisValue;

	FORCEINLINE bool IsWeaponSheathed() const { return bWeaponSheathed; }

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

	/** Returns true if character can dodge */
	virtual bool CanDodge() const;
	
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

	/** [server + client] Set current state of character */
	FORCEINLINE void SetCharacterState(const ECharacterState NewState);

	/** [server + client] Set current state of character */
	UFUNCTION(BlueprintCallable, Category = "EOD Character", meta = (DisplayName = "Set Character State"))
	void BP_SetCharacterState(const ECharacterState NewState);

	/** Get current state of character */
	FORCEINLINE ECharacterState GetCharacterState() const;

	UFUNCTION(BlueprintPure, Category = "EOD Character", meta = (DisplayName = "Get Character State"))
	ECharacterState BP_GetCharacterState() const;

	FORCEINLINE UStatsComponentBase* GetStatsComponent() const;

	/** [server + client] Change character max walk speed */
	UFUNCTION(BlueprintCallable, Category = "EOD Character")
	void SetWalkSpeed(const float WalkSpeed);

	/** [server + client] Change character rotation. Do not use this for consecutive rotation change */
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
	UFUNCTION(BlueprintCallable, Category = "EOD Character")
	void SetUseControllerRotationYaw(const bool bNewBool);

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

	/** Set the ID of the skill that is currently being used */
	FORCEINLINE void SetCurrentActiveSkillID(FName SkillID);

	FORCEINLINE void SetCurrentActiveSkill(FSkillTableRow* Skill);

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

	/** [server + client] Plays an animation montage over network */
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

	/** Character state determines the current action character is doing */
	UPROPERTY(ReplicatedUsing = OnRep_CharacterState)
	ECharacterState CharacterState;

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

	/** Determines if invincibility frames are active */
	UPROPERTY(Transient)
	bool bActiveiFrames;

	/** Determines if character is blocking any incoming damage */
	UPROPERTY(Transient)
	bool bBlockingDamage;

	/** Determines whether character is currently engaged in combat or not */
	UPROPERTY(Transient)
	bool bInCombat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Constants)
	int DodgeStaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Constants)
	float DodgeImmunityTriggerDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Constants)
	float DodgeImmunityDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Constants)
	float DamageBlockTriggerDelay;

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

	FOnGameplayEventMCDelegate OnEnteringCombat;

	FOnGameplayEventMCDelegate OnLeavingCombat;
	//~ 

protected:

	FTimerHandle TargetSwitchTimerHandle;

	FTimerHandle DodgeImmunityTimerHandle;

	FTimerHandle BlockTimerHandle;

	FTimerHandle CrowdControlTimerHandle;

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CombatEvents)
	virtual void TurnOnTargetSwitch();

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CombatEvents)
	virtual void TurnOffTargetSwitch();


	////////////////////////////////////////////////////////////////////////////////
	// NETWORK
	////////////////////////////////////////////////////////////////////////////////
private:
	UFUNCTION()
	void OnRep_WeaponSheathed();

	UFUNCTION()
	void OnRep_CharacterState(ECharacterState OldState);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TriggeriFrames(float Duration, float Delay);

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

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharacterState(ECharacterState NewState);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetWalkSpeed(float WalkSpeed);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharacterRotation(FRotator NewRotation);
	
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
	
	
};

inline void AEODCharacterBase::UpdatePCTryingToMove()
{
	if (ForwardAxisValue == 0 && RightAxisValue == 0)
	{
		if (IsPCTryingToMove())
		{
			SetPCTryingToMove(false);
		}
	}
	else
	{
		if (!IsPCTryingToMove())
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

inline float AEODCharacterBase::GetRotationYawFromAxisInput() const
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

FORCEINLINE void AEODCharacterBase::EnableInteractionSphere()
{
	InteractionSphere->Activate();
	InteractionSphere->SetCollisionProfileName(FName("OverlapAllDynamic"));
}

FORCEINLINE void AEODCharacterBase::DisableInteractionSphere()
{
	InteractionSphere->Deactivate();
	InteractionSphere->SetCollisionProfileName(FName("NoCollision"));
}

FORCEINLINE void AEODCharacterBase::ZoomInCamera()
{
	if (CameraBoom && CameraBoom->TargetArmLength >= CameraArmMinimumLength)
	{
		CameraBoom->TargetArmLength -= CameraZoomRate;
	}
}

FORCEINLINE void AEODCharacterBase::ZoomOutCamera()
{
	if (CameraBoom && CameraBoom->TargetArmLength <= CameraArmMaximumLength)
	{
		CameraBoom->TargetArmLength += CameraZoomRate;
	}
}

FORCEINLINE bool AEODCharacterBase::IsAlive() const
{
	return StatsComp->GetCurrentHealth() > 0;
}

FORCEINLINE bool AEODCharacterBase::IsDead() const
{
	return StatsComp->GetCurrentHealth() <= 0;
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
	return CharacterState == ECharacterState::Attacking && GetCurrentActiveSkillID() != NAME_None;
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
	return IsAlive() && !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::Flinch);
}

FORCEINLINE bool AEODCharacterBase::CanStun() const
{
	return IsAlive() && !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::Stunned);
}

FORCEINLINE bool AEODCharacterBase::CanKnockdown() const
{
	return IsAlive() && !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::KnockedDown);
}

FORCEINLINE bool AEODCharacterBase::CanKnockback() const
{
	return IsAlive() && !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::KnockedBack);
}

FORCEINLINE bool AEODCharacterBase::CanFreeze() const
{
	return IsAlive() && !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::Crystalized);
}

FORCEINLINE bool AEODCharacterBase::CanInterrupt() const
{
	return IsAlive() && !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::Interrupt);
}

FORCEINLINE bool AEODCharacterBase::NeedsHealing() const
{
	return StatsComp->IsLowOnHealth();
}

FORCEINLINE void AEODCharacterBase::SetOffTargetSwitch()
{
	TurnOnTargetSwitch();
}

FORCEINLINE void AEODCharacterBase::SetCharacterState(const ECharacterState NewState)
{
	CharacterState = NewState;
	if (Role < ROLE_Authority)
	{
		Server_SetCharacterState(NewState);
	}
}

FORCEINLINE ECharacterState AEODCharacterBase::GetCharacterState() const
{
	return CharacterState;
}

FORCEINLINE UStatsComponentBase* AEODCharacterBase::GetStatsComponent() const
{
	return StatsComp;
}

FORCEINLINE EFaction AEODCharacterBase::GetFaction() const
{
	return Faction;
}

inline FSkillTableRow* AEODCharacterBase::GetSkill(FName SkillID, const FString& ContextString) const
{
	FSkillTableRow* Skill = nullptr;

	if (SkillsDataTable)
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

FORCEINLINE void AEODCharacterBase::SetCurrentActiveSkillID(FName SkillID)
{
	CurrentActiveSkillID = SkillID;
}

FORCEINLINE void AEODCharacterBase::SetCurrentActiveSkill(FSkillTableRow* Skill)
{
	CurrentActiveSkill = Skill;
}

FORCEINLINE FLastUsedSkillInfo& AEODCharacterBase::GetLastUsedSkill()
{
	return LastUsedSkillInfo;
}

inline void AEODCharacterBase::PlayAnimationMontage(UAnimMontage* MontageToPlay, FName SectionToPlay)
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay, MontageToPlay);
	}
}

inline void AEODCharacterBase::PlayAnimationMontage(UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay, MontageToPlay);
		CharacterState = NewState;
	}

	Server_PlayAnimationMontage(MontageToPlay, SectionToPlay, NewState);
}
