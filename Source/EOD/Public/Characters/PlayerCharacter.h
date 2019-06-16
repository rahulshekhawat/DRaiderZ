// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "HumanCharacter.h"
#include "EODBlueprintFunctionLibrary.h"

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
class USphereComponent;
class UDialogueWindowWidget;
class UWeaponDataAsset;
class APlayerCharacter;

/** Delegate for when a player changes it's weapon */
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponChangedMCDelegate, FName, WeaponID, UWeaponDataAsset*, WeaponDataAsset);

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

	virtual void LoadCharacterState() override;

	// --------------------------------------
	//  Combat Interface
	// --------------------------------------

	virtual TSharedPtr<FAttackInfo> GetAttackInfoPtr(const FName& SkillGroup, const int32 CollisionIndex) override;

	/** [server] Called to process the post attack event */
	virtual void PostAttack(const TArray<FAttackResponse>& AttackResponses, const TArray<AActor*> HitActors);

	/** Returns the sound that should be played when this character hits a physical surface */
	virtual USoundBase* GetMeleeHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const;

	/** Returns the sound that should be played when this character fails to hit anything */
	virtual USoundBase* GetMeleeHitMissSound() const override;

	UPROPERTY(ReplicatedUsing = OnRep_LastAttackResponses)
	TArray<FAttackResponse> LastAttackResponses;

	// --------------------------------------
	//	Components
	// --------------------------------------

	FORCEINLINE UAudioComponent* GetSystemAudioComponent() const { return SystemAudioComponent; }

	FORCEINLINE USphereComponent* GetInteractionSphere() const { return InteractionSphere; }

	static const FName SystemAudioComponentName;

	static const FName InteractionSphereComponentName;

protected:

	/** Audio component for playing system messages */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	UAudioComponent* SystemAudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Interaction")
	USphereComponent* InteractionSphere;

public:

	// --------------------------------------
	//  Character States
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Character Stats")
	void SwitchToInteractionState();

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

	/** Returns true if character can use any skill at all */
	virtual bool CanUseAnySkill() const override;

	// --------------------------------------
	//  Sounds
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	FPlayerSystemSounds SystemSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	FPlayerGameplaySounds GameplaySounds;

	UFUNCTION(BlueprintCallable, Category = "Sounds")
	void PlaySystemSound(USoundBase* SoundToPlay);

	inline USoundBase* GetGreatswordHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const;
	inline USoundBase* GetWarhammerHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const;
	inline USoundBase* GetLongswordHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const;
	inline USoundBase* GetMaceHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const;
	inline USoundBase* GetStaffHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const;
	inline USoundBase* GetDaggerHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const;

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
	//  Interaction
	// --------------------------------------

	/** Event called when interaction sphere begins overlap with interactive actors */
	UFUNCTION()
	void OnInteractionSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	/** Event called when interaction sphere ends overlap with interactive actors */
	UFUNCTION()
	void OnInteractionSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	virtual bool CanStartInteraction() const override;

	/** Trigger interaction with an NPC or an in-game interactive object */
	virtual void TriggerInteraction() override;

	virtual void StartInteraction() override;

	virtual void UpdateInteraction() override;

	virtual void CancelInteraction() override;

	virtual void FinishInteraction() override;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Character Interaction")
	TArray<AActor*> OverlappingInteractiveActors;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Character Interaction")
	AActor* ActiveInteractiveActor;

	UFUNCTION(BlueprintCallable, Category = "Character Interaction")
	void DisplayDialogueWidget(FName DialogueWindowID);

	UFUNCTION(BlueprintCallable, Category = "Character Interaction")
	void RemoveDialogueWidget();

	// --------------------------------------
	//  Weapon System
	// --------------------------------------

	/*
	UFUNCTION()
	void ActivateStatusEffectFromWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset);

	UFUNCTION()
	void DeactivateStatusEffectFromWeapon(FName WeaponID, UWeaponDataAsset* WeaponDataAsset);
	*/

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

	FORCEINLINE bool SkillHasDirectionalAnimations() const;

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

	/*
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Combat|Weapons")
	FOnWeaponChangedMCDelegate OnPrimaryWeaponEquipped;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Combat|Weapons")
	FOnWeaponChangedMCDelegate OnPrimaryWeaponUnequipped;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Combat|Weapons")
	FOnWeaponChangedMCDelegate OnSecondaryWeaponEquipped;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Combat|Weapons")
	FOnWeaponChangedMCDelegate OnSecondaryWeaponUnequipped;
	*/

private:

	TArray<UStatusEffectBase*> ManagedStatusEffectsList;

	TArray<UStatusEffectBase*> InflictedStatusEffectsList;

	// TArray<FCombatEvent> EventsOnUsingSkill;
	TArray<FOnGameplayEventMCDelegate> EventsOnUsingSkill;

	TArray<FOnGameplayEventMCDelegate> EventsOnSuccessfulSkillAttack;
	// TArray<FCombatEvent> EventsOnSuccessfulSkillAttack;

public:

	// --------------------------------------
	//  Replicated Stats
	// --------------------------------------

	virtual void SetCharacterLevel(int32 NewLevel) override;

	void AddEXP(int32 Value);

private:

	void SetLeveupEXP(int32 EXP);

	int32 LeveupEXP;

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


	// UPROPERTY()
	// UWeaponDataAsset* PrimaryWeaponDataAsset;

	// UPROPERTY()
	// UWeaponDataAsset* SecondaryWeaponDataAsset;

	UPROPERTY(Transient)
	bool bSkillHasDirectionalAnimations;


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


public:

	// --------------------------------------
	//  Effects
	// --------------------------------------

	virtual void CreateGhostTrail_Implementation() override;

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

	UFUNCTION()
	virtual void OnRep_LastAttackResponses(const TArray<FAttackResponse>& OldAttackResponses);

	virtual void OnRep_PrimaryWeaponID() override;
	virtual	void OnRep_SecondaryWeaponID() override;

	//~ Begin AEODCharacterBase RPC overrides
	virtual void OnRep_LastReceivedHit(const FReceivedHitInfo& OldHitInfo) override;
	virtual void OnRep_CharacterStateInfo(const FCharacterStateInfo& OldStateInfo) override;
	virtual void Server_Dodge_Implementation(uint8 DodgeIndex, float RotationYaw) override;
	virtual void Server_NormalAttack_Implementation(uint8 AttackIndex) override;
	//~ End AEODCharacterBase RPC overrides

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetPrimaryWeaponID(FName NewWeaponID);
	virtual void Server_SetPrimaryWeaponID_Implementation(FName NewWeaponID);
	virtual bool Server_SetPrimaryWeaponID_Validate(FName NewWeaponID);

};

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

FORCEINLINE void APlayerCharacter::SetOffSmoothRotation(float DesiredYaw)
{
	bRotateSmoothly = true;
	DesiredSmoothRotationYaw = DesiredYaw;
}

FORCEINLINE bool APlayerCharacter::SkillHasDirectionalAnimations() const
{
	return bSkillHasDirectionalAnimations;
}

inline USoundBase* APlayerCharacter::GetGreatswordHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const
{
	// Slash2
	USoundBase* Sound = nullptr;

	if (bCritHit)
	{
		Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Slash2CritHitSounds);
	}
	else
	{
		if (HitSurface == SURFACETYPE_FLESH)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Slash2FleshHitSounds);
		}
		else if (HitSurface == SURFACETYPE_METAL)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Slash2MetalHitSounds);
		}
		else if (HitSurface == SURFACETYPE_STONE)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Slash2StoneHitSounds);
		}
		else if (HitSurface == SURFACETYPE_UNDEAD)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Slash2UndeadHitSounds);
		}
		else if (HitSurface == SURFACETYPE_WOOD)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Slash2WoodHitSounds);
		}
		else
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Slash2FleshHitSounds);
		}
	}

	return Sound;
}

inline USoundBase* APlayerCharacter::GetWarhammerHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const
{
	// blunt3
	USoundBase* Sound = nullptr;

	if (bCritHit)
	{
		Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt3CritHitSounds);
	}
	else
	{
		if (HitSurface == SURFACETYPE_FLESH)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt3FleshHitSounds);
		}
		else if (HitSurface == SURFACETYPE_METAL)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt3MetalHitSounds);
		}
		else if (HitSurface == SURFACETYPE_STONE)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt3StoneHitSounds);
		}
		else if (HitSurface == SURFACETYPE_UNDEAD)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt3UndeadHitSounds);
		}
		else if (HitSurface == SURFACETYPE_WOOD)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt3WoodHitSounds);
		}
		else
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt3FleshHitSounds);
		}
	}

	return Sound;
}

inline USoundBase* APlayerCharacter::GetLongswordHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const
{
	// Slash1
	USoundBase* Sound = nullptr;

	if (bCritHit)
	{
		Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SlashCritHitSounds);
	}
	else
	{
		if (HitSurface == SURFACETYPE_FLESH)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SlashFleshHitSounds);
		}
		else if (HitSurface == SURFACETYPE_METAL)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SlashMetalHitSounds);
		}
		else if (HitSurface == SURFACETYPE_STONE)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SlashStoneHitSounds);
		}
		else if (HitSurface == SURFACETYPE_UNDEAD)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SlashUndeadHitSounds);
		}
		else if (HitSurface == SURFACETYPE_WOOD)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SlashWoodHitSounds);
		}
		else
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SlashFleshHitSounds);
		}
	}
	
	return Sound;
}

inline USoundBase* APlayerCharacter::GetMaceHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const
{
	// blunt1
	USoundBase* Sound = nullptr;

	if (bCritHit)
	{
		Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.BluntCritHitSounds);
	}
	else
	{
		if (HitSurface == SURFACETYPE_FLESH)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.BluntFleshHitSounds);
		}
		else if (HitSurface == SURFACETYPE_METAL)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.BluntMetalHitSounds);
		}
		else if (HitSurface == SURFACETYPE_STONE)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.BluntStoneHitSounds);
		}
		else if (HitSurface == SURFACETYPE_UNDEAD)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.BluntUndeadHitSounds);
		}
		else if (HitSurface == SURFACETYPE_WOOD)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.BluntWoodHitSounds);
		}
		else
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.BluntFleshHitSounds);
		}
	}
	
	return Sound;
}

inline USoundBase* APlayerCharacter::GetStaffHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const
{
	// blunt2
	USoundBase* Sound = nullptr;

	if (bCritHit)
	{
		Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt2CritHitSounds);
	}
	else
	{
		if (HitSurface == SURFACETYPE_FLESH)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt2FleshHitSounds);
		}
		else if (HitSurface == SURFACETYPE_METAL)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt2MetalHitSounds);
		}
		else if (HitSurface == SURFACETYPE_STONE)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt2StoneHitSounds);
		}
		else if (HitSurface == SURFACETYPE_UNDEAD)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt2UndeadHitSounds);
		}
		else if (HitSurface == SURFACETYPE_WOOD)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt2WoodHitSounds);
		}
		else
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.Blunt2FleshHitSounds);
		}
	}
	
	return Sound;
}

inline USoundBase* APlayerCharacter::GetDaggerHitSound(const TEnumAsByte<EPhysicalSurface> HitSurface, const bool bCritHit) const
{
	// slice
	USoundBase* Sound = nullptr;

	if (bCritHit)
	{
		Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SliceCritHitSounds);
	}
	else
	{
		if (HitSurface == SURFACETYPE_FLESH)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SliceFleshHitSounds);
		}
		else if (HitSurface == SURFACETYPE_METAL)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SliceMetalHitSounds);
		}
		else if (HitSurface == SURFACETYPE_STONE)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SliceStoneHitSounds);
		}
		else if (HitSurface == SURFACETYPE_UNDEAD)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SliceUndeadHitSounds);
		}
		else if (HitSurface == SURFACETYPE_WOOD)
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SliceWoodHitSounds);
		}
		else
		{
			Sound = UEODBlueprintFunctionLibrary::GetRandomSound(WeaponHitSounds.SliceFleshHitSounds);
		}
	}
	
	return Sound;
}
