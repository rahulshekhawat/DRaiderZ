// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomEODSettings.generated.h"

/**
 * 
 */
UCLASS(config=EODSettings, meta = (DisplayName = "Custom EOD Settings"))
class EOD_API UCustomEODSettings : public UObject
{
	GENERATED_BODY()

public:

	UCustomEODSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(config, BlueprintReadWrite, Category = "Custom Settings")
	bool bDisplayFPS;

	UPROPERTY(config, BlueprintReadWrite, Category = "Custom Settings")
	float MouseXSensitivity;

	UPROPERTY(config, BlueprintReadWrite, Category = "Custom Settings")
	float MouseYSensitivity;

	UPROPERTY(config, BlueprintReadWrite, Category = "Custom Settings")
	float MasterVolumeLevel;

	UPROPERTY(config, BlueprintReadWrite, Category = "Custom Settings")
	float MusicVolumeLevel;

	UPROPERTY(config, BlueprintReadWrite, Category = "Custom Settings")
	float VoiceVolumeLevel;

	UPROPERTY(config, BlueprintReadWrite, Category = "Custom Settings")
	float EffectsVolumeLevel;

	UFUNCTION(BlueprintCallable, Category ="Custom Settings", meta = (DisplayName = "Save Config"))
	void BP_SaveConfig();



};
