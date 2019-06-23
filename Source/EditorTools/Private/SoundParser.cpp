// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SoundParser.h"
#include "EditorFunctionLibrary.h"
#include "EOD.h"

#include "XmlFile.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "AssetRegistryModule.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimSequence.h"
#include "Misc/ScopedSlowTask.h"
#include "HAL/FileManagerGeneric.h"

const FString USoundParser::DataFolderPath(TEXT("F:/Zunk/Zunk_Tests/datadump/Data"));
const FString USoundParser::AnimationSoundXmlFilePostfix(TEXT(".elu.animationsoundevent.xml"));

USoundParser::USoundParser(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USoundParser::ImportSoundForSkeletalMesh(USkeletalMesh* Mesh)
{
	TArray<FAssetData> AnimationAssets = UEditorFunctionLibrary::GetAllAnimationsForSkeletalMesh(Mesh);
	if (AnimationAssets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Import failed because we couldn't find any animations"));
		return;
	}

	FString SoundEventFilePath;
	bool bFound = GetSoundEventFile(Mesh, SoundEventFilePath);
	if (!bFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find sound even file for the given skeletal mesh"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *SoundEventFilePath);
}

bool USoundParser::GetSoundEventFile(USkeletalMesh* Mesh, FString& OutFilePath)
{
	FString FullMeshName = Mesh->GetFName().ToString();
	FString MeshName = FullMeshName.RightChop(3);

	FString MeshAnimationSoundEventXmlFileName = MeshName + AnimationSoundXmlFilePostfix;

	TArray<FString> AllSoundEventFiles;
	IFileManager& FileManager = IFileManager::Get();
	FString SearchString = FString("*") + AnimationSoundXmlFilePostfix;
	FileManager.FindFilesRecursive(AllSoundEventFiles, *DataFolderPath, *SearchString, true, false);

	FScopedSlowTask SlowTask(AllSoundEventFiles.Num(), FText::FromString("Printing Sound Event Files!"));
	SlowTask.MakeDialog();

	for (const FString& FilePath : AllSoundEventFiles)
	{
		const FString CleanFileName = FPaths::GetCleanFilename(FilePath);
		if (CleanFileName == MeshAnimationSoundEventXmlFileName)
		{
			OutFilePath = FilePath;
			return true;
		}
	}
	return false;
}
