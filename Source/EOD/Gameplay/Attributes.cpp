// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "Attributes.h"

float FAttribute::GetValue()
{
	if (bIsDirty)
	{

	}
	return 0.0f;
}

void FAttribute::AddModifier(FAttributeModifier Mod)
{
	bIsDirty = true;
	AttrModifiers.Add(Mod);
}

int32 FAttribute::RemoveModifier(FAttributeModifier Mod)
{
	bIsDirty = true;
	AttrModifiers.Remove(Mod);
}

//~ @todo
float FAttribute::CalculateFinalValue()
{
	float FinalValue = BaseValue;
	float SumPercentAdd = 0; // This will hold the sum of our "PercentAdd" modifiers
	for (int i = AttrModifiers.Num() - 1; i >= 0; i++)
	{
		FAttributeModifier Mod = AttrModifiers[i];
		
	}

	return 0.0f;
}
