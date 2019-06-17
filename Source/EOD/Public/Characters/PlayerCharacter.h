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
class USphereComponent;
class UDialogueWindowWidget;
class UWeaponDataAsset;
class APlayerCharacter;

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

	UFUNCTION(BlueprintCallable, Category = "Character Interaction")
	void DisplayDialogueWidget(FName DialogueWindowID);

	UFUNCTION(BlueprintCallable, Category = "Character Interaction")
	void RemoveDialogueWidget();
	
	UFUNCTION(BlueprintCallable, Category = PlayerInteraction)
	void FocusCameraOnActor(AActor* TargetActor);
	
	/** End any interaction currently in progress */
	UFUNCTION(BlueprintCallable, Category = PlayerInteraction)
	void EndInteraction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = PlayerInteraction)
	void RequestDialogue(AActor* Requestor, FName DialogueWindowID);
	virtual void RequestDialogue_Implementation(AActor* Requestor, FName DialogueWindowID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = PlayerInteraction)
	void FinishDialogue(UDialogueWindowWidget* Widget);
	virtual void FinishDialogue_Implementation(UDialogueWindowWidget* Widget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = PlayerInteraction)
	void ExitDialogue(UDialogueWindowWidget* Widget);
	virtual void ExitDialogue_Implementation(UDialogueWindowWidget* Widget);

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerInteraction)
	TSubclassOf<UDialogueWindowWidget> DialogueWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerInteraction)
	USoundBase* DialogueTriggeredSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerInteraction)
	USoundBase* DialogueEndSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerInteraction)
	USoundBase* InteractiveActorDetectedSound;
		
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Character Interaction")
	TArray<AActor*> OverlappingInteractiveActors;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Character Interaction")
	AActor* ActiveInteractiveActor;
	
	UPROPERTY(Transient, BlueprintReadWrite, Category = PlayerInteraction)
	UDialogueWindowWidget* DialogueWidget;

public:

	// --------------------------------------
	//  
	// --------------------------------------

	/** Called on an animation montage blending out to clean up, reset, or change any state variables */
	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted) override;

	/** Called on an animation montage ending to clean up, reset, or change any state variables */
	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) override;

	virtual void TurnOnTargetSwitch() override;

	virtual void TurnOffTargetSwitch() override;

	void OnSkillGroupAddedToSkillBar(const FString& SkillGroup);

	void OnSkillGroupRemovedFromSkillBar(const FString& SkillGroup);

	FORCEINLINE bool SkillHasDirectionalAnimations() const;

private:
	
	UPROPERTY(Transient)
	bool bSkillHasDirectionalAnimations;

public:

	// --------------------------------------
	//  Replicated Stats
	// --------------------------------------

	virtual void SetCharacterLevel(int32 NewLevel) override;

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

	virtual void OnRep_PrimaryWeaponID() override;
	virtual	void OnRep_SecondaryWeaponID() override;

	//~ Begin AEODCharacterBase RPC overrides
	virtual void OnRep_CharacterStateInfo(const FCharacterStateInfo& OldStateInfo) override;
	virtual void Server_Dodge_Implementation(uint8 DodgeIndex, float RotationYaw) override;
	virtual void Server_NormalAttack_Implementation(uint8 AttackIndex) override;
	//~ End AEODCharacterBase RPC overrides

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetPrimaryWeaponID(FName NewWeaponID);
	virtual void Server_SetPrimaryWeaponID_Implementation(FName NewWeaponID);
	virtual bool Server_SetPrimaryWeaponID_Validate(FName NewWeaponID);

};

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
