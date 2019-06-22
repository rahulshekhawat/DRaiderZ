// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AssetData.h"
#include "UObject/NoExportTypes.h"
#include "SoundParser.generated.h"

class USkeletalMesh;
class UAnimSequenceBase;

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

	static TArray<FAssetData> GetAllAnimationsWithString(const FString& String);

	static const FString DataFolderPath;
	static const FString AnimationSoundXmlFilePostfix;
	

};
