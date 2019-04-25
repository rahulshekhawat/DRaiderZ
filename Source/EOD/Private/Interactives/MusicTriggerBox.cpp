// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "MusicTriggerBox.h"

AMusicTriggerBox::AMusicTriggerBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

USoundBase* AMusicTriggerBox::BP_GetRandomMusic() const
{
	return GetRandomMusic();
}
