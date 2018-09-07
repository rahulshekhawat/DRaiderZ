// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffectBase.h"
#include "Statics/CharacterLibrary.h"
#include "GameFramework/Character.h"
#include "EODCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatEvent, TArray<TWeakObjectPtr<AEODCharacterBase>>, RecipientCharacters);

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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Returns true if character is alive */
	virtual bool IsAlive() const;

	/** Returns true if character is dead */
	virtual bool IsDead() const;
	
	/** Returns true if character is idling */
	virtual bool IsIdle() const;

	/** Returns true if character is moving around */
	virtual bool IsMoving() const;

	/** Returns true if character is in idle-walk-run state */
	virtual bool IsIdleOrMoving() const;

	/** Returns true if character is jumping */
	virtual bool IsJumping() const;

	/** Determines if the character is in dodge state. Used to trigger dodge animation */
	virtual bool IsDodging() const;	
	
	/** Determines if the character is dodging incoming damage */
	virtual bool IsDodgingDamage() const;

	/**
	 * Detemines if the character is in block state. Used to trigger block animation.
	 * @note there is a slight delay between when the block animation is triggered and when the character actually starts blocking damage
	 */
	virtual bool IsBlocking() const;

	/** 
	 * Determines if the character is actively blocking any incoming damage
	 * @note there is a slight delay between when the block animation is triggered and when the character actually starts blocking damage
	 */
	virtual bool IsBlockingDamage() const;

	/** Returns true if character is currently casting a spell */
	virtual bool IsCastingSpell() const;
	
	/** Returns true if character is using a normal attack */
	virtual bool IsNormalAttacking() const;

	/** Returns true if character is using any skill */
	virtual bool IsUsingAnySkill() const;

	/** Returns true if character is using skill at SkillIndex */
	virtual bool IsUsingSkill(int32 SkillIndex) const;

	/** Returns true if a hit from HitSkill was critical */
	virtual bool IsCriticalHit(const FSkill* HitSkill) const;

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
	
	/** Returns true if this character requires healing (low on HP) */
	virtual bool NeedsHeal() const;

	/** Returns true if this character is healing either self or another character */
	virtual bool IsHealing() const;

	/** [server + client] Applies stun to this character */
	virtual void ApplyStun(float Duration);

	UFUNCTION()
	virtual void EnableiFrames(float Duration = 0.f);

	UFUNCTION()
	virtual void DisableiFrames();

	UFUNCTION()
	virtual void EnableDamageBlocking();

	UFUNCTION()
	virtual void DisableDamageBlocking();

	/**
	 * Kills this character 
	 * @param CauseOfDeath - The reason for death of this character
	 * @param Instigator - The character that instigated the death of this character (if any)
	 */
	virtual void Die(ECauseOfDeath CauseOfDeath, AEODCharacterBase* Instigator = nullptr);

	/** Set whether character is in combat or not */
	virtual void SetInCombat(const bool bValue) { bInCombat = bValue; }

	/** Get current state of character */
	ECharacterState GetCharacterState() const;

	/** [server + client] Set current state of character */
	void SetCharacterState(const ECharacterState NewState);

	/** [server + client] Change character max walk speed */
	void SetWalkSpeed(const float WalkSpeed);

	/** [server + client] Chagne character rotation */
	void SetCharacterRotation(const FRotator NewRotation);

	/** [server + client] Set whether character should use controller rotation yaw or not */
	void SetUseControllerRotationYaw(const bool bNewBool);

	/** Returns character faction */
	EFaction GetFaction() const;

	/** Returns the skill at given SkillIndex */
	virtual FSkill* GetSkill(int32 SkillIndex) const;

	/** [server + client] Use a skill and play it's animation */
	UFUNCTION(BlueprintCallable, Category = Skills)
	virtual bool UseSkill(int32 SkillIndex);
	
	/** 
	 * Determines and returns the status of skill at a given SkillIndex
	 * Returns EEODTaskStatus::Active if character is currently using skill
	 * Returns EEODTaskStatus::Finished if character has finished using skill
	 * Returns EEODTaskStatus::Aborted if skill was aborted before completion
	 * Returns EEODTaskStatus::Inactive if the character is using or have used another skill
	 */
	UFUNCTION(BlueprintCallable, Category = Skills)
	virtual EEODTaskStatus CheckSkillStatus(int32 SkillIndex);

	//~ @todo modify function parameters to accept enemy target perhaps
	/** [AI] Returns the skill that is more appropriate to use in the given situtation */
	UFUNCTION(BlueprintCallable, Category = Skills)
	virtual int32 GetMostWeightedSkillIndex() const;

	/** Returns the skill that character is using currently. Returns nullptr if character is not using any skill */
	virtual FSkill* GetCurrentActiveSkill() const;

	/** Returns the last used skill */
	virtual FLastUsedSkillInfo& GetLastUsedSkill();

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
	virtual void OnMeleeCollision(UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit);

	/** [server] Apply damage to this character */
	virtual FEODDamageResult ApplyEODDamage(FEODDamage& EODDamage);

	/** Called on an animation montage blending out to clean up, reset, or change any state variables */
	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	/** Called on an animation montage ending to clean up, reset, or change any state variables */
	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	/** [server + client] Plays an animation montage over network */
	UFUNCTION(BlueprintCallable, Category = Animations)
	void PlayAnimationMontage(class UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);

	void SetNextMontageSection(FName CurrentSection, FName NextSection);

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

	FSkill* CurrentActiveSkill;

	FLastUsedSkillInfo LastUsedSkillInfo;

	FTimerHandle DodgeTimerHandle;

	FTimerHandle BlockTimerHandle;

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
	void Server_PlayAnimationMontage(class UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);
	
	UFUNCTION(NetMultiCast, Reliable)
	void MultiCast_PlayAnimationMontage(class UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);
	//~ End Multiplayer Code
	
	
};
