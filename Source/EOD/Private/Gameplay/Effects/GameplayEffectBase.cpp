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

void UGameplayEffectBase::ActivateEffect_Implementation(int32 ActivationLevel)
{
}

void UGameplayEffectBase::DeactivateEffect_Implementation()
{
}

void UGameplayEffectBase::UpdateEffect_Implementation(float DeltaTime)
{
}

AEODCharacterBase* UGameplayEffectBase::GetEffectInstigator() const
{
	return EffectInstigator.Get();
}

UGameplaySkillsComponent* UGameplayEffectBase::GetInstigatorSkillComponent() const
{
	return InstigatorSkillComponent.Get();
}

TArray<AEODCharacterBase*> UGameplayEffectBase::GetEffectTargets() const
{
	TArray<AEODCharacterBase*> Targets;
	for (TWeakObjectPtr<AEODCharacterBase>Target : EffectTargets)
	{
		if (Target.IsValid())
		{
			Targets.Add(Target.Get());
		}
	}
	return Targets;
}

TArray<UGameplaySkillsComponent*> UGameplayEffectBase::GetTargetSkillComponents() const
{
	TArray<UGameplaySkillsComponent*> SkillComponents;
	for (TWeakObjectPtr<UGameplaySkillsComponent>SkillComp : TargetSkillComponents)
	{
		if (SkillComp.IsValid())
		{
			SkillComponents.Add(SkillComp.Get());
		}
	}
	return SkillComponents;
}
