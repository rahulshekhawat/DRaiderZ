// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponLibrary.h"
#include "UObject/NoExportTypes.h"
#include "CharacterLibrary.generated.h"

class UAnimMontage;


UENUM(BlueprintType)
enum class ECharMovementDirection : uint8
{
	None,
	F UMETA(DisplayName = "Forward"),
	B UMETA(DisplayName = "Backward"),
	L UMETA(DisplayName = "Left"),
	R UMETA(DisplayName = "Right"),
	FL UMETA(DisplayName = "Forward Left"),
	FR UMETA(DisplayName = "Forward Right"),
	BL UMETA(DisplayName = "Backward Left"),
	BR UMETA(DisplayName = "Backward Right"),
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	// None,
	IdleWalkRun,
	Jumping,
	Dodging,
	Blocking,
	Attacking,
	Looting,
	SpecialAction,
	Interacting, // Interacting with another character, i.e., engaged in dialogue
	UsingActiveSkill,
	CastingSpell,
	SpecialMovement,
	GotHit,
	Dead
};

USTRUCT(BlueprintType)
struct EOD_API FPlayerAnimationReferences : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AnimationMontage_Jump;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AnimationMontage_Dodge;
	
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// UAnimMontage* AnimationMontage_WeaponChange;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AnimationMontage_NormalAttacks;

	//~ @note Add AnimationMontage_WeaponChange animations here
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AnimationMontage_SpecialActions;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AnimationMontage_Skills;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AnimationMontage_Spells;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AnimationMontage_SpecialMovement;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AnimationMontage_CrowdControlEffects;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AnimationMontage_Flinch;

};


/**
 * 
 */
UCLASS()
class EOD_API UCharacterLibrary : public UObject
{
	GENERATED_BODY()
	
public:

	UCharacterLibrary(const FObjectInitializer& ObjectInitializer);
	
	// This function cannot be BlueprintCallable
	static FPlayerAnimationReferences* GetPlayerAnimationReferences(EWeaponAnimationType PlayerWeaponAnimationType);

};
