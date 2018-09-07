// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/ElementalBase.h"
#include "HolyElemental.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UHolyElemental : public UElementalBase
{
	GENERATED_BODY()

public:

	UHolyElemental(const FObjectInitializer& ObjectInitializer);

	/** Called to deinitialize this status effect on a character */
	virtual void Deinitialize() override;

	virtual TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>* GetCharacterToStatusInfoMap() override;

protected:

	/** Called to process the ticking of this status effect. Must be overridden in inherited classes */
	// UFUNCTION()
	virtual void OnStatusEffectTick(FBaseCharacter_WeakObjPtrWrapper& WrappedRecipientCharacter) override;

	/** Called to deactivate this status effect on a recipient character */
	virtual void DeactivateStatusEffect(TWeakObjectPtr<AEODCharacterBase>& RecipientCharacter) override;

	UPROPERTY(EditDefaultsOnly, Category = HolyElemental)
	float MaxDefenseReduction;

private:

	static TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> CharactersToStatusInfoMap;

	static TMap<TWeakObjectPtr<AEODCharacterBase>, float> CharacterToPhysicalDefenseReductionMap;

	static TMap<TWeakObjectPtr<AEODCharacterBase>, float> CharacterToMagickDefenseReductionMap;

	float CalculatePhysicalDefenseReduction(float HolyDamage, float HolyDefense);

	float CalculateMagickDefenseReduction(float HolyDamage, float HolyDefense);
	
};
