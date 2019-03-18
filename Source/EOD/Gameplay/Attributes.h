// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Attributes.generated.h"


UENUM(BlueprintType)
enum class EAttributeModifierType : uint8
{
	Flat,
	Percent
};

USTRUCT(BlueprintType)
struct EOD_API FAttributeModifier
{
	GENERATED_USTRUCT_BODY()

	/** Value of this attribute */
	UPROPERTY(Transient)
	float BaseValue;



};

USTRUCT(BlueprintType)
struct EOD_API FAttribute
{
	GENERATED_USTRUCT_BODY()

	float GetValue();

	void AddModifier(FAttributeModifier Mod);

	int32 RemoveModifier(FAttributeModifier Mod);

private:

	float CalculateFinalValue();

	/** Value of this attribute */
	UPROPERTY(Transient)
	float BaseValue;

	UPROPERTY(Transient)
	TArray<FAttributeModifier> AttrModifiers;

	bool bIsDirty;

	FAttribute(float Value)
	{
		BaseValue = Value;
		bIsDirty = true;
	}

};

