// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/ElementalBase.h"
#include "IceElemental.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS()
class EOD_API UIceElemental : public UElementalBase
{
	GENERATED_BODY()
	
public:

	UIceElemental(const FObjectInitializer& ObjectInitializer);

	virtual TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo>* GetCharacterToStatusInfoMap() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = IceElemental)
	float SlowDownModifier;

	/** Called to process the ticking of this status effect. Must be overridden in inherited classes */
	// UFUNCTION()
	virtual void OnStatusEffectTick(FBaseCharacter_WeakObjPtrWrapper& WrappedRecipientCharacter) override;

private:

	static TMap<TWeakObjectPtr<AEODCharacterBase>, FStatusInfo> CharactersToStatusInfoMap;

	static TMap<TWeakObjectPtr<AEODCharacterBase>, float> CharacterToPhysicalDamageReductionMap;

	static TMap<TWeakObjectPtr<AEODCharacterBase>, float> CharacterToMagickalDamageReductionMap;

	float CalculatePhysicalDamageReduction(float IceDamage, float IceDefense);

	float CalculateMagickalDamageReduction(float IceDamage, float IceDefense);


};
