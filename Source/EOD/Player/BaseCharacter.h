// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffect.h"
#include "Statics/CharacterLibrary.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS(Abstract)
class EOD_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Property replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//~ Begin Character Action State Handlers
	
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
	
	// Playing special idle animations through Random Sequece Player seems like a better idea
	// virtual bool IsPlayingSpecialIdle() const;

	//~ End Character Action State Handlers


	virtual void SetInCombat(bool bValue) { bInCombat = bValue; }

	// virtual void SetCombatState
	//~ TODO : Combat State: Damaged, running, waiting, engaged with enemy, healing etc.?

	//~ Begin Character Combat Status Functionality
	virtual bool NeedsHeal() const;
	//~ End Character Combat Status Functionality


	//~ Begin Multiplayer Code
	void SetCharacterState(ECharacterState NewState);
	
	UPROPERTY(Replicated)
	ECharacterState CharacterState;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharacterState(ECharacterState NewState);

	void SetWalkSpeed(float WalkSpeed);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetWalkSpeed(float WalkSpeed);

	void SetCharacterRotation(FRotator NewRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharacterRotation(FRotator NewRotation);

	void SetUseControllerRotationYaw(bool bNewBool);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetUseControllerRotationYaw(bool bNewBool);

	void SetNextMontageSection(FName CurrentSection, FName NextSection);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetNextMontageSection(FName CurrentSection, FName NextSection);
	
	UFUNCTION(NetMultiCast, Reliable)
	void Multicast_SetNextMontageSection(FName CurrentSection, FName NextSection);

	UFUNCTION(BlueprintCallable, Category = Animations)
	void PlayAnimationMontage(class UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayAnimationMontage(class UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);
	
	UFUNCTION(NetMultiCast, Reliable)
	void MultiCast_PlayAnimationMontage(class UAnimMontage* MontageToPlay, FName SectionToPlay, ECharacterState NewState);
	//~ End Multiplayer Code

	// @todo improve the status effect visuals system
	/**
	 * Call this to add status effect visuals on character (e.g. burning particle effect)
	 * This function will be used to add the status effect icon in player UI as well.
	*/	
	UFUNCTION(BlueprintCallable, Category = StatusEffects)
	virtual void AddStatusEffectVisuals(FStatusEffectInfo StatusEffectInfo);
	
	UFUNCTION(BlueprintCallable, Category = StatusEffects)
	virtual void RemoveStatusEffectVisuals(FStatusEffectInfo StatusEffectInfo);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	class UBaseStatsComponent* StatsComp;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Determines whether character is currently engaged in combat or not. Not in combat by default. */
	// UPROPERTY(replicated)
	bool bInCombat = false;	



};
