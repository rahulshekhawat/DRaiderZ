// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "XmlFile.h"
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

	//---
	static bool GetRaiderzFilePath(const FString& InFileName, FString& OutFilePath);
	static FString GetRaiderzFileExtension(const FString& FilePath, bool bIncludeDot = true);
	static FString GetRaiderzBaseFileName(const FString& FilePath);

	//--- 
	static TArray<FXmlNode*> GetNodesWithTag(FXmlNode* BaseNode, const FString& Tag);

	//---
	static bool ReadStringFromBinaryData(FString& StringBuffer, const TArray<uint8>& BinaryData, UINT& Offset);
	static bool WriteBinaryDataToBuffer(void* Buffer, const UINT NumOfBytesToRead, const TArray<uint8>& BinaryData, UINT& Offset);


	static const FString DarkRaiderzAssetPath;
	static const FString DataFolderPath;
	static const FString SoundXmlFilePath;
	static const FString NPCXmlFilePath;
	static const FString TalentXmlFilePath;
	static const FString TalentHitInfoXmlFilePath;

	static const FString EluAnimationXmlExt;
	static const FString EluAnimationEventXmlExt;
	static const FString EluAnimationInfoXmlExt;
	static const FString EluAnimationSoundEventXmlExt;
	static const FString EluXmlExt;
	static const FString SceneXmlExt;

};
