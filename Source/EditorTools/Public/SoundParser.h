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

	UPROPERTY(Transient)
	FString AnimationName;

	UPROPERTY(Transient)
	FString AnimationFileName;

	// map of frame and sound name
	UPROPERTY(Transient)
	TMap<int32, FString> FrameToSoundNameMap;
	
	// map of frame and sound file name
	UPROPERTY(Transient)
	TMap<int32, FString> FrameToSoundFilenameMap;

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

	//~ .elu.animationsoundevent.xml
	// static TArray<FXmlNode*> GetAnimationNodes(const FString& XmlFilePath);
	//~ elu.animation.xml
	// static TArray<FXmlNode*> GetAddAnimationNodes(const FString& XmlFilePath);

};
