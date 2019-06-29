// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FbxImportUtilities.generated.h"

/**
 * 
 */
UCLASS()
class EDITORTOOLS_API UFbxImportUtilities : public UObject
{
	GENERATED_BODY()
	
public:

	UFbxImportUtilities(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "RaiderZ Library", meta = (DisplayName = "Import RaiderZ Fbx File"))
	static void ImportRaiderzFbxFile();

	UFUNCTION(BlueprintCallable, Category = "RaiderZ Library", meta = (DisplayName = "Import RaiderZ Fbx Files From Folder"))
	static void ImportRaiderzFbxFilesFromFolder();

private:




};
