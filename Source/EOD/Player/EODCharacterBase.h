// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/EODLibrary.h"
#include "Statics/CharacterLibrary.h"
#include "Components/StatsComponentBase.h"
#include "StatusEffects/StatusEffectBase.h"

#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "EODCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatEvent, TWeakObjectPtr<AEODCharacterBase>, RecipientCharacter);

class UAnimMontage;
class UInputComponent;
class UStatusEffectBase;
class UStatsComponentBase;

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

	/** Returns true if character can move */
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

	/** Applies stun to this character */
	virtual bool Stun(const float Duration);

	/** [client] Removes 'stun' crowd control effect from this character */
	virtual void EndStun();

	/** Freeze this character */
	virtual bool Freeze(const float Duration);

	/** [client] Removes 'freeze' crowd control effect from this character */
	virtual void EndFreeze();

	/** Knockdown this character */
	virtual bool Knockdown(const float Duration);

	/** [client] Removes 'knock-down' crowd control effect from this character */
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

	/** Enables immunity frames for a given duration */
	UFUNCTION(BlueprintCallable, Category = Combat)
	void EnableiFrames(float Duration = 0.f);

	/** Disables immunity frames */
	UFUNCTION()
	void DisableiFrames();

	/** Enables blocking of incoming attacks */
	UFUNCTION()
	void EnableDamageBlocking();

	/** Disables blocking of incoming attacks */
	UFUNCTION()
	void DisableDamageBlocking();

	/**
	 * Called on successfully dodging an enemy attack
	 * @param AttackInstigator Enemy character whose incoming damage this character dodged
	 */
	void OnSuccessfulDodge(AEODCharacterBase* AttackInstigator);

	/**
	 * Called on successfully blocking an enemy attack
	 * @param AttackInstigator Enemy character whose incoming damage this character blocked
	 */
	void OnSuccessfulBlock(AEODCharacterBase* AttackInstigator);

	/**
	 * Called on getting an attack of this character blocked by an enemy
	 * @param AttackBlocker Enemy character that blocked this character's attack
	 */
	void OnAttackDeflected(AEODCharacterBase* AttackBlocker, bool bSkillIgnoresBlock);

	/** Temporarily trigger 'Target_Switch' material parameter to make the character glow */
	FORCEINLINE void SetOffTargetSwitch();

	/** Set whether character is engaged in combat or not */
	UFUNCTION(BlueprintCallable, Category = "EOD Character")
	virtual void SetInCombat(const bool bValue) { bInCombat = bValue; };
	
	/** Returns true if character is engaged in combat */
	FORCEINLINE bool GetInCombat() const;

	UFUNCTION(BlueprintPure, Category = "EOD Character", meta = (DisplayName = "Get In Combat"))
	bool BP_GetInCombat() const;

	/** [server + client] Set current state of character */
	inline void SetCharacterState(const ECharacterState NewState);

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

	/** Change character rotation */
	// void 

	/** [server + client] Chagne character rotation */
	UFUNCTION(BlueprintCallable, Category = "EOD Character")
	void SetCharacterRotation(const FRotator NewRotation);

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
	UFUNCTION(BlueprintCallable, Category = Skills)
	virtual bool UseSkill(FName SkillID);

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
	
	/** [server + client] Plays an animation montage and changes character state over network */
	inline void PlayAnimationMontage(UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);

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
	UFUNCTION(BlueprintCallable, category = Rotation)
	bool DeltaRotateCharacterToDesiredYaw(float DesiredYaw, float DeltaTime, float Precision = 0.1f, float RotationRate = 600.f);

	/**
	 * Kills this character 
	 * @param CauseOfDeath - The reason for death of this character
	 * @param Instigator - The character that instigated the death of this character (if any)
	 */
	virtual void Die(ECauseOfDeath CauseOfDeath, AEODCharacterBase* InstigatingChar = nullptr);

private:

	/** StatsComp contains and manages the stats info of this character */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStatsComponentBase* StatsComp;

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
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CharacterState)
	ECharacterState CharacterState;

protected:

	UPROPERTY()
	FCharacterStateData CharacterStateData;

	/** Data table for character skills */
	UPROPERTY(EditDefaultsOnly, Category = Skills)
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

	FCombatEvent OnSuccessfulDodgeEvent;

	FCombatEvent OnSuccessfulBlockEvent;

	FCombatEvent OnAttackDeflectedEvent;

	//~
	FCombatEvent OnReceivingHit;

	FCombatEvent OnSuccessfulHit;

	FCombatEvent OnSuccessfulMagickAttack;

	FCombatEvent OnSuccessfulPhysicalAttack;

	FCombatEvent OnUnsuccessfulHit;

	FCombatEvent OnCriticalHit;

	FCombatEvent OnKillingEnemy;

	FCombatEvent OnFullHealth;

	FCombatEvent OnDamageAtFullHealth;

	FCombatEvent OnLowHealth;

	FCombatEvent OnEnteringCombat;

	FCombatEvent OnLeavingCombat;
	//~ 

	FTimerHandle TargetSwitchTimerHandle;

	FTimerHandle DodgeImmunityTimerHandle;

	FTimerHandle BlockTimerHandle;

	FTimerHandle CrowdControlTimerHandle;

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CombatEvents)
	virtual void TurnOnTargetSwitch();

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CombatEvents)
	virtual void TurnOffTargetSwitch();

private:
	
	//~ Begin Multiplayer Code
	UFUNCTION()
	void OnRep_CharacterState(ECharacterState OldState);

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
	//~ End Multiplayer Code
	
	
};

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
	return !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::Flinch);
}

FORCEINLINE bool AEODCharacterBase::CanStun() const
{
	return !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::Stunned);
}

FORCEINLINE bool AEODCharacterBase::CanKnockdown() const
{
	return !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::KnockedDown);
}

FORCEINLINE bool AEODCharacterBase::CanKnockback() const
{
	return !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::KnockedBack);
}

FORCEINLINE bool AEODCharacterBase::CanFreeze() const
{
	return !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::Crystalized);
}

FORCEINLINE bool AEODCharacterBase::CanInterrupt() const
{
	return !StatsComp->HasCrowdControlImmunity(ECrowdControlEffect::Interrupt);
}

FORCEINLINE bool AEODCharacterBase::NeedsHealing() const
{
	return StatsComp->IsLowOnHealth();
}

FORCEINLINE void AEODCharacterBase::SetOffTargetSwitch()
{
	TurnOnTargetSwitch();
}

FORCEINLINE bool AEODCharacterBase::GetInCombat() const
{
	return bInCombat;
}

inline void AEODCharacterBase::SetCharacterState(const ECharacterState NewState)
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

FORCEINLINE FLastUsedSkillInfo & AEODCharacterBase::GetLastUsedSkill()
{
	return LastUsedSkillInfo;
}

inline void AEODCharacterBase::PlayAnimationMontage(UAnimMontage * MontageToPlay, FName SectionToPlay)
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay, MontageToPlay);
	}
}

inline void AEODCharacterBase::PlayAnimationMontage(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay, MontageToPlay);
		CharacterState = NewState;
	}

	Server_PlayAnimationMontage(MontageToPlay, SectionToPlay, NewState);
}
