// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLibrary.h"
#include "EODCharacterBase.h"

#include "Engine/StreamableManager.h"
#include "AICharacterBase.generated.h"

class UEODWidgetComponent;
class UGameplaySkillBase;

/**
 * AICharacterBase is the base class for AI controlled characters
 */
UCLASS()
class EOD_API AAICharacterBase : public AEODCharacterBase
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AAICharacterBase(const FObjectInitializer& ObjectInitializer);

	/** Initialize skills */
	virtual void PostInitializeComponents() override;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Updates character state every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;

	// --------------------------------------
	//  Combat Interface
	// --------------------------------------

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

	// --------------------------------------
	//  Components
	// --------------------------------------

	/** Get aggro widget component */
	FORCEINLINE UEODWidgetComponent* GetAggroWidgetComp() const { return AggroWidgetComp; }

	/** Get health widget component */
	FORCEINLINE UEODWidgetComponent* GetHealthWidgetComp() const { return HealthWidgetComp; }

	static const FName AggroWidgetCompName;

	static const FName HealthWidgetCompName;

protected:

	/** Used to display floating aggro widget above AI character */
	UPROPERTY(Category = UI, VisibleAnywhere, BlueprintReadOnly)
	UEODWidgetComponent* AggroWidgetComp;

	/** Used to display floating health widget above AI character */
	UPROPERTY(Category = UI, VisibleAnywhere, BlueprintReadOnly)
	UEODWidgetComponent* HealthWidgetComp;

public:

	// --------------------------------------
	//  Crowd Control Effects
	// --------------------------------------

	/** Flinch this character (visual feedback) */
	virtual bool CCEFlinch_Implementation(const float BCAngle) override;

	/** Interrupt this character's current action */
	virtual bool CCEInterrupt_Implementation(const float BCAngle) override;

	/** Applies stun to this character */
	virtual bool CCEStun_Implementation(const float Duration) override;

	/** Removes 'stun' crowd control effect from this character */
	virtual void CCERemoveStun_Implementation() override;

	/** Freeze this character */
	virtual bool CCEFreeze_Implementation(const float Duration) override;

	/** Removes 'freeze' crowd control effect from this character */
	virtual void CCEUnfreeze_Implementation() override;
	
	/** Knockdown this character */
	virtual bool CCEKnockdown_Implementation(const float Duration) override;
	
	/** Removes 'knock-down' crowd control effect from this character */
	virtual void CCEEndKnockdown_Implementation() override;
	
	/** Knockback this character */
	virtual bool CCEKnockback_Implementation(const float Duration, const FVector& ImpulseDirection) override;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	/** In game name of this AI character. This may or may not differ for each instance of character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "In-Game Information")
	FString InGameName;

	/** In game description of this AI character. This may or may not differ for each instance of character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "In-Game Information")
	FString InGameDescription;
	
	/** Animation montage containing animations for character flinch */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* FlinchMontage;

	/** Animation montage containing animations for character getting interrupted */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* InterruptMontage;

	/** Animation montage containing animations for character getting knocked down */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* KnockdownMontage;

	/** Animation montage containing animations for character getting stunned */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* StunMontage;

	/** Animation montage containing animations for character attack getting deflected */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* AttackDeflectMontage;

	/** Animation montage containing animations for character blocking an incoming attack */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* BlockAttackMontage;

	/** Animation montage containing animations for character dodging */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* DodgeMontage;

	/** Animation montage containing animations for character dodging */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* DieMontage;

	// --------------------------------------
	//  Widgets
	// --------------------------------------

	void UpdateHealthWidget();

	// --------------------------------------
	//  Replicated Stats
	// --------------------------------------

	/** [server] Event called on server when character's health changes */
	virtual void OnHealthUpdated(int32 BaseHealth, int32 MaxHealth, int32 CurrentHealth) override;

	// --------------------------------------
	//  Skill System
	// --------------------------------------

	/** Returns true if character can use any skill at all */
	virtual bool CanUseAnySkill() const;

	virtual bool CanUseSkill(FName SkillID, UGameplaySkillBase* Skill = nullptr);

	/** Use a skill and play it's animation */
	virtual bool UseSkill_Implementation(FName SkillID, UGameplaySkillBase* Skill = nullptr) override;

	/**
	 * Determines and returns the status of a skill
	 * Returns EEODTaskStatus::Active if character is currently using the skill
	 * Returns EEODTaskStatus::Finished if character has finished using the skill
	 * Returns EEODTaskStatus::Aborted if the skill was aborted before completion
	 * Returns EEODTaskStatus::Inactive if the character is using or have used a different skill
	 */
	virtual EEODTaskStatus CheckSkillStatus(FName SkillID) override;

	/** [AI] Returns the melee attack skill that is more appropriate to use in current state against the given enemy */
	virtual FName GetMostWeightedMeleeSkillID(const AEODCharacterBase* TargetCharacter) const override;

	// --------------------------------------
	//  Combat
	// --------------------------------------

	/** Set whether character is in combat or not */
	virtual void SetInCombat(bool bValue) override;

	/** Returns true if this AI character can currently assist an ally */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Behavior")
	bool CanAssistAlly();
	virtual bool CanAssistAlly_Implementation();

	/** Called when an ally requests assistance from this character */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Behavior")
	void AssistanceRequested(const AAICharacterBase* Requestor);
	virtual void AssistanceRequested_Implementation(const AAICharacterBase* Requestor);

	// --------------------------------------
	//  Character States
	// --------------------------------------

	/** Updates character movement every frame */
	virtual void UpdateMovement(float DeltaTime) override;

	/** Updates character rotation every frame */
	virtual void UpdateRotation(float DeltaTime) override;

protected:

	// --------------------------------------
	//  Network
	// --------------------------------------

	virtual void OnRep_InCombat() override;
	virtual void OnRep_Health(FCharacterStat& OldHealth) override;
	virtual void OnRep_LastReceivedHit(const FReceivedHitInfo& OldHitInfo) override;

};
