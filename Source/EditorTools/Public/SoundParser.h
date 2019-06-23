// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "XmlFile.h"
#include "AssetData.h"
#include "UObject/NoExportTypes.h"
#include "SoundParser.generated.h"

class USkeletalMesh;
class UAnimSequenceBase;

USTRUCT(BlueprintType)
struct EDITORTOOLS_API FAnimSoundInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString AnimationName;

	UPROPERTY()
	FString AnimationFileName;

	// map of frame and sound assets
	TMap<int32, FAssetData> FrameToSoundAssetMap;

	FAssetData AnimationAssetData;

};

/**
 * 
 */
UCLASS()
class EDITORTOOLS_API USoundParser : public UObject
{
	GENERATED_BODY()

public:
	
	USoundParser(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = EditorLibrary)
	static void ImportSoundForSkeletalMesh(USkeletalMesh* Mesh);

	static bool GetFilePath(const FString& InFileName, FString& OutFilePath);

	static const FString DataFolderPath;
	static const FString SoundXmlFilePath;

	static const FString AnimationSoundXmlFilePostfix;
	static const FString AnimationXmlFilePostfix;

	static TArray<FXmlNode*> GetNodesWithTag(FXmlNode* BaseNode, const FString& Tag);

	static bool GetAnimationFileName(TArray<FXmlNode*> AddAnimNodes, const FString& AnimationName, FString& OutFileName);

};
