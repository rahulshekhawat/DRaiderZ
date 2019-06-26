// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "EditorFunctionLibrary.h"

#include "Misc/Paths.h"
#include "AssetRegistryModule.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimSequence.h"
#include "Misc/ScopedSlowTask.h"

UEditorFunctionLibrary::UEditorFunctionLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UEditorFunctionLibrary::IsHumanPlayerMesh(USkeletalMesh* Mesh)
{
	if (Mesh == nullptr)
	{
		return false;
	}

	FString FullMeshName = Mesh->GetFName().ToString();
	if (FullMeshName.StartsWith(TEXT("SK_hf")) || FullMeshName.StartsWith(TEXT("SK_hm")))
	{
		return true;
	}

	return false;
}

TArray<FAssetData> UEditorFunctionLibrary::GetAllAnimationsForSkeletalMesh(USkeletalMesh* SkeletalMesh)
{
	if (SkeletalMesh == nullptr)
	{
		return TArray<FAssetData>();
	}

	FSoftObjectPath SkeletonSoftPath(SkeletalMesh->Skeleton);
	if (!SkeletonSoftPath.IsValid())
	{
		return TArray<FAssetData>();
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AnimationsAssetData;
	AssetRegistryModule.Get().GetAssetsByClass(FName("AnimSequenceBase"), AnimationsAssetData, true);

	int32 AssetNum = AnimationsAssetData.Num();
	TArray<FAssetData> Animations;

	FScopedSlowTask FindAnimationTask(AssetNum, FText::FromString("Finding animations!"));
	FindAnimationTask.MakeDialog();

	for (int i = 0; i < AssetNum; i++)
	{
		FAssetData AssetData = AnimationsAssetData[i];
		FAssetDataTagMapSharedView::FFindTagResult TagResult = AssetData.TagsAndValues.FindTag(TEXT("Skeleton"));
		const FString& Result = TagResult.GetValue();
		FSoftObjectPath ResultPath(Result);

		if (ResultPath == SkeletonSoftPath)
		{
			Animations.Add(AssetData);
		}

		FindAnimationTask.EnterProgressFrame();
	}

	return  Animations;
}

TArray<FAssetData> UEditorFunctionLibrary::GetAllSoundAssets()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> SoundAssets;
	AssetRegistryModule.Get().GetAssetsByClass(FName("SoundBase"), SoundAssets, true);
	return SoundAssets;
}
