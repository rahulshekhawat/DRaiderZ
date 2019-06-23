// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AssetData.h"
#include "UObject/NoExportTypes.h"
#include "EditorFunctionLibrary.generated.h"

class USkeletalMesh;


/**
 * 
 */
UCLASS()
class EDITORTOOLS_API UEditorFunctionLibrary : public UObject
{
	GENERATED_BODY()
	
public:

	UEditorFunctionLibrary(const FObjectInitializer& ObjectInitializer);

	static TArray<FAssetData> GetAllAnimationsForSkeletalMesh(USkeletalMesh* SkeletalMesh);
	static TArray<FAssetData> GetAllSoundAssets();
	
	static FString GetNestedFileExtension(const FString& FilePath, bool bIncludeDot = true);
	static FString GetBaseFileName(const FString& FilePath);

};
