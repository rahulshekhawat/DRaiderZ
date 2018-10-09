// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffectBase.h"
#include "Statics/CharacterLibrary.h"
#include "GameFramework/Character.h"
#include "EODCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatEvent, TArray<TWeakObjectPtr<AEODCharacterBase>>, RecipientCharacters);

class UAnimMontage;
class UInputComponent;
class UStatusEffectBase;
class UStatsComponentBase;

USTRUCT(BlueprintType)
struct FLastUsedSkillInfo
{
	GENERATED_USTRUCT_BODY()

public:

	FSkill* LastUsedSkill;

	/** True if the last skill was interrupted */
	bool bInterrupted;

	FLastUsedSkillInfo()
	{
		bInterrupted = false;
		LastUsedSkill = nullptr;
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

	/** Sets default values for this character's properties */
	AEODCharacterBase(const FObjectInitializer& ObjectInitializer);

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

	//~ @todo redefinition
	/** Returns true if character is using any skill */
	FORCEINLINE bool IsUsingAnySkill() const;

	/** Returns true if character is using skill at SkillIndex */
	FORCEINLINE bool IsUsingSkill(int32 SkillIndex) const;

	/** Returns true if character has just been hit */
	FORCEINLINE bool HasBeenHit() const;

	/** Returns true if a hit from HitSkill was critical */
	FORCEINLINE bool IsCriticalHit(const FSkill* HitSkill) const;

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

	/** Returns true if character can be stunned */
	virtual bool CanBeStunned() const;
	
	/** Returns true if character can use any skill at all */
	virtual bool CanUseAnySkill() const;

	/** Returns true if character can use a particular skill at SkillIndex */
	virtual bool CanUseSkill(int32 SkillIndex) const;
	
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

	/** [server + client] Interrupt this character's current action */
	virtual void Interrupt(const EHitDirection InterruptDirection) PURE_VIRTUAL(AEODCharacterBase::Interrupt, );

	/** [server + client] Flinch this character. This is nothing more than a visual feedback to getting attacked */
	virtual void Flinch(const EHitDirection FlinchDirection) PURE_VIRTUAL(AEODCharacterBase::Flinch, );

	/** [server + client] Applies stun to this character */
	virtual void Stun(const float Duration) PURE_VIRTUAL(AEODCharacterBase::Stun, );

	/** [client] Removes 'stun' crowd control effect from this character */
	virtual void EndStun() PURE_VIRTUAL(AEODCharacterBase::EndStun, );

	/** [server + client] Freeze this character */
	virtual void Freeze(const float Duration) PURE_VIRTUAL(AEODCharacterBase::Freeze, );

	/** [client] Removes 'freeze' crowd control effect from this character */
	virtual void EndFreeze() PURE_VIRTUAL(AEODCharacterBase::EndFreeze, );

	/** [server + client] Knockdown this character */
	virtual void Knockdown(const float Duration) PURE_VIRTUAL(AEODCharacterBase::Knockdown, );

	/** [client] Removes 'knock-down' crowd control effect from this character */
	virtual void EndKnockdown() PURE_VIRTUAL(AEODCharacterBase::EndKnockdown, );

	/** [server + client] Knockback this character */
	virtual void Knockback(const float Duration, const FVector& Impulse) PURE_VIRTUAL(AEODCharacterBase::Knockback, );

	/** Enables immunity frames for a given duration */
	UFUNCTION()
	void EnableiFrames(float Duration = 0.f);

	/** Disables immunity frames */
	UFUNCTION()
	void DisableiFrames();

	UFUNCTION()
	void EnableDamageBlocking();

	UFUNCTION()
	void DisableDamageBlocking();

	/**
	 * Kills this character 
	 * @param CauseOfDeath - The reason for death of this character
	 * @param Instigator - The character that instigated the death of this character (if any)
	 */
	virtual void Die(ECauseOfDeath CauseOfDeath, AEODCharacterBase* InstigatingChar = nullptr);

	/** Set whether character is in combat or not */
	virtual void SetInCombat(const bool bValue) { bInCombat = bValue; }

	/** Get current state of character */
	FORCEINLINE ECharacterState GetCharacterState() const;

	/** [server + client] Set current state of character */
	void SetCharacterState(const ECharacterState NewState);

	/** [server + client] Change character max walk speed */
	void SetWalkSpeed(const float WalkSpeed);

	/** [server + client] Chagne character rotation */
	void SetCharacterRotation(const FRotator NewRotation);

	/** [server + client] Set whether character should use controller rotation yaw or not */
	void SetUseControllerRotationYaw(const bool bNewBool);

	/** Returns character faction */
	FORCEINLINE EFaction GetFaction() const;

	/** Get skill info from a given SkillID */
	// FORCEINLINE FSkill* GetSkill(FName SkillID) const;

	/** Returns the skill at given SkillIndex */
	// virtual FSkill* GetSkill(int32 SkillIndex) const;

	/** [server + client] Use a skill and play it's animation */
	// UFUNCTION(BlueprintCallable, Category = Skills)
	// virtual bool UseSkill(int32 SkillIndex);

	/** Use a skill and play it's animation */
	UFUNCTION(BlueprintCallable, Category = Skills)
	virtual bool UseSkill(FName SkillID);

	//~ @note might not be needed
	UFUNCTION(BlueprintCallable, Category = Skills)
	virtual void StartSkill(FName SkillID);

	UFUNCTION(BlueprintCallable, Category = Skills)
	virtual void StopSkill(FName SkillID);

	/** 
	 * Determines and returns the status of skill at a given SkillIndex
	 * Returns EEODTaskStatus::Active if character is currently using skill
	 * Returns EEODTaskStatus::Finished if character has finished using skill
	 * Returns EEODTaskStatus::Aborted if skill was aborted before completion
	 * Returns EEODTaskStatus::Inactive if the character is using or have used another skill
	 */
	// UFUNCTION(BlueprintCallable, Category = Skills)
	// virtual EEODTaskStatus CheckSkillStatus(int32 SkillIndex);

	//~ @todo modify function parameters to accept enemy target perhaps
	//~ @todo replace it with GetMostWeightedSkillID()
	/** [AI] Returns the skill that is more appropriate to use in the given situtation */
	// UFUNCTION(BlueprintCallable, Category = Skills)
	// virtual int32 GetMostWeightedSkillIndex() const;
		
	/** [AI] Returns the skill that is more appropriate to use in current state against the given enemy */
	UFUNCTION(BlueprintCallable, Category = Skills)
	virtual FName GetMostWeightedMeleeSkillID(AEODCharacterBase const * const TargetCharacter) const;

	/** Returns the skill that character is using currently. Returns nullptr if character is not using any skill */
	// FORCEINLINE FSkill* GetCurrentActiveSkill() const;

	// virtual void SetCurrentActiveSkill(FName SkillID) PURE_VIRTUAL(AEODCharacterBase::SetCurrentActiveSkill, );

	virtual void OnNormalAttackSectionStart(FName SectionName) PURE_VIRTUAL(AEODCharacterBase::OnNormalAttackSectionStart, );

	/** Returns the last used skill */
	FORCEINLINE FLastUsedSkillInfo& GetLastUsedSkill();

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

	/** [server] Handle melee collision */
	// virtual void OnMeleeCollision(UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit);

	/** [server] Apply damage to this character */
	// virtual FEODDamageResult ApplyEODDamage(FEODDamage& EODDamage);

	// virtual FEODDamageResult ApplyEODDamage(AEODCharacterBase* InstigatingChar, const FEODDamage& EODDamage, const FHitResult& CollisionHitResult);

	// virtual FEODDamageResult ApplyEODDamage(AEODCharacterBase* InstigatingChar, const FEODDamage& EODDamage, const FHitResult& CollisionHitResult, const FHitResult& LineHitResult);

	/** Called on an animation montage blending out to clean up, reset, or change any state variables */
	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	/** Called on an animation montage ending to clean up, reset, or change any state variables */
	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	/** [server + client] Plays an animation montage over network */
	FORCEINLINE void PlayAnimationMontage(UAnimMontage* MontageToPlay, FName SectionToPlay);
	
	/** [server + client] Plays an animation montage and changes character state over network */
	FORCEINLINE void PlayAnimationMontage(UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);

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

	/** StatsComp contains and manages the stats info of this character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	UStatsComponentBase* StatsComp;

	//~ @todo Combat State: Damaged, running, waiting, engaged in combat with enemy, healing etc.?
	//~ @todo Ranged collision

protected:

	FCombatEvent OnReceivingHit;

	FCombatEvent OnSuccessfulHit;

	FCombatEvent OnSuccessfulMagickAttack;

	FCombatEvent OnSuccessfulPhysicalAttack;

	FCombatEvent OnUnsuccessfulHit;

	FCombatEvent OnCriticalHit;

	FCombatEvent OnKillingEnemy;

	FCombatEvent OnSuccessfulDodge;

	FCombatEvent OnSuccessfulBlock;

	FCombatEvent OnFullHealth;

	FCombatEvent OnDamageAtFullHealth;

	FCombatEvent OnLowHealth;

	FCombatEvent OnEnteringCombat;

	FCombatEvent OnLeavingCombat;

	/** Map of skill index and it's respective combat event */
	TMap<uint8, FCombatEvent> OnUsingSkillEventMap;

	TArray<FSkill*> Skills;

	TMap<FName, FSkill*> IDToSkillMap;

	TMap<FName, FSkill*> NormalAttackSectionToSkillMap;

	FSkill* CurrentActiveSkill;

	FLastUsedSkillInfo LastUsedSkillInfo;

	FTimerHandle DodgeTimerHandle;

	FTimerHandle BlockTimerHandle;

	FTimerHandle CrowdControlTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Rotation)
	float CharacterRotationPrecision;

	UPROPERTY(EditDefaultsOnly, Category = RequiredInfo)
	EFaction Faction;

	/** True if character is in God Mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GodMode)
	bool bGodMode;
	
	/** Character state determines the current action character is doing */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CharacterState)
	ECharacterState CharacterState;
	
	/** Determines whether character is currently engaged in combat or not */
	UPROPERTY(Transient)
	bool bInCombat;	

	/** Determines if invincibility frames are active */
	UPROPERTY(Transient)
	bool bHasActiveiFrames;

	/** Determines if character is blocking any incoming damage */
	UPROPERTY(Transient)
	bool bIsBlockingDamage;

	// @todo
	// virtual void OnBlockingEnemy()

	// virtual void OnBlockedByEnemy();

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
