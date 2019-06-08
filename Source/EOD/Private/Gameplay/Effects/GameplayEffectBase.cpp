// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "GameplayEffectBase.h"
#include "EODCharacterBase.h"
#include "GameplaySkillsComponent.h"

UGameplayEffectBase::UGameplayEffectBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameplayEffectBase::InitEffect(AEODCharacterBase* Instigator, TArray<AEODCharacterBase*> Targets)
{
	EffectInstigator = Instigator;
	if (Instigator)
	{
		InstigatorSkillComponent = Instigator->GetGameplaySkillsComponent();
	}

	for (AEODCharacterBase* Target : Targets)
	{
		EffectTargets.Add(Target);
		if (Target)
		{
			TargetSkillComponents.Add(Target->GetGameplaySkillsComponent());
		}
	}
}

void UGameplayEffectBase::ActivateEffect(int32 ActivationLevel)
{
}

void UGameplayEffectBase::DeactivateEffect()
{
}

void UGameplayEffectBase::UpdateEffect(float DeltaTime)
{
}
