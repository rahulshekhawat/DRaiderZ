// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "Gameplay/Abilities/EODGameplayAbility.h"

#include "CharacterAttributeSetBase.h"
#include "Calculations/GMMC_AbilityCooldown.h"
#include "Characters/EODCharacterBase.h"
#include "Gameplay/EODAbilitySystemComponent.h"
#include "EODGameInstance.h"

UEODGameplayAbility::UEODGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UEODGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bCanActivate && CurrentActorInfo)
	{
		FGameplayAbilitySpec* Spec = GetAbilitySystemComponentFromActorInfo()->FindAbilitySpecFromHandle(Handle);
		if (Spec)
		{
			UEODGameplayAbility* AbilityInstance = Cast<UEODGameplayAbility>(Spec->GetPrimaryInstance());
			if (AbilityInstance)
			{
				if (AbilityInstance->K2_CheckAbilityCooldown() == false)
				{
					bCanActivate = false;
				}
				else if (AbilityInstance->K2_CheckAbilityCost() == false)
				{
					bCanActivate = false;
				}
			}
		}
	}
	return bCanActivate;
}

FEODAction UEODGameplayAbility::GetActionForTag(const FGameplayTag& ActionTag)
{
	return FEODAction();
}

void UEODGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);	
}

FGameplayEffectSpecHandle UEODGameplayAbility::GenerateDamageSpec(float Modifier) const
{
	FGameplayEffectSpecHandle DamageSpec = nullptr;

	UEODGameInstance* GameInstance = OwningCharacter ? Cast<UEODGameInstance>(OwningCharacter->GetGameInstance()) : nullptr;
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	if (ensureAlways(GameInstance) && ensureAlways(ASC) && ensureAlways(OwningCharacter))
	{
		TSubclassOf<UGameplayEffect> DamageGE;
		float DamageVal = 0.f;
		if (DamageDataTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("DamageType.Physical"))))
		{
			DamageGE = GameInstance->PhysicalDamageClass;
			DamageVal = OwningCharacter->GetPrimaryAttributeSet()->GetPhysicalAttack();
		}
		else if (DamageDataTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("DamageType.Magic"))))
		{
			DamageGE = GameInstance->MagicDamageClass;
			DamageVal = OwningCharacter->GetPrimaryAttributeSet()->GetMagicAttack();
		}
		// ... else if (holy, elemental, etc.)
		
		DamageSpec = ASC->MakeOutgoingSpec(DamageGE, GetAbilityLevel(), ASC->MakeEffectContext());
		DamageSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), DamageVal * Modifier);
		DamageSpec.Data->DynamicAssetTags.AppendTags(DamageDataTags);
		DamageSpec.Data->CapturedSourceTags.GetSpecTags().AppendTags(DamageDataTags);
	}

	return DamageSpec;
}

void UEODGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	
	if (OwningCharacter == nullptr)
	{
		if (CurrentActorInfo)
		{
			OwningCharacter = Cast<AEODCharacterBase>(GetAvatarActorFromActorInfo());
		}
	}

	bool bActivatedAbility = false;
	if (bActivateAbilityOnGranted && ActorInfo && ActorInfo->AbilitySystemComponent != nullptr)
	{
		bActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}
