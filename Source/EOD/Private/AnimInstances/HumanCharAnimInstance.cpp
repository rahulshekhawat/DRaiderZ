// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "HumanCharAnimInstance.h"
#include "HumanCharacter.h"

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
