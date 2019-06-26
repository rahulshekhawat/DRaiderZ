// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "XmlFile.h"
#include "AssetData.h"
#include "UObject/NoExportTypes.h"
#include "CollisionImporter.generated.h"

class USkeletalMesh;

struct EDITORTOOLS_API FCollisionInfo
{
	FString TalentID;
	FString AnimationName;
	FString AnimationFileName;
	FAssetData AnimationAssetData;

	TMap<FString, TArray<FString>> FrameToCollisionStringMap;

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

private:

	static TArray<FCollisionInfo> GenerateCollisionInfoArray(
		FXmlNode* NPCNode,
		const TArray<FXmlNode*>& TalentNodes,
		const TArray<FXmlNode*>& AddAnimNodes,
		const TArray<FXmlNode*>& TalentHitNodes,
		const TArray<FAssetData>& MeshAnimAssets);

	static FCollisionInfo GetCollisionInfo(
		FXmlNode* NPCNode,
		FXmlNode* TalentNode,
		const TArray<FXmlNode*>& AddAnimNodes,
		const TArray<FXmlNode*>& TalentHitNodes,
		const FString& AnimationFileName,
		const TArray<FAssetData>& MeshAnimAssets);

	static TMap<FString, TArray<FString>> GetFrameToCollisionStringMap(const TArray<FXmlNode*>& TalentHitNodes, FXmlNode* TalentNode, FXmlNode* NPCNode);
	static FXmlNode* GetTalentHitNodeWithID(const TArray<FXmlNode*>& TalentHitNodes, const FString& TalentID);
	static bool GetAnimationFileName(const TArray<FXmlNode*>& AddAnimNodes, FXmlNode* TalentNode, FString& OutFileName);

	static FXmlNode* GetNPCNode(FXmlNode* NPCRootNode, const FString& MeshName);
	static TArray<FXmlNode*> GetNPCTalents(FXmlNode* TalentRootNode, const FString& InNPCID);

	/** Name of the mesh that is currently being processed */
	static FString CurrentMeshName;



};
