// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "RideBase.h"


ARideBase::ARideBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	USpringArmComponent* SpringArm = GetCameraBoomComponent();
	if (SpringArm)
	{
		SpringArm->AddLocalOffset(FVector(0.f, 0.f, 40.f));
	}
}

void ARideBase::BeginPlay()
{
	Super::BeginPlay();
}
