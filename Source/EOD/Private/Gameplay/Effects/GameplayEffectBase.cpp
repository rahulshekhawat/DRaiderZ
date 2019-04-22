// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "GameplayEffectBase.h"
#include "EODCharacterBase.h"

UGameplayEffectBase::UGameplayEffectBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameplayEffectBase::InitEffect(AEODCharacterBase* Instigator, AEODCharacterBase* Target)
{
	EffectInstigator = Instigator;
	EffectTarget = Target;

	if (Instigator)
	{
		InstigatorSkillComponent = Instigator->GetGameplaySkillsComponent();
	}

	if (Target)
	{
		TargetSkillComponent = Target->GetGameplaySkillsComponent();
	}
}

void UGameplayEffectBase::ActivateEffect()
{
}

void UGameplayEffectBase::DeactivateEffect()
{
}

void UGameplayEffectBase::UpdateEffect(float DeltaTime)
{
}
