// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "MusicTriggerSphere.h"

AMusicTriggerSphere::AMusicTriggerSphere(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

USoundBase* AMusicTriggerSphere::BP_GetRandomMusic() const
{
	return GetRandomMusic();
}
