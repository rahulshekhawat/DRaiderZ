// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "XmlFile.h"
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

	UFUNCTION(BlueprintCallable, Category = EditorLibrary)
	static void CreateBlueprint(UClass* ParentClass);
	
	static bool IsHumanPlayerMesh(USkeletalMesh* Mesh);
	static FString GetRaiderZMeshName(USkeletalMesh* Mesh);

	static TArray<FAssetData> GetAllAnimationsForSkeletalMesh(USkeletalMesh* SkeletalMesh);
	static TArray<FAssetData> GetAllSoundAssets();


};
