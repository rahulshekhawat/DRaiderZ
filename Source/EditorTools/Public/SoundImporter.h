// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "XmlFile.h"
#include "AssetData.h"
#include "UObject/NoExportTypes.h"
#include "SoundImporter.generated.h"

class USoundBase;
class USkeletalMesh;
class USoundAttenuation;
class UAnimSequenceBase;

struct EDITORTOOLS_API FAnimSoundInfo
{
	FString AnimationName;
	FString AnimationFileName;
	FAssetData AnimationAssetData;

	// map of frame and sound assets
	TMap<float, FAssetData> FrameToSoundAssetMap;

	FAnimSoundInfo() :
		AnimationName(),
		AnimationFileName(),
		AnimationAssetData(),
		FrameToSoundAssetMap()
	{
	}
};

/**
 * 
 */
UCLASS()
class EDITORTOOLS_API USoundImporter : public UObject
{
	GENERATED_BODY()
	
public:

	USoundImporter(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = EditorLibrary)
	static void ImportSoundForSkeletalMesh(USkeletalMesh* Mesh, USoundAttenuation* AttenuationToApply = nullptr);

private:

	static bool GetAnimationFileName(const TArray<FXmlNode*>& AddAnimNodes, FXmlNode* AnimNode, FString& OutFileName);

	static TArray<FAnimSoundInfo> GenerateAnimSoundInfoArray(
		const TArray<FXmlNode*>& AnimationNodes,
		const TArray<FXmlNode*>& AddAnimationNodes,
		const TArray<FXmlNode*>& SoundNodes,
		const TArray<FAssetData>& MeshAnimAssets,
		const TArray<FAssetData>& AllSoundAssets);

	static FAnimSoundInfo GetAnimSoundInfo(
		FXmlNode* AnimNode,
		const TArray<FXmlNode*>& SoundNodes,
		const FString& AnimationFileName,
		const TArray<FAssetData>& MeshAnimAssets,
		const TArray<FAssetData>& AllSoundAssets);

	static FString GetEditorSoundName(FXmlNode* EventNode, const TArray<FXmlNode*>& SoundNodes);
	static FAssetData GetSoundAsset(FXmlNode* EventNode, const TArray<FXmlNode*>& SoundNodes, const TArray<FAssetData>& AllSoundAssets);
	static TMap<float, FAssetData> GetFrameToSoundAssetMap(FXmlNode* AnimNode, const TArray<FXmlNode*>& SoundNodes, const TArray<FAssetData>& AllSoundAssets);
	static void CreateAndApplySoundNotifies(const TArray<FAnimSoundInfo>& AnimSoundInfoArray, USoundAttenuation* AttenuationToApply);
	static void AddSoundNotifiesToAnimation(UAnimSequenceBase* Animation, const TMap<float, FAssetData>& FrameToSoundAssetMap, USoundAttenuation* AttenuationToApply);
	static bool HasSoundNotify(UAnimSequenceBase* Animation, float NotifyTime, USoundBase* NotifySound);

	/** Name of the mesh that is currently being processed */
	static FString CurrentMeshName;

};
