// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EluLibrary.h"
#include "EluMeshNodeLoader.h"
#include "UObject/NoExportTypes.h"
#include "EluImporter.generated.h"

struct EDITORTOOLS_API FEluFileData
{
public:
	bool bLoadSuccess = false;
	FEluHeader EluHeader;
	TArray<TSharedPtr<FEluMeshNode>> EluMeshNodes;
};

/**
 * 
 */
UCLASS()
class EDITORTOOLS_API UEluImporter : public UObject
{
	GENERATED_BODY()
	
public:

	UEluImporter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Elu Importer")
	static void ImportEluStaticMesh();
	
	UFUNCTION(BlueprintCallable, Category = "Elu Importer")
	static void ImportEluSkeletalMesh();

private:

	static bool PickEluFile(FString& OutFilePath);
	static FEluFileData LoadEluData(const FString& EluFilePath);
	static bool ImportEluStaticMesh_Internal(const FString& EluFilePath);
	static bool ImportEluSkeletalMesh_Internal(const FString& EluFilePath);

};
