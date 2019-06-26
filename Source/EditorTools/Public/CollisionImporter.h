// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "XmlFile.h"
#include "UObject/NoExportTypes.h"
#include "CollisionImporter.generated.h"

class USkeletalMesh;

struct EDITORTOOLS_API FCollisionInfo
{
	FString AnimationName;
	FString AnimationFileName;
	FAssetData AnimationAssetData;

	TMap<float, FString> FrameToCollisionStringMap;

	FCollisionInfo() :
		AnimationName(),
		AnimationFileName(),
		AnimationAssetData(),
		FrameToCollisionStringMap()
	{
	}
};


/**
 * 
 */
UCLASS()
class EDITORTOOLS_API UCollisionImporter : public UObject
{
	GENERATED_BODY()
	
public:

	UCollisionImporter(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = EditorLibrary)
	static void ImportCollisionForSkeletalMesh(USkeletalMesh* Mesh);

	static bool GetNPCID(FXmlNode* NPCRootNode, const FString& MeshName, FString& OutNPCID);
	static TArray<FXmlNode*> GetNPCTalents(FXmlNode* TalentRootNode, const FString& InNPCID);

};
