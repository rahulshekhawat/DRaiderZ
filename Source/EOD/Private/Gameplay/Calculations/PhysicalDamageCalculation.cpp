// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "Gameplay/Calculations/PhysicalDamageCalculation.h"

#include "CharacterAttributeSetBase.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct FPhysicalDamageStatics
{
	// DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalAttack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalCritRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalCritBonus);
	
	// Meta attribute that we're passing into the ExecCalc via SetByCaller on the GESpec so we don't capture it.
	// We still need to declare and define it so that we can output to it.
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	FPhysicalDamageStatics()
	{
		// Snapshot happens at time of GESpec creation

		// Capture the Target's PhysicalResistance. Don't snapshot (the false).
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, PhysicalResistance, Target, false);
		
		// Capture the Source's PhysicalCritRate. Don't snapshot (the false).
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, PhysicalCritRate, Source, false);
		
		// Capture the Source's PhysicalCritBonus. Don't snapshot (the false).
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, PhysicalCritBonus, Source, false);
		
		// The Target's received Damage. This is the value of health that will be subtracted on the Target. We're not capturing this
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, Damage, Target, false);
	}
};

static const FPhysicalDamageStatics& PhysicalDamageStatics()
{
	static FPhysicalDamageStatics DStatics;
	return DStatics;
}

UPhysicalDamageCalculation::UPhysicalDamageCalculation()
{
	RelevantAttributesToCapture.Add(PhysicalDamageStatics().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(PhysicalDamageStatics().PhysicalCritRateDef);
	RelevantAttributesToCapture.Add(PhysicalDamageStatics().PhysicalCritBonusDef);

	// We don't include Damage here because we're not capturing it. It is generated inside the ExecCalc.
}

void UPhysicalDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// SetByCaller Damage
	const float Damage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);
	
	float PhysicalResistance = 0.f;
	FMath::Max<float>(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(PhysicalDamageStatics().PhysicalResistanceDef, EvaluationParameters, PhysicalResistance), 0.f);

	float PhysicalCritRate = 0.f;
	FMath::Max<float>(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(PhysicalDamageStatics().PhysicalCritRateDef, EvaluationParameters, PhysicalCritRate), 0.f);

	float UnmitigatedDamage = 0.f;
	const bool bIsCritical = FMath::RandRange(0.f, 100.f) < PhysicalCritRate;
	if (bIsCritical)
	{
		float PhysicalCritBonus = 0.f;
		FMath::Max<float>(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(PhysicalDamageStatics().PhysicalCritBonusDef, EvaluationParameters, PhysicalCritBonus), 0.f);
		UnmitigatedDamage = Damage * 2.f /* CritMultiplier */ + PhysicalCritBonus; 
	}
	else
	{
		UnmitigatedDamage = Damage;
	}

	const float BufferValue = 1000.f;
	float MitigatedDamage = UnmitigatedDamage * (BufferValue / (BufferValue + PhysicalResistance)); 

	if (MitigatedDamage)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(PhysicalDamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}
}
