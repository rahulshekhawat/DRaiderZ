// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EluLibrary.h"
#include "AniLoader.h"
#include "EluMeshNodeLoader.h"
#include "UObject/NoExportTypes.h"
#include "EluImporter.generated.h"

class USkeletalMesh;

struct EDITORTOOLS_API FEluFileData
{
public:
	bool bLoadSuccess = false;
	FEluHeader EluHeader;
	TArray<TSharedPtr<FEluMeshNode>> EluMeshNodes;
};

struct EDITORTOOLS_API FAniFileData
{
public:
	bool bLoadSuccess = false;
	FAniHeader AniHeader;
	TArray<TSharedPtr<FAniNode>> AniNodes;

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

	UFUNCTION(BlueprintCallable, Category = "Elu Importer")
	static void ImportEluAnimation(USkeletalMesh* Mesh);

	static const int TICKSPERFRAME = 160;

private:

	static bool PickEluFile(FString& OutFilePath);
	static FEluFileData LoadEluData(const FString& EluFilePath);
	static FAniFileData LoadAniData(const FString& AniFilePath);
	static bool ImportEluStaticMesh_Internal(const FString& EluFilePath);
	static bool ImportEluSkeletalMesh_Internal(const FString& EluFilePath);

	static bool ImportSkeletalMeshV2_Internal(const FString& EluFilePath);


};
