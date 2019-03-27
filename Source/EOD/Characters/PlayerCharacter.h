// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Weapons/WeaponBase.h"
// #include "EOD/Weapons/WeaponSlot.h"
#include "EOD/Weapons/PrimaryWeapon.h"
#include "EOD/Weapons/SecondaryWeapon.h"
#include "EOD/Characters/HumanCharacter.h"

#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter.generated.h"

class UHUDWidget;
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

	/** Updates player states */
	virtual void Tick(float DeltaTime) override;

	/** Initializes player animation references. Creates player HUD widget and adds it to the viewport. */
	virtual void BeginPlay() override;

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;

	// --------------------------------------
	//	Load/Save System
	// --------------------------------------

	/** Saves current player state */
	virtual void SaveCharacterState() override;

	// --------------------------------------
	//	Strings
	// --------------------------------------

	/** Get the prefix string for equipped weapon type */
	inline FString GetEquippedWeaponPrefix() const;

	inline FString GetWeaponPrefix() const;

	/** Get the suffix string from the normal attack section */
	inline FString GetNormalAttackSuffix(FName NormalAttackSection) const;

	// --------------------------------------
	//	Components
	// --------------------------------------

	FORCEINLINE USkillsComponent* GetSkillsComponent() const { return SkillsComponent; }

private:
	//~ @note The default skeletal mesh component inherited from ACharacter class will reference the skeletal mesh for player face

	//~ Skills component - manages skills of character // DEPRECATED
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkillsComponent* SkillsComponent;

	//~ Audio component
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AudioComponent;

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








public:
	/** Determines if this character should be rotated toward DesiredSmoothRotationYaw */
	UPROPERTY(Transient)
	bool bRotateSmoothly;

	/** The yaw to which the character needs to be rotated smoothly */
	UPROPERTY(Transient)
	float DesiredSmoothRotationYaw;

	/** Returns true if character can move */
	virtual bool CanMove() const override;

	/** Returns true if character can jump */
	virtual bool CanJump() const override;

	/** Returns true if character can dodge */
	virtual bool CanDodge() const override;

	/** Returns true if character can guard against incoming attacks */
	virtual bool CanGuardAgainstAttacks() const override;

	/** Returns true if character can block */
	virtual bool CanBlock() const override;

	/** Returns true if character can use normal attack */
	virtual bool CanNormalAttack() const override;

	/** Returns true if character can use any skill at all */
	virtual bool CanUseAnySkill() const;

	/** Returns true if character can use a particular skill */
	virtual bool CanUseSkill(FSkillTableRow* Skill);

	/** Returns true if character can auto run */
	FORCEINLINE bool CanAutoRun() const;

	FORCEINLINE bool CanSwitchWeapon() const;

	FORCEINLINE bool CanToggleSheathe() const { return IsIdleOrMoving() && IsPrimaryWeaponEquippped(); }

	FORCEINLINE bool IsSwitchingWeapon() const;

	/** Returns true if player is on auto run */
	FORCEINLINE bool IsAutoRunning() const;

	/** Returns true if primary weapon is equipped */
	FORCEINLINE bool IsPrimaryWeaponEquippped() const;

	/** Returns true if secondary weapon is equipped */
	FORCEINLINE bool IsSecondaryWeaponEquipped() const;

	FORCEINLINE bool IsFastRunning() const;

	/** Returns primary weapon actor */
	FORCEINLINE APrimaryWeapon* GetPrimaryWeapon() const;

	/** Returns secondary weapon actor */
	FORCEINLINE ASecondaryWeapon* GetSecondaryWeapon() const;

	/** Returns the weapon type of primary weapon */
	virtual EWeaponType GetEquippedWeaponType() const override;

	/** Returns HUD widget */
	FORCEINLINE UHUDWidget* GetHUDWidget() const;

	FORCEINLINE ECharacterGender GetCharacterGender() const;

	/** Returns HUD widget */
	UFUNCTION(BlueprintPure, Category = UI, meta = (DisplayName = "Get HUD Widget"))
	UHUDWidget* BP_GetHUDWidget() const;

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

	/** [server + local] Applies stun to this character */
	virtual bool Stun(const float Duration) override;

	/** Removes 'stun' crowd control effect from this character */
	virtual void EndStun() override;

	/** [server + local] Freeze this character */
	virtual bool Freeze(const float Duration) override;

	/** Removes 'freeze' crowd control effect from this character */
	virtual void EndFreeze() override;

	/** [server + local] Knockdown this character */
	virtual bool Knockdown(const float Duration) override;

	/** Removes 'knock-down' crowd control effect from this character */
	virtual void EndKnockdown() override;

	/** [server + local] Knockback this character */
	virtual bool Knockback(const float Duration, const FVector& ImpulseDirection) override;

	virtual void BlockAttack() override;

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

	/** Removes primary weapon if it is currently equipped */
	void RemovePrimaryWeapon();

	/** Removes secondary weapon if it is currently equipped */
	void RemoveSecondaryWeapon();

	/** Replace primary weapon with a new weapon */
	void SetCurrentPrimaryWeapon(const FName WeaponID);

	/** Replace secondary weapon with a new weapon */
	void SetCurrentSecondaryWeapon(const FName WeaponID);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = SpecialEffects)
	void CreateGhostTrail();

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

	FORCEINLINE void SetOffSmoothRotation(float DesiredYaw);

	inline void EnableFastRun();

	inline void DisableFastRun();

	inline FPlayerAnimationReferencesTableRow* GetActiveAnimationReferences() const;

	inline FPlayerAnimationReferencesTableRow* GetEquippedWeaponAnimationReferences() const;

	FORCEINLINE bool SkillAllowsMovement() const;

	FORCEINLINE bool SkillHasDirectionalAnimations() const;

	FORCEINLINE void SetCanUseChainSkill(bool bNewValue);

	FORCEINLINE void SetNormalAttackSectionChangeAllowed(bool bNewValue);

	UFUNCTION(BlueprintCallable, Category = Skills, meta = (DisplayName = "Set Can Use Chain Skill"))
	void BP_SetCanUseChainSkill(bool bNewValue);

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

	const int CameraZoomRate = 15;

	const int CameraArmMinimumLength = 50;

	const int CameraArmMaximumLength = 500;

	// TArray<FCombatEvent> EventsOnUsingSkill;
	TArray<FOnGameplayEventMCDelegate> EventsOnUsingSkill;

	TArray<FOnGameplayEventMCDelegate> EventsOnSuccessfulSkillAttack;
	// TArray<FCombatEvent> EventsOnSuccessfulSkillAttack;

	/** Timer handle needed for executing SP normal attacks */
	FTimerHandle SPAttackTimerHandle;


	////////////////////////////////////////////////////////////////////////////////
	// WEAPONS
	////////////////////////////////////////////////////////////////////////////////
private:
	/** An actor for primary weapon equipped by the player */
	// UPROPERTY(Transient)
	// APrimaryWeapon* PrimaryWeapon;

	/** An actor for secondary weapon equipped by the player */
	UPROPERTY(Transient)
	ASecondaryWeapon* SecondaryWeapon;

	FORCEINLINE void SetPrimaryWeaponID(FName NewWeaponID);

protected:
	/** ID of the current primary weapon equipped. It will be NAME_None if no primary weapon is equipped */
	UPROPERTY(ReplicatedUsing = OnRep_PrimaryWeaponID, EditDefaultsOnly, BlueprintReadOnly)
	FName PrimaryWeaponID;
	
	/** ID of current secondary weapon equipped. It will be NAME_None if no secondary weapon is equipped */
	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeaponID, EditDefaultsOnly, BlueprintReadOnly)
	FName SecondaryWeaponID;

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
	bool bForwardPressed;

	UPROPERTY(Transient)
	bool bBackwardPressed;

	UPROPERTY(Transient)
	bool bBlockPressed;

	UPROPERTY(Transient)
	bool bCanUseChainSkill;

	UPROPERTY(Transient)
	bool bSkillHasDirectionalAnimations;

	UPROPERTY(Transient)
	bool bNormalAttackSectionChangeAllowed;

	/** Player HUD class reference */
	UPROPERTY(Transient)
	UHUDWidget* HUDWidget;

	/** The blueprint widget class to use for player HUD */
	UPROPERTY(EditDefaultsOnly, Category = RequiredInfo)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Skills, meta = (AllowPrivateAccess = "true"))
	uint8 MaxNumberOfSkills;


private:
	/** Data table containing player animation references */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UDataTable* PlayerAnimationReferencesDataTable;

	/** Animation references by weapon type */
	TMap<EWeaponType, FPlayerAnimationReferencesTableRow*> AnimationReferencesMap;

	/** Streamable handle for animation references by weapon type */
	TMap<EWeaponType, TSharedPtr<FStreamableHandle>> AnimationReferencesStreamableHandles;

	inline FPlayerAnimationReferencesTableRow* GetAnimationReferences() const;

	UFUNCTION()
	void LoadWeaponAnimationReferences(FName WeaponID, UWeaponDataAsset* WeaponDataAsset);

	UFUNCTION()
	void UnloadWeaponAnimationReferences(FName WeaponID, UWeaponDataAsset* WeaponDataAsset);


	/** Animations for this player when it has a weapon equipped */
	FPlayerAnimationReferencesTableRow* EquippedWeaponAnimationReferences;

	/** Animations for this player when it does not have a weapon equipped */
	FPlayerAnimationReferencesTableRow* UnequippedWeaponAnimationReferences;

	TSharedPtr<FStreamableHandle> EquippedWeaponAnimationsStreamableHandle;

	TSharedPtr<FStreamableHandle> UnequippedWeaponAnimationsStreamableHandle;


	FName GetAnimationReferencesRowID(EWeaponType WeaponType, ECharacterGender CharGender);

	TSharedPtr<FStreamableHandle> LoadAnimationReferences(FPlayerAnimationReferencesTableRow* AnimationReferences);

	void UnloadUnequippedWeaponAnimationReferences();

	void LoadUnequippedWeaponAnimationReferences();

	void UnloadEquippedWeaponAnimationReferences();

	void LoadEquippedWeaponAnimationReferences();


	//~ @note Pressing and releasing skill keys are separate events to support charge events (e.g. charge rage)

	/** Handles player pressing a skill key */
	void OnPressingSkillKey(const uint32 SkillButtonIndex);

	/** Handles player releasing a skill key */
	void OnReleasingSkillKey(const uint32 SkillButtonIndex);
	
	void ZoomInCamera();

	void ZoomOutCamera();

	void OnDodge();

public:
	virtual void OnPressedForward() override;

	virtual void OnPressedBackward() override;

private:
	/** Sets the boolean used to enable block to true */
	void OnPressedBlock();
	
	/** Sets the boolean used to enable block to false */
	void OnReleasedBlock();

	void OnPressedEscape();

	/** Enable damage blocking */
	inline void EnableBlock();

	/** Disable damage blocking */
	inline void DisableBlock();

	/** Event triggered when player presses the jump key */
	void OnJump();

	/** Interacts with NPC or an in-game interactable object */
	void OnInteract();

	////////////////////////////////////////////////////////////////////////////////
	// ACTIONS
	////////////////////////////////////////////////////////////////////////////////
private:
	/** Put or remove weapon inside sheath */
	virtual void ToggleSheathe() override;

	virtual void PlayToggleSheatheAnimation() override;

	/** Display or hide character stats UI */
	void OnToggleCharacterStatsUI();

	/** Display or hide mouse cursor */
	void OnToggleMouseCursor();

	void OnPressedNormalAttack();

	void OnReleasedNormalAttack();

	void DoNormalAttack();

	void UpdateIdleState(float DeltaTime);

	// void UpdateMovement(float DeltaTime);

	// virtual void UpdateGuardState(float DeltaTime) override;

	void UpdateFastMovementState(float DeltaTime);

	void UpdateAutoRun(float DeltaTime);
	
	virtual void StartNormalAttack() override;

	virtual void StopNormalAttack() override;

	virtual void UpdateNormalAttackState(float DeltaTime) override;

	/** Enable or disable auto run */
	void OnToggleAutoRun();

	inline void EnableAutoRun();

	inline void DisableAutoRun();

	void DisableForwardPressed();

	void DisableBackwardPressed();

	FName GetNextNormalAttackSectionName(const FName& CurrentSection) const;

	/** Called when player presses a skill key */
	template<uint32 SkillButtonIndex>
	void PressedSkillKey();

	/** Called when player releases a skill key */
	template<uint32 SkillButtonIndex>
	void ReleasedSkillKey();

	UFUNCTION(BlueprintCallable, Category = PlayerInteraction)
	void DisplayDialogueWidget(FName DialogueWindowID);

	UFUNCTION(BlueprintCallable, Category = PlayerInteraction)
	void RemoveDialogueWidget();

	/** Plays normal attack animation over network */
	void PlayNormalAttackAnimation(FName OldSection, FName NewSection);

private:

	// --------------------------------------
	//  Network
	// --------------------------------------

	UFUNCTION()
	void OnRep_PrimaryWeaponID();

	UFUNCTION()
	void OnRep_SecondaryWeaponID();

	virtual void Server_Dodge_Implementation(uint8 DodgeIndex, float RotationYaw);

	virtual bool Server_Dodge_Validate(uint8 DodgeIndex, float RotationYaw);

	virtual void Multicast_Dodge_Implementation(uint8 DodgeIndex, float RotationYaw);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayNormalAttackAnimation(FName OldSection, FName NewSection);

	UFUNCTION(NetMultiCast, Reliable)
	void Multicast_PlayNormalAttackAnimation(FName OldSection, FName NewSection);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetPrimaryWeaponID(FName NewWeaponID);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddPrimaryWeaponToCurrentSlot(FName WeaponID, UWeaponDataAsset* WeaponDataAsset);

	UFUNCTION(NetMultiCast, Reliable)
	void Multicast_AddPrimaryWeaponToCurrentSlot(FName WeaponID, UWeaponDataAsset* WeaponDataAsset, AWeaponBase* PrimaryWep);

};

inline FString APlayerCharacter::GetEquippedWeaponPrefix() const
{
	switch (GetEquippedWeaponType())
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

inline FString APlayerCharacter::GetWeaponPrefix() const
{
	FString ReturnString = FString("");
	switch (GetEquippedWeaponType())
	{
	case EWeaponType::GreatSword:
		ReturnString += FString("GS_");
		break;
	case EWeaponType::WarHammer:
		ReturnString += FString("WH_");
		break;
	case EWeaponType::LongSword:
		ReturnString += FString("LS_");
		break;
	case EWeaponType::Mace:
		ReturnString += FString("MC_");
		break;
	case EWeaponType::Dagger:
		ReturnString += FString("DG_");
		break;
	case EWeaponType::Staff:
		ReturnString += FString("ST_");
		break;
	case EWeaponType::Shield:
	case EWeaponType::None:
	default:
		break;
	}
	return ReturnString;
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

inline void APlayerCharacter::EnableAutoRun()
{
	SetCharacterState(ECharacterState::AutoRun);
	SetUseControllerRotationYaw(true);
}

inline void APlayerCharacter::DisableAutoRun()
{
	// @todo put a test to check if the player is really auto running

	SetCharacterState(ECharacterState::IdleWalkRun);
	SetUseControllerRotationYaw(false);
}

FORCEINLINE APrimaryWeapon* APlayerCharacter::GetPrimaryWeapon() const
{
	return PrimaryWeapon;
}

FORCEINLINE ASecondaryWeapon* APlayerCharacter::GetSecondaryWeapon() const
{
	return SecondaryWeapon;
}

FORCEINLINE UHUDWidget* APlayerCharacter::GetHUDWidget() const
{
	return HUDWidget;
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

inline void APlayerCharacter::EnableBlock()
{
	/*
	if (IsAutoRunning())
	{
		DisableAutoRun();
	}
	*/

	if (IsNormalAttacking())
	{
		StopNormalAttack();
	}

	SetCharacterState(ECharacterState::Blocking);
	SetUseControllerRotationYaw(true);
	SetWalkSpeed(DefaultWalkSpeedWhileBlocking * GetCharacterStatsComponent()->GetMovementSpeedModifier());

	// FTimerHandle TimerDelegate;
	GetWorld()->GetTimerManager().SetTimer(BlockTimerHandle, this, &APlayerCharacter::EnableDamageBlocking, DamageBlockTriggerDelay, false);
}

inline void APlayerCharacter::DisableBlock()
{
	SetUseControllerRotationYaw(false);
	SetCharacterState(ECharacterState::IdleWalkRun);
	DisableDamageBlocking();
}

inline void APlayerCharacter::EnableFastRun()
{
	if (IsIdleOrMoving())
	{
		SetCharacterState(ECharacterState::SpecialMovement);
	}
}

inline void APlayerCharacter::DisableFastRun()
{
	if (IsFastRunning())
	{
		SetCharacterState(ECharacterState::IdleWalkRun);
	}
}

inline FPlayerAnimationReferencesTableRow* APlayerCharacter::GetAnimationReferences() const
{
	EWeaponType WeaponType = EWeaponType::None;
	if (!IsWeaponSheathed())
	{
		WeaponType = GetEquippedWeaponType();
	}

	FPlayerAnimationReferencesTableRow* const* AnimationsPtr = AnimationReferencesMap.Find(WeaponType);
	return AnimationsPtr ? *AnimationsPtr : nullptr;
}

inline FPlayerAnimationReferencesTableRow* APlayerCharacter::GetActiveAnimationReferences() const
{
	if (IsWeaponSheathed() || GetEquippedWeaponType() == EWeaponType::None)
	{
		return UnequippedWeaponAnimationReferences;
	}

	return EquippedWeaponAnimationReferences;
}

inline FPlayerAnimationReferencesTableRow* APlayerCharacter::GetEquippedWeaponAnimationReferences() const
{
	return EquippedWeaponAnimationReferences;
}

FORCEINLINE bool APlayerCharacter::SkillAllowsMovement() const
{
	return bSkillAllowsMovement;
}

FORCEINLINE bool APlayerCharacter::SkillHasDirectionalAnimations() const
{
	return bSkillHasDirectionalAnimations;
}

FORCEINLINE void APlayerCharacter::SetCanUseChainSkill(bool bNewValue)
{
	bCanUseChainSkill = bNewValue;
}

FORCEINLINE void APlayerCharacter::SetNormalAttackSectionChangeAllowed(bool bNewValue)
{
	bNormalAttackSectionChangeAllowed = bNewValue;
}

FORCEINLINE bool APlayerCharacter::CanAutoRun() const
{
	return IsIdleOrMoving();
}

FORCEINLINE bool APlayerCharacter::CanSwitchWeapon() const
{
	return IsIdleOrMoving();
}

FORCEINLINE bool APlayerCharacter::IsSwitchingWeapon() const
{
	return GetCharacterState() == ECharacterState::SwitchingWeapon;
}

FORCEINLINE bool APlayerCharacter::IsAutoRunning() const
{
	return GetCharacterState() == ECharacterState::AutoRun;
}

FORCEINLINE bool APlayerCharacter::IsPrimaryWeaponEquippped() const
{
	return PrimaryWeaponID != NAME_None && PrimaryWeapon->IsAttachedToCharacterOwner();
}

FORCEINLINE bool APlayerCharacter::IsSecondaryWeaponEquipped() const
{
	return SecondaryWeaponID != NAME_None && SecondaryWeapon->IsAttachedToCharacterOwner();
}

FORCEINLINE bool APlayerCharacter::IsFastRunning() const
{
	return GetCharacterState() == ECharacterState::SpecialMovement;
}

/*
FORCEINLINE void APlayerCharacter::SetMasterPoseComponentForMeshes()
{
	if (GetMesh())
	{
		Hair->SetMasterPoseComponent(GetMesh());
		HatItem->SetMasterPoseComponent(GetMesh());
		FaceItem->SetMasterPoseComponent(GetMesh());
		Chest->SetMasterPoseComponent(GetMesh());
		Hands->SetMasterPoseComponent(GetMesh());
		Legs->SetMasterPoseComponent(GetMesh());
		Feet->SetMasterPoseComponent(GetMesh());
	}
}
*/
