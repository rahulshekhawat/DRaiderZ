// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "XmlFile.h"
#include "AssetData.h"
#include "UObject/NoExportTypes.h"
#include "SoundImporter.generated.h"

class USkeletalMesh;
class USoundAttenuation;
class UAnimSequenceBase;

struct EDITORTOOLS_API FAnimSoundInfo
{
	FString AnimationName;
	FString AnimationFileName;
	FAssetData AnimationAssetData;

	// map of frame and sound assets
	TMap<float, FAssetData> FrameToSoundAssetMap;

	FAnimSoundInfo() :
		AnimationName(),
		AnimationFileName(),
		AnimationAssetData(),
		FrameToSoundAssetMap()
	{
	}
};

/**
 * 
 */
UCLASS()
class EDITORTOOLS_API USoundImporter : public UObject
{
	GENERATED_BODY()
	
public:

	USoundImporter(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = EditorLibrary)
	static void ImportSoundForSkeletalMesh(USkeletalMesh* Mesh, USoundAttenuation* SoundAttentionToApply = nullptr);

	static bool GetFilePath(const FString& InFileName, FString& OutFilePath);
	static bool GetAnimationFileName(TArray<FXmlNode*> AddAnimNodes, const FString& AnimationName, FString& OutFileName);

	static const FString DataFolderPath;
	static const FString SoundXmlFilePath;

	static const FString AnimationSoundXmlFilePostfix;
	static const FString AnimationXmlFilePostfix;

};
