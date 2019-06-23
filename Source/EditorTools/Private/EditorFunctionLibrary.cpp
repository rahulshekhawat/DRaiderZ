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

FString UEditorFunctionLibrary::GetNestedFileExtension(const FString& FilePath, bool bIncludeDot)
{
	const FString Filename = FPaths::GetCleanFilename(FilePath);
	int32 DotPos = Filename.Find(TEXT("."), ESearchCase::CaseSensitive, ESearchDir::FromStart);
	if (DotPos != INDEX_NONE)
	{
		return Filename.Mid(DotPos + (bIncludeDot ? 0 : 1));
	}
	return TEXT("");
}

FString UEditorFunctionLibrary::GetBaseFileName(const FString& FilePath)
{
	const FString Filename = FPaths::GetCleanFilename(FilePath);
	int32 ExtPos = Filename.Find(TEXT("."), ESearchCase::CaseSensitive, ESearchDir::FromStart);
	if (ExtPos != INDEX_NONE)
	{
		return Filename.Left(ExtPos);
	}
	return TEXT("");
}
