// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffectBase.h"
#include "Statics/CharacterLibrary.h"
#include "GameFramework/Character.h"
#include "EODCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatEvent, TArray<TWeakObjectPtr<AEODCharacterBase>>, RecipientCharacters);

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

	//~ Begin state handlers
	virtual bool IsAlive() const;

	virtual bool IsDead() const;
	
	virtual bool IsIdle() const;

	virtual bool IsMoving() const;

	virtual bool IsJumping() const;

	virtual bool IsDodging() const;	
	
	virtual bool IsBlocking() const;

	virtual bool IsCastingSpell() const;

	virtual bool IsNormalAttacking() const;

	virtual bool CanMove() const;
	
	virtual bool CanJump() const;

	virtual bool CanDodge() const;
	
	virtual bool CanBlock() const;

	virtual bool CanRespawn() const;

	virtual bool CanNormalAttack() const;
	
	virtual void SetInCombat(bool bValue) { bInCombat = bValue; }
	
	virtual bool NeedsHeal() const;
	//~ End state handlers

	//~ @todo Combat State: Damaged, running, waiting, engaged in combat with enemy, healing etc.?
	// @todo improve the status effect visuals system

	/**
	 * Adds the status effect visuals to this character
	 * For example, if a character is burning, this function will put the flame (particle effects) on character
	 * If the character is also a player character, this function will add status effect icon to player's viewport
	 */
	// UFUNCTION(BlueprintCallable, Category = StatusEffects)
	// virtual void AddStatusEffectVisuals(FStatusEffectInfo StatusEffectInfo);
	
	/** Removes the status effect visuals from this character */
	// UFUNCTION(BlueprintCallable, Category = StatusEffects)
	// virtual void RemoveStatusEffectVisuals(FStatusEffectInfo StatusEffectInfo);

	/** StatsComp contains and manages the stats info of this character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	class UStatsComponentBase* StatsComp;

	//~ Begin combat events
	FCombatEvent OnReceivingHit;
	FCombatEvent OnSuccessfulHit;
	FCombatEvent OnUnsuccessfulHit;
	FCombatEvent OnCriticalHit;
	FCombatEvent OnSuccessfulDodge;
	FCombatEvent OnSuccessfulBlock;
	FCombatEvent OnFullHealth;
	FCombatEvent OnDamageAtFullHealth;
	FCombatEvent OnLowHealth;
	FCombatEvent OnEnteringCombat;
	FCombatEvent OnLeavingCombat;
	//~ End combat events

	/** Map of skill index and it's respective combat event */
	TMap<uint8, FCombatEvent> OnUsingSkillEventMap;

	/** Returns the info for skill that this character is curretly using */
	virtual FActiveSkill GetCurrentActiveSkillInfo();

	/** [server] Handle melee collision */
	virtual void HandleMeleeCollision(UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit) PURE_VIRTUAL(AEODCharacterBase::HandleMeleeCollision, );

	// @todo
	// virtual void HandleRangedCollision();

protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:
	
	/** Character state determines the current action character is doing */
	UPROPERTY(Replicated)
	ECharacterState CharacterState;
	
	/** Determines whether character is currently engaged in combat or not */
	UPROPERTY(Transient)
	bool bInCombat;

	void SetCharacterState(ECharacterState NewState);
	
	void SetWalkSpeed(float WalkSpeed);
	
	void SetCharacterRotation(FRotator NewRotation);
	
	void SetUseControllerRotationYaw(bool bNewBool);
	
	void SetNextMontageSection(FName CurrentSection, FName NextSection);
	
	UFUNCTION(BlueprintCallable, Category = Animations)
	void PlayAnimationMontage(class UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);

private:
	
	//~ Begin Multiplayer Code
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
