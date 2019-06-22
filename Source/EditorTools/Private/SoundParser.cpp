// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SoundParser.h"

#include "AssetRegistryModule.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimSequence.h"
#include "Misc/ScopedSlowTask.h"

const FString USoundParser::DataFolderPath(TEXT("F:/Zunk/Zunk_Tests/datadump/Data"));
const FString USoundParser::AnimationSoundXmlFilePostfix(TEXT(".elu.animationsoundevent.xml"));

USoundParser::USoundParser(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USoundParser::ImportSoundForSkeletalMesh(USkeletalMesh* Mesh)
{
	if (Mesh == nullptr || Mesh->Skeleton == nullptr)
	{
		return;
	}

	FString FullMeshName = Mesh->GetFName().ToString();
	FString MeshName = FullMeshName.RightChop(3);
	TArray<FAssetData> Animations = GetAllAnimationsWithString(MeshName);
	
	int32 AnimNum = Animations.Num();
	for (int i = 0; i < AnimNum; i++)
	{
		const FAssetData& AssetData = Animations[i];
		UE_LOG(LogTemp, Warning, TEXT("%s"), *AssetData.GetFullName());
	}

	FString MeshAnimationSoundEventXmlFileName = MeshName + AnimationSoundXmlFilePostfix;



}

TArray<FAssetData> USoundParser::GetAllAnimationsWithString(const FString& String)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AnimationsAssetData;
	AssetRegistryModule.Get().GetAssetsByClass(FName("AnimSequenceBase"), AnimationsAssetData, true);

	int32 AssetNum = AnimationsAssetData.Num();

	FScopedSlowTask FindAnimationTask(AssetNum, FText::FromString("Finding animations"));
	FindAnimationTask.MakeDialog();

	TArray<FAssetData> Animations;
	for (int i = 0; i < AssetNum; i++)
	{
		FAssetData AssetData = AnimationsAssetData[i];
		if (AssetData.GetFullName().Contains(String))
		{
			Animations.Add(AssetData);
		}
		FindAnimationTask.EnterProgressFrame();
	}

	return  Animations;
}
