// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Player/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UAnimMontage;
class USkeletalMeshComponent;
class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class EOD_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Property replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#if WITH_EDITOR
	/**
	 * Detects changes to EditorArmorID and EditorWeaponID properties.
	 * If the ID changes, it looks up for respective weapon or armor in
	 * data tables and adds them to character.
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;
	
public:
	
	//~ Begin Functions to Assist Input Handling
	/** Player used a skill */
	void OnUsingSkill(uint32 SkillButtonIndex);
	
	/** Converts player controller rotation from 0/360 range to -180/180 range and returns it. */
	UFUNCTION(BlueprintCallable, category = PlayerRotationHandler)
	float GetPlayerControlRotationYaw();
	
	/** Delta rotate player toward desired yaw */
	UFUNCTION(BlueprintCallable, category = PlayerRotationHandler)
	bool DeltaRotatePlayerToDesiredYaw(float DesiredYaw, float DeltaTime, float RotationRate = 600.f);
	
	/**
	 * Returns the expected rotation yaw of character based on current Axis Input.
	 * NOTE: Only call for locally controlled character otherwise it would lead to crash (intentional)
	 */
	UFUNCTION(BlueprintCallable, category = PlayerRotationHandler)
	float GetRotationYawFromAxisInput();
	//~ End Functions to Assist Input Handling
	
private:

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;


	//~ Begin Weapon Components Declaration
	// UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// UStaticMeshComponent* PrimaryWeapon;

	// UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// UStaticMeshComponent* SecondaryWeapon;

	// UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// UStaticMeshComponent* SheathedPrimaryWeapon;

	// UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// UStaticMeshComponent* SheathedSecondaryWeapon;

	// UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// UStaticMeshComponent* DroppedPrimaryWeapon;

	// UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// UStaticMeshComponent* DroppedSecondaryWeapon;
	//~ End Weapon Components Declaration


	//~ Begin Armor Components Declaration
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Hair;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* HatItem;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FaceItem;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Chest;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Hands;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Legs;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Feet;
	//~ End Armor Components Declaration


	//~ Inventory
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;

	/** Creates new armor component. Helper function for constructor */
	USkeletalMeshComponent* CreateNewArmorComponent(FName Name, const FObjectInitializer& ObjectInitializer);

	/** Creates new armor component. Helper function for constructor */
	UStaticMeshComponent* CreateNewWeaponComponent(FName Name, const FObjectInitializer& ObjectInitializer);


	//~ Begin Input Handling Variables
	bool bWPressed;

	bool bAPressed;

	bool bDPressed;

	bool bSPressed;
	
	bool bBlockPressed;

	bool bAttackPressed;
	
	const int CameraZoomRate = 15;

	const int CameraArmMinimumLength = 50;

	const int CameraArmMaximumLength = 500;
	//~ End Input Handling Variables
	

	//~ Begin Character Action State Handlers
	virtual bool CanMove() const override;
	
	virtual bool CanJump() const override;

	virtual bool CanDodge() const override;
	
	virtual bool CanNormalAttack() const;
	//~ End Character Action State Handlers


	//~ Begin Input Handling Functions
	/** Move player forward/backward */
	void MoveForward(const float Value);

	/** Move player left/right */
	void MoveRight(const float Value);
	
	void ZoomInCamera();

	void ZoomOutCamera();

	void OnDodge();

	void EnableBlock();
	
	void DisableBlock();

	void StartNormalAttack();

	void StopNormalAttack();

	void OnJump();

	void OnInteract();

	void OnToggleSheath();

	void OnToggleCharacterStatsUI();

	void OnToggleMouseCursor();

	void OnNormalAttack();
	//~ End Input Handling Functions

	
	//~ Begin template functions
	/** Player pressed a skill action key */
	template<uint32 SkillButtonIndex>
	void OnPressedSkillKey();

	/** Player released a skill action key */
	template<uint32 SkillButtonIndex>
	void OnReleasedSkillKey();

	// template<typename StructType>
	// void AddStatsToCharacter(StructType* StructData);

	// template<typename StructType>
	// void RemoveStatsFromCharacter(StructType* StructData);
	//~ End template functions


	class UPlayerAnimInstance* PlayerAnimInstance;
	// class UHumanPlayerAnimInstance* PlayerAnimInstance;
	
	float MaxPlayerWalkSpeed = 400.f;

	void UpdateIdleState(float DeltaTime);

	void UpdateMovement(float DeltaTime);

	void UpdateBlockState(float DeltaTime);

public:

	FPlayerAnimationReferences* PlayerAnimationReferences = nullptr;

	void UpdatePlayerAnimationReferences();
	
	//~ Begin multiplayer code
	/** Character movement direction for Idle Walk Run */
	UPROPERTY(Replicated)
	ECharMovementDirection IWR_CharacterMovementDirection;
	
	void SetIWRCharMovementDir(ECharMovementDirection NewDirection);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIWRCharMovementDir(ECharMovementDirection NewDirection);
	
	UPROPERTY(Replicated)
	EWeaponAnimationType CurrentWeaponAnimationToUse;
	
	void SetCurrentWeaponAnimationToUse(EWeaponAnimationType NewWeaponAnimationType);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCurrentWeaponAnimationToUse(EWeaponAnimationType NewWeaponAnimationType);

	UPROPERTY(Replicated)
	float BlockMovementDirectionYaw;

	void SetBlockMovementDirectionYaw(float NewYaw);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetBlockMovementDirectionYaw(float NewYaw);

	//~ End multiplayer code

};

template<uint32 SkillButtonIndex>
inline void APlayerCharacter::OnPressedSkillKey()
{
	OnUsingSkill(SkillButtonIndex);
}

template<uint32 SkillButtonIndex>
inline void APlayerCharacter::OnReleasedSkillKey()
{
}
