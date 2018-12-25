// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/AnimInstances/HumanCharAnimInstance.h"
#include "EOD/Characters/HumanCharacter.h"

UHumanCharAnimInstance::UHumanCharAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MasterStateMachine_AnimationsBlendTime = 0.f;
	CurrentWeaponType = EWeaponType::None;
}

void UHumanCharAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	HumanOwner = TryGetPawnOwner() ? Cast<AHumanCharacter>(TryGetPawnOwner()) : nullptr;
}

void UHumanCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
}

void UHumanCharAnimInstance::NativePostEvaluateAnimation()
{
}

void UHumanCharAnimInstance::NativeUninitializeAnimation()
{
}
