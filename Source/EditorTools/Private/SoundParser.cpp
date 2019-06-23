// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SoundParser.h"
#include "EditorFunctionLibrary.h"
#include "EOD.h"

#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "AssetRegistryModule.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimSequence.h"
#include "Misc/ScopedSlowTask.h"
#include "HAL/FileManagerGeneric.h"

const FString USoundParser::DataFolderPath(TEXT("F:/Zunk/Zunk_Tests/datadump/Data"));
const FString USoundParser::SoundXmlFilePath(TEXT("F:/Zunk/Zunk_Tests/datadump/Data/Sound/sound.xml"));

const FString USoundParser::AnimationSoundXmlFilePostfix(TEXT(".elu.animationsoundevent.xml"));
const FString USoundParser::AnimationXmlFilePostfix(TEXT(".elu.animation.xml"));

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

	FString FullMeshName = Mesh->GetFName().ToString();
	FString MeshName = FullMeshName.RightChop(3);

	FString SoundEventFilePath;
	FString AnimXmlFilePath;
	bool bFoundSoundXml = GetFilePath(MeshName + AnimationSoundXmlFilePostfix, SoundEventFilePath);
	bool bFoundAnimXml = GetFilePath(MeshName + AnimationXmlFilePostfix, AnimXmlFilePath);
	if (!(bFoundSoundXml && bFoundAnimXml))
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find sound or animation xml file for the given skeletal mesh"));
		return;
	}

	FXmlFile SoundEventFileObj(SoundEventFilePath);
	FXmlNode* RootSoundEventNode = SoundEventFileObj.GetRootNode();
	TArray<FXmlNode*> AnimationNodes = GetNodesWithTag(RootSoundEventNode, TEXT("Animation"));

	FXmlFile AnimFileObj(AnimXmlFilePath);
	FXmlNode* RootAnimNode = AnimFileObj.GetRootNode();
	TArray<FXmlNode*> AddAnimationNodes = GetNodesWithTag(RootAnimNode, TEXT("AddAnimation"));

	FXmlFile SoundFileObj(SoundXmlFilePath);
	FXmlNode* RootSoundNode = SoundFileObj.GetRootNode();
	TArray<FXmlNode*> AddAnimationNodes = GetNodesWithTag(RootSoundNode, TEXT("SOUND"));



	for (FXmlNode* AnimationNode : AnimationNodes)
	{
		if (AnimationNode)
		{
			FString AnimName = AnimationNode->GetAttribute(TEXT("name"));
			UE_LOG(LogTemp, Warning, TEXT("%s"), *AnimName);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *SoundEventFilePath);
}

bool USoundParser::GetFilePath(const FString& InFileName, FString& OutFilePath)
{
	FString FileExtension = UEditorFunctionLibrary::GetNestedFileExtension(InFileName);

	if (FileExtension != TEXT(""))
	{
		TArray<FString> AllFiles;
		IFileManager& FileManager = IFileManager::Get();

		FString SearchString = FString("*") + FileExtension;
		FileManager.FindFilesRecursive(AllFiles, *DataFolderPath, *SearchString, true, false);

		for (const FString& FilePath : AllFiles)
		{
			const FString CleanFileName = FPaths::GetCleanFilename(FilePath);
			if (CleanFileName == InFileName)
			{
				OutFilePath = FilePath;
				return true;
			}
		}
	}

	return false;
}

TArray<FXmlNode*> USoundParser::GetNodesWithTag(FXmlNode* BaseNode, const FString& Tag)
{
	if (!BaseNode)
	{
		return TArray<FXmlNode*>();
	}

	TArray<FXmlNode*> ResultNodes;
	if (BaseNode->GetTag() == Tag)
	{
		ResultNodes.Add(BaseNode);
	}

	TArray<FXmlNode*> ChildrenNodes = BaseNode->GetChildrenNodes();
	for (FXmlNode* ChildNode : ChildrenNodes)
	{
		TArray<FXmlNode*> ResultingChildNodes = GetNodesWithTag(ChildNode, Tag);
		ResultNodes.Append(ResultingChildNodes);
	}
	return ResultNodes;
}
