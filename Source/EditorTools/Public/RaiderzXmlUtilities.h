// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RaiderzXmlUtilities.generated.h"

/**
 * 
 */
UCLASS()
class EDITORTOOLS_API URaiderzXmlUtilities : public UObject
{
	GENERATED_BODY()
	
public:

	URaiderzXmlUtilities(const FObjectInitializer& ObjectInitializer);

	static const FString DataFolderPath;
	static const FString SoundXmlFilePath;
	static const FString NPCXmlFilePath;
	static const FString TalentXmlFilePath;

	static const FString EluAnimationXmlExt;
	static const FString EluAnimationEventXmlExt;
	static const FString EluAnimationInfoXmlExt;
	static const FString EluAnimationSoundEventXmlExt;
	static const FString EluXmlExt;
	static const FString SceneXmlExt;

};
