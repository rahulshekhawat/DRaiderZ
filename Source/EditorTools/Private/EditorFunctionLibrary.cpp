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

FString UEditorFunctionLibrary::GetRaiderZMeshName(USkeletalMesh* Mesh)
{
	if (Mesh)
	{
		FString FullMeshName = Mesh->GetFName().ToString();
		if (FullMeshName.StartsWith(TEXT("SK_hf")))
		{
			return TEXT("hf");
		}
		else if (FullMeshName.StartsWith(TEXT("SK_hm")))
		{
			return TEXT("hm");
		}
		else
		{
			return FullMeshName.RightChop(3);
		}
	}
	return TEXT("");
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
	TArray<FAssetData> AllAnimations;
	AssetRegistryModule.Get().GetAssetsByClass(FName("AnimSequenceBase"), AllAnimations, true);

	int32 AssetNum = AllAnimations.Num();
	TArray<FAssetData> MeshAnimations;

	FString TaskMessage = TEXT("Finding animations for mesh '") + SkeletalMesh->GetFName().ToString() + TEXT("'");
	FScopedSlowTask FindAnimationTask(AssetNum, FText::FromString(TaskMessage));
	FindAnimationTask.MakeDialog();

	for (int i = 0; i < AssetNum; i++)
	{
		FAssetData AssetData = AllAnimations[i];
		FAssetDataTagMapSharedView::FFindTagResult TagResult = AssetData.TagsAndValues.FindTag(TEXT("Skeleton"));
		const FString& Result = TagResult.GetValue();
		FSoftObjectPath ResultPath(Result);

		if (ResultPath == SkeletonSoftPath)
		{
			MeshAnimations.Add(AssetData);
		}

		FindAnimationTask.EnterProgressFrame();
	}

	return MeshAnimations;
}

TArray<FAssetData> UEditorFunctionLibrary::GetAllSoundAssets()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> SoundAssets;
	AssetRegistryModule.Get().GetAssetsByClass(FName("SoundBase"), SoundAssets, true);
	return SoundAssets;
}
