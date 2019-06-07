// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "CustomEODSettings.h"

UCustomEODSettings::UCustomEODSettings(const FObjectInitializer& ObjectInitializer)
{
	LoadConfig(GetClass());
}

void UCustomEODSettings::BP_SaveConfig()
{
	this->SaveConfig();
}
