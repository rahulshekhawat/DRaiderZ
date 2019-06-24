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

	static const FString AnimationSoundXmlFilePostfix;
	static const FString AnimationXmlFilePostfix;

};
