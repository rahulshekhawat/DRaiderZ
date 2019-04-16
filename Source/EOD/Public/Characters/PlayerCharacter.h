// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "HumanCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter.generated.h"


class USkillBarWidget;
class UAudioComponent;
class UAnimMontage;
class UPlayerAnimInstance;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class USkillsComponent;
class USphereComponent;
class UDialogueWindowWidget;
class UWeaponDataAsset;
class APlayerCharacter;

/** Delegate for when a player changes it's weapon */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponChangedMCDelegate, FName, WeaponID, UWeaponDataAsset*, WeaponDataAsset);

/**
 * PlayerCharacter is the base class for playable characters
 */
UCLASS()
class EOD_API APlayerCharacter : public AHumanCharacter
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	/** Create and initialize skeletal armor mesh, camera, and inventory components. */
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	/** Property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/** Spawn default weapon(s) */
	virtual void PostInitializeComponents() override;

	/** Initializes player animation references. Creates player HUD widget and adds it to the viewport. */
	virtual void BeginPlay() override;

	/** Updates player states */
	virtual void Tick(float DeltaTime) override;

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;

	// --------------------------------------
	//	Load/Save System
	// --------------------------------------

	/** Saves current player state */
	virtual void SaveCharacterState() override;


	// --------------------------------------
	//	Components
	// --------------------------------------

	FORCEINLINE UAudioComponent* GetSystemAudioComponent() const { return SystemAudioComponent; }

	static const FName SystemAudioComponentName;

protected:

	/** Audio component for playing system messages */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	UAudioComponent* SystemAudioComponent;

public:

	// --------------------------------------
	//  Character States
	// --------------------------------------

	/** Start dodging */
	virtual void StartDodge() override;

	/** Cancel dodging */
	virtual void CancelDodge() override;

	/** Finish dodging */
	virtual void FinishDodge() override;

	/** Start normal attacks */
	virtual void StartNormalAttack() override;

	/** Cancel normal attacks */
	virtual void CancelNormalAttack() override;

	/** Finish normal attacks and reset back to Idle-Walk-Run */
	virtual void FinishNormalAttack() override;

	/** Updates character normal attck state every frame if the character wants to normal attack */
	virtual void UpdateNormalAttackState(float DeltaTime) override;

	FName GetNextNormalAttackSectionName(const FName& CurrentSection) const;

	void ChangeNormalAttackSection(FName OldSection, FName NewSection);

	/** Put or remove weapon inside sheath */
	virtual void ToggleSheathe() override;

	virtual void StartWeaponSwitch() override;

	virtual void CancelWeaponSwitch() override;

	virtual void FinishWeaponSwitch() override;

	/** Plays BlockAttack animation on blocking an incoming attack */
	virtual void PlayAttackBlockedAnimation() override;

	/** Returns true if character can move */
	virtual bool CanMove() const override;

	/** Returns true if character can jump */
	virtual bool CanJump() const override;

	/** Returns true if character can dodge */
	virtual bool CanDodge() const override;

	/** Returns true if character can guard against incoming attacks */
	virtual bool CanGuardAgainstAttacks() const override;

	/** Returns true if character can use normal attack */
	virtual bool CanNormalAttack() const override;

	/** Returns true if character can use any skill at all */
	virtual bool CanUseAnySkill() const;

	// --------------------------------------
	//  Sounds
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	FPlayerSystemSounds SystemSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	FPlayerGameplaySounds GameplaySounds;

	// --------------------------------------
	//  Skill System
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Skill System")
	void SetCanUseChainSkill(bool bNewValue);

	// --------------------------------------
	//  Rotation
	// --------------------------------------

	//~ DEPRECATED
	/** Determines if this character should be rotated toward DesiredSmoothRotationYaw */
	UPROPERTY(Transient)
	bool bRotateSmoothly;

	//~ DEPRECATED
	/** The yaw to which the character needs to be rotated smoothly */
	UPROPERTY(Transient)
	float DesiredSmoothRotationYaw;

	//~ DEPRECATED
	FORCEINLINE void SetOffSmoothRotation(float DesiredYaw);

	// --------------------------------------
	//  Weapon System
	// --------------------------------------

	UFUNCTION()
	void ActivateStatusEffectFromWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset);

	UFUNCTION()
	void DeactivateStatusEffectFromWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset);

	/** Add or replace primary weapon with a new weapon */
	void AddPrimaryWeapon(FName WeaponID);

	/** Add or replace secondary weapon with a new weapon */
	void AddSecondaryWeapon(FName WeaponID);

	UPROPERTY()
	int32 ActiveWeaponSlotIndex;

	void AddPrimaryWeaponToCurrentSlot(FName WeaponID);

	void AddPrimaryWeaponToCurrentSlot(FName WeaponID, UWeaponDataAsset* WeaponDataAsset);

	void AddSecondaryWeaponToCurrentSlot(FName WeaponID);

	void AddSecondaryWeaponToCurrentSlot(FName WeaponID, UWeaponDataAsset* WeaponDataAsset);

	void AddPrimaryWeaponToSlot(FName WeaponID, int32 SlotIndex);

	void AddPrimaryWeaponToSlot(FName WeaponID, UWeaponDataAsset* WeaponDataAsset, int32 SlotIndex);

	void AddSecondaryWeaponToSlot(FName WeaponID, int32 SlotIndex);

	void AddSecondaryWeaponToSlot(FName WeaponID, UWeaponDataAsset* WeaponDataAsset, int32 SlotIndex);

	void RemovePrimaryWeaponFromCurrentSlot();

	void RemoveSecondaryWeaponFromCurrentSlot();

	void RemovePrimaryWeaponFromSlot(int32 SlotIndex);

	void RemoveSecondaryWeaponFromSlot(int32 SlotIndex);

	void ToggleWeaponSlot();




	/** [server + local] Change idle-walk-run direction of character */
	// inline void SetIWRCharMovementDir(ECharMovementDirection NewDirection);

	/** Event called when a new normal attack section starts playing */
	void OnNormalAttackSectionStart(FName SectionName);

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayStatusMessage(const FString& Message);

	/** Called on an animation montage blending out to clean up, reset, or change any state variables */
	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted) override;

	/** Called on an animation montage ending to clean up, reset, or change any state variables */
	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) override;

	virtual void TurnOnTargetSwitch() override;

	virtual void TurnOffTargetSwitch() override;

	void OnSkillGroupAddedToSkillBar(const FString& SkillGroup);

	void OnSkillGroupRemovedFromSkillBar(const FString& SkillGroup);


	// inline FPlayerAnimationReferencesTableRow* GetActiveAnimationReferences() const;

	// inline FPlayerAnimationReferencesTableRow* GetEquippedWeaponAnimationReferences() const;

	// FORCEINLINE bool SkillAllowsMovement() const;

	FORCEINLINE bool SkillHasDirectionalAnimations() const;

	FORCEINLINE void SetNormalAttackSectionChangeAllowed(bool bNewValue);

	UFUNCTION(BlueprintCallable, Category = Skills, meta = (DisplayName = "Set Normal Attack Section Change Allowed"))
	void BP_SetNormalAttackSectionChangeAllowed(bool bNewValue);

	/** On beginning overlap with an interactive actors */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = PlayerInteraction)
	void OnInteractionSphereBeginOverlap(AActor* OtherActor);

	/** On beginning overlap with an interactive actors */
	virtual void OnInteractionSphereBeginOverlap_Implementation(AActor* OtherActor);

	/** On ending overlap with an interactive actors */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = PlayerInteraction)
	void OnInteractionSphereEndOverlap(AActor* OtherActor);

	/** On ending overlap with an interactive actors */
	virtual void OnInteractionSphereEndOverlap_Implementation(AActor* OtherActor);

	UPROPERTY(Transient, BlueprintReadWrite, Category = PlayerInteraction)
	TArray<AActor*> OverlappingInteractiveActors;

	UPROPERTY(Transient, BlueprintReadWrite, Category = PlayerInteraction)
	AActor* ActiveInteractiveActor;

	UPROPERTY(Transient, BlueprintReadWrite, Category = PlayerInteraction)
	UDialogueWindowWidget* DialogueWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerInteraction)
	TSubclassOf<UDialogueWindowWidget> DialogueWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerInteraction)
	USoundBase* DialogueTriggeredSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerInteraction)
	USoundBase* DialogueEndSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerInteraction)
	USoundBase* InteractiveActorDetectedSound;

	/** Attempts to start interaction with the active interactive actor */
	// UFUNCTION(BlueprintCallable, Category = PlayerInteraction)
	virtual void StartInteraction() override;

	// UFUNCTION(BlueprintCallable, Category = PlayerInteraction)
	virtual void UpdateInteraction() override;

	// virtual void UpdateInteraction_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = PlayerInteraction)
	void RequestDialogue(AActor* Requestor, FName DialogueWindowID);

	virtual void RequestDialogue_Implementation(AActor* Requestor, FName DialogueWindowID);

	UFUNCTION(BlueprintCallable, Category = PlayerInteraction)
	void FocusCameraOnActor(AActor* TargetActor);

	// UFUNCTION(BlueprintCallable, Category = PlayerInteraction)
	// void UpdateActiveInteraction();

	/** End any interaction currently in progress */
	UFUNCTION(BlueprintCallable, Category = PlayerInteraction)
	void EndInteraction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = PlayerInteraction)
	void FinishDialogue(UDialogueWindowWidget* Widget);

	virtual void FinishDialogue_Implementation(UDialogueWindowWidget* Widget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = PlayerInteraction)
	void ExitDialogue(UDialogueWindowWidget* Widget);

	virtual void ExitDialogue_Implementation(UDialogueWindowWidget* Widget);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Combat|Weapons")
	FOnWeaponChangedMCDelegate OnPrimaryWeaponEquipped;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Combat|Weapons")
	FOnWeaponChangedMCDelegate OnPrimaryWeaponUnequipped;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Combat|Weapons")
	FOnWeaponChangedMCDelegate OnSecondaryWeaponEquipped;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Combat|Weapons")
	FOnWeaponChangedMCDelegate OnSecondaryWeaponUnequipped;

	UPROPERTY(EditDefaultsOnly, Category = "Player UI")
	TSubclassOf<USkillBarWidget> SkillBarWidgetClass;

private:

	TArray<UStatusEffectBase*> ManagedStatusEffectsList;

	TArray<UStatusEffectBase*> InflictedStatusEffectsList;

	// TArray<FCombatEvent> EventsOnUsingSkill;
	TArray<FOnGameplayEventMCDelegate> EventsOnUsingSkill;

	TArray<FOnGameplayEventMCDelegate> EventsOnSuccessfulSkillAttack;
	// TArray<FCombatEvent> EventsOnSuccessfulSkillAttack;


	////////////////////////////////////////////////////////////////////////////////
	// WEAPONS
	////////////////////////////////////////////////////////////////////////////////
private:
	/** An actor for primary weapon equipped by the player */
	// UPROPERTY(Transient)
	// APrimaryWeapon* PrimaryWeapon;


	FORCEINLINE void SetPrimaryWeaponID(FName NewWeaponID);

protected:

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void EquipPrimaryWeapon(FName WeaponID);

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Slot")
	int32 MaxWeaponSlots;

	void SetActiveWeaponSlotIndex(int32 NewSlotIndex);


private:


	UPROPERTY()
	UWeaponDataAsset* PrimaryWeaponDataAsset;

	UPROPERTY()
	UWeaponDataAsset* SecondaryWeaponDataAsset;

	UPROPERTY(Transient)
	bool bSkillHasDirectionalAnimations;

	UPROPERTY(Transient)
	bool bNormalAttackSectionChangeAllowed;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Skills, meta = (AllowPrivateAccess = "true"))
	uint8 MaxNumberOfSkills;


private:





	//~ @note Pressing and releasing skill keys are separate events to support charge events (e.g. charge rage)

	



	////////////////////////////////////////////////////////////////////////////////
	// ACTIONS
	////////////////////////////////////////////////////////////////////////////////
private:


	/** Display or hide character stats UI */
	void OnToggleCharacterStatsUI();

	void UpdateIdleState(float DeltaTime);

	void UpdateFastMovementState(float DeltaTime);

	void UpdateAutoRun(float DeltaTime);


public:

	// --------------------------------------
	//  Effects
	// --------------------------------------

	virtual void CreateGhostTrail_Implementation() override;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	FORCEINLINE ECharacterGender GetCharacterGender() const;

	inline FName GetNormalAttackSectionName(uint8 AttackIndex);

	inline uint8 GetNormalAttackIndex(FName SectionName);

	/** Get the suffix string from the normal attack section */
	inline FString GetNormalAttackSuffix(FName NormalAttackSection) const;

	// --------------------------------------
	//  Dialgoue System
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Character Interaction")
	void DisplayDialogueWidget(FName DialogueWindowID);

	UFUNCTION(BlueprintCallable, Category = "Character Interaction")
	void RemoveDialogueWidget();

	// --------------------------------------
	//  Crowd Control Effect
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
	//  Input Handling
	// --------------------------------------

	/** Display or hide mouse cursor */
	void OnToggleMouseCursor();

	/** Handles player pressing a skill key */
	void OnPressingSkillKey(const uint32 SkillButtonIndex);

	/** Handles player releasing a skill key */
	void OnReleasingSkillKey(const uint32 SkillButtonIndex);

	/** Called when player presses a skill key */
	template<uint32 SkillButtonIndex>
	void PressedSkillKey();

	/** Called when player releases a skill key */
	template<uint32 SkillButtonIndex>
	void ReleasedSkillKey();

protected:

	// --------------------------------------
	//  Network
	// --------------------------------------

	virtual void OnRep_PrimaryWeaponID() override;
	virtual	void OnRep_SecondaryWeaponID() override;

	//~ Begin AEODCharacterBase RPC overrides
	virtual void OnRep_CharacterStateInfo(const FCharacterStateInfo& OldStateInfo) override;
	virtual void Server_Dodge_Implementation(uint8 DodgeIndex, float RotationYaw);
	virtual void Multicast_Dodge_Implementation(uint8 DodgeIndex, float RotationYaw);
	virtual void Server_NormalAttack_Implementation(uint8 AttackIndex);
	//~ End AEODCharacterBase RPC overrides

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetPrimaryWeaponID(FName NewWeaponID);
	virtual void Server_SetPrimaryWeaponID_Implementation(FName NewWeaponID);
	virtual bool Server_SetPrimaryWeaponID_Validate(FName NewWeaponID);

};

inline FName APlayerCharacter::GetNormalAttackSectionName(uint8 AttackIndex)
{
	if (AttackIndex == 1)
	{
		return UCharacterLibrary::SectionName_FirstSwing;
	}
	else if (AttackIndex == 2)
	{
		return UCharacterLibrary::SectionName_SecondSwing;
	}
	else if (AttackIndex == 3)
	{
		return UCharacterLibrary::SectionName_ThirdSwing;
	}
	else if (AttackIndex == 4)
	{
		return UCharacterLibrary::SectionName_FourthSwing;
	}
	else if (AttackIndex == 5)
	{
		return UCharacterLibrary::SectionName_FifthSwing;
	}
	else if (AttackIndex == 11)
	{
		return UCharacterLibrary::SectionName_ForwardSPSwing;
	}
	else if (AttackIndex == 12)
	{
		return UCharacterLibrary::SectionName_BackwardSPSwing;
	}
	else
	{
		return NAME_None;
	}
}

inline uint8 APlayerCharacter::GetNormalAttackIndex(FName SectionName)
{
	if (SectionName == NAME_None)
	{
		return 0;
	}
	else if (SectionName == UCharacterLibrary::SectionName_FirstSwing)
	{
		return 1;
	}
	else if (SectionName == UCharacterLibrary::SectionName_SecondSwing)
	{
		return 2;
	}
	else if (SectionName == UCharacterLibrary::SectionName_ThirdSwing)
	{
		return 3;
	}
	else if (SectionName == UCharacterLibrary::SectionName_FourthSwing)
	{
		return 4;
	}
	else if (SectionName == UCharacterLibrary::SectionName_FifthSwing)
	{
		return 5;
	}
	else if (SectionName == UCharacterLibrary::SectionName_ForwardSPSwing)
	{
		return 11;
	}
	else if (SectionName == UCharacterLibrary::SectionName_BackwardSPSwing)
	{
		return 12;
	}
	else
	{
		return 0;
	}
}

inline FString APlayerCharacter::GetNormalAttackSuffix(FName NormalAttackSection) const
{
	if (NormalAttackSection == UCharacterLibrary::SectionName_FirstSwing)
	{
		return FString("1");
	}
	else if (NormalAttackSection == UCharacterLibrary::SectionName_SecondSwing)
	{
		return FString("2");
	}
	else if (NormalAttackSection == UCharacterLibrary::SectionName_ThirdSwing)
	{
		return FString("3");
	}
	else if (NormalAttackSection == UCharacterLibrary::SectionName_FourthSwing)
	{
		return FString("4");
	}
	else if (NormalAttackSection == UCharacterLibrary::SectionName_FifthSwing)
	{
		return FString("5");
	}
	else if (NormalAttackSection == UCharacterLibrary::SectionName_BackwardSPSwing)
	{
		return FString("BSP");
	}
	else if (NormalAttackSection == UCharacterLibrary::SectionName_ForwardSPSwing)
	{
		return FString("FSP");
	}
	else
	{
		return FString("");
	}
}

FORCEINLINE void APlayerCharacter::SetPrimaryWeaponID(FName NewWeaponID)
{
	if (Role < ROLE_Authority)
	{
		Server_SetPrimaryWeaponID(NewWeaponID);
	}
	else
	{
		PrimaryWeaponID = NewWeaponID;
		SetCurrentPrimaryWeapon(NewWeaponID);
	}
}

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

FORCEINLINE ECharacterGender APlayerCharacter::GetCharacterGender() const
{
	return Gender;
}

FORCEINLINE void APlayerCharacter::SetOffSmoothRotation(float DesiredYaw)
{
	bRotateSmoothly = true;
	DesiredSmoothRotationYaw = DesiredYaw;
}

FORCEINLINE bool APlayerCharacter::SkillHasDirectionalAnimations() const
{
	return bSkillHasDirectionalAnimations;
}

FORCEINLINE void APlayerCharacter::SetNormalAttackSectionChangeAllowed(bool bNewValue)
{
	bNormalAttackSectionChangeAllowed = bNewValue;
}
