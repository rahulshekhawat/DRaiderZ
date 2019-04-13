// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayEffectBase.generated.h"

class AEODCharacterBase;
class UGameplaySkillsComponent;

/**
 * 
 */
UCLASS()
class EOD_API UGameplayEffectBase : public UObject
{
	GENERATED_BODY()

public:

	UGameplayEffectBase(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//	Gameplay Effect Interface
	// --------------------------------------

	virtual void InitEffect(AEODCharacterBase* Instigator, AEODCharacterBase* Target);

	virtual void ActivateEffect();

	virtual void DeactivateEffect();

	virtual void UpdateEffect(float DeltaTime);

	// --------------------------------------
	//	Cache
	// --------------------------------------

	/** This effect's instigator */
	UPROPERTY(Transient)
	TWeakObjectPtr<AEODCharacterBase> EffectInstigator;

	/** Skill component of this effect's instigator */
	UPROPERTY(Transient)
	TWeakObjectPtr<UGameplaySkillsComponent> InstigatorSkillComponent;

	/** This effect's target */
	UPROPERTY(Transient)
	TWeakObjectPtr<AEODCharacterBase> EffectTarget;

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	/** The duration for which this effect will last */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Information")
	float GameplayEffectDuration;

	
};
