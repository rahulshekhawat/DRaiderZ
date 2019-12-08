// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODLibrary.h"
#include "InventoryInterface.h"
#include "Engine/DataAsset.h"
#include "HealthPotionBase.generated.h"

UENUM(BlueprintType)
enum class EPotionEffectMethod : uint8
{
	/**
	 * The potion will heal the character the moment it is consumed before the potion drinking animation plays.
	 * Interrupting the potion drinking animation will NOT cancel the healing effect.
	 */
	InstantPreAnimation,

	/**
	 * The potion will heal the character the moment potion drinking animation finishes
	 * Interrupting the potion drinking animation will cancel the healing effect.
	 */
	InstantPostAnimation,

	/**
	 * The potion will heal the character at desired time (i.e., after the 'HealDelay' amount of time after consuming potion)
	 * Interrupting the potion drinking animation will cancel the healing effect.
	 */
	Desired,

	/**
	 * The potion will heal the character over a desired amount of time while the potion drinking animation plays.
	 * Interrupting the potion drinking animation will cancel the healing effect.
	 */
	ContinuousWithAnimation,

	/**
	 * The potion will heal the character over a desired amount of time but the potion drinking animation will only play once.
	 * The character will continue to receive heals even after the potion drinking animation finishes.
	 * Interrupting the potion drinking animation will NOT cancel the healing effect
	 */
	ContinuousWithoutAnimation
};

/**
 * 
 */
UCLASS()
class EOD_API UHealthPotionBase : public UPrimaryDataAsset, public IInventoryInterface
{
	GENERATED_BODY()

public:

	UHealthPotionBase(const FObjectInitializer& ObjectInitializer);

	virtual void OnClick_Implementation(AEODCharacterBase* ClickingChar) const override;

	virtual EInventoryItemType GetInventoryItemType() const override;

	virtual FInGameInformation GetInGameInformation() const override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Potion")
	FInGameInformation PotionInformation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Potion")
	EPotionEffectMethod PotionEffectMethod;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Potion")
	float HealthRegenValue;

	/**
	 * The initial delay before the heal is applied to the character
	 * We will make sure that the potion drinking animation is playing on the character before heal effect is applied.
	 * This won't be used for EPotionEffectMethod::InstantPreAnimation and EPotionEffectMethod::InstantPostAnimation
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Potion")
	float HealDelay;

	/** Potion type can either be consumable or reuseable */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	EInventoryItemType PotionType;

	FTimerHandle HealTimerHandle;

};
