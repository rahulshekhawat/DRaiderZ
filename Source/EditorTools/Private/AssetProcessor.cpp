// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "AssetProcessor.h"
#include "EOD.h"
#include "RaiderzXmlUtilities.h"
#include "EditorFunctionLibrary.h"

#include "Sound/SoundBase.h"
#include "Sound/SoundWave.h"
#include "FastXml.h"
#include "RawMesh.h"
#include "PackageTools.h"
#include "MeshUtilities.h"
#include "Animation/AnimSequence.h"
#include "AssetRegistryModule.h"
#include "Engine/StaticMesh.h"
#include "Misc/ScopedSlowTask.h"

UAssetProcessor::UAssetProcessor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UAssetProcessor::GenerateUniqueUVForStaticMesh(UStaticMesh* StaticMesh)
{
	if (!StaticMesh)
	{
		return false;
	}

	StaticMesh->Modify();
	IMeshUtilities& MeshUtilities = FModuleManager::Get().LoadModuleChecked<IMeshUtilities>("MeshUtilities");
	TArray<FVector2D> OutUniqueUVs;

	FRawMesh RawMesh;
	StaticMesh->GetSourceModels()[0].LoadRawMesh(RawMesh);
	bool bResult = MeshUtilities.GenerateUniqueUVsForStaticMesh(RawMesh, 2048, OutUniqueUVs);
	if (bResult)
	{
		FString Message = TEXT("Unique UV generation succeeded for ") + StaticMesh->GetName();
		PrintLog(Message);
	}
	else
	{
		FString Message = TEXT("Unique UV generation failed for ") + StaticMesh->GetName();
		PrintLog(Message);
	}

	// RawMesh.WedgeTexCoords[0] = OutUniqueUVs;
	RawMesh.WedgeTexCoords[1] = OutUniqueUVs;		// lightmap uvs

	StaticMesh->GetSourceModels()[0].SaveRawMesh(RawMesh);

	TArray<FText> ErrorText;
	StaticMesh->Build(false, &ErrorText);

	StaticMesh->MarkPackageDirty();
	return bResult;
}

bool UAssetProcessor::NormalizeBoneScale(UAnimSequence* AnimSeq)
{
	if (!AnimSeq)
	{
		return false;
	}

	AnimSeq->Modify();

	TArray<FName> TrackNames = AnimSeq->GetAnimationTrackNames();
	TArray<FRawAnimSequenceTrack> RawAnimationData = AnimSeq->GetRawAnimationData();
	for (const FName& TrackName : TrackNames)
	{
	}

	AnimSeq->MarkPackageDirty();
	return true;
}

void UAssetProcessor::ImportRaiderzSoundSettings(USoundAttenuation* Attenuation)
{
	FXmlFile SoundFileObj(URaiderzXmlUtilities::SoundXmlFilePath);
	FXmlNode* RootSoundNode = SoundFileObj.GetRootNode();
	TArray<FXmlNode*> SoundNodes = URaiderzXmlUtilities::GetNodesWithTag(RootSoundNode, TEXT("SOUND"));


	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> SCAssets;
	AssetRegistryModule.Get().GetAssetsByClass(FName("SoundClass"), SCAssets, true);

	USoundClass* MasterSC = nullptr;
	USoundClass* MusicSC = nullptr;
	USoundClass* EffectsSC = nullptr;
	USoundClass* UISC = nullptr;
	USoundClass* VoiceSC = nullptr;

	for (const FAssetData& SCAsset : SCAssets)
	{
		if (SCAsset.AssetName.ToString() == TEXT("SC_Master"))
		{
			MasterSC = Cast<USoundClass>(SCAsset.GetAsset());
		}
		else if (SCAsset.AssetName.ToString() == TEXT("SC_Music"))
		{
			MusicSC = Cast<USoundClass>(SCAsset.GetAsset());
		}
		else if (SCAsset.AssetName.ToString() == TEXT("SC_SoundEffects"))
		{
			EffectsSC = Cast<USoundClass>(SCAsset.GetAsset());
		}
		else if (SCAsset.AssetName.ToString() == TEXT("SC_UI"))
		{
			UISC = Cast<USoundClass>(SCAsset.GetAsset());
		}
		else if (SCAsset.AssetName.ToString() == TEXT("SC_Voice"))
		{
			VoiceSC = Cast<USoundClass>(SCAsset.GetAsset());
		}
	}

	check(MasterSC);
	check(MusicSC);
	check(EffectsSC);
	check(UISC);
	check(VoiceSC);

	FScopedSlowTask SlowTask(SoundNodes.Num(), FText::FromString("Importing Raiderz Sound Settings!"));
	SlowTask.MakeDialog();

	TArray<FAssetData> AllSoundAssets = UEditorFunctionLibrary::GetAllSoundAssets();

	for (FXmlNode* Node : SoundNodes)
	{
		SlowTask.EnterProgressFrame();

		check(Node);

		const FString& SoundFilePath = Node->GetAttribute(TEXT("filename"));
		const FString& SoundFileName = FPaths::GetCleanFilename(SoundFilePath);
		const FString& ProbableEditorSoundName = URaiderzXmlUtilities::GetRaiderzBaseFileName(SoundFileName);
		const FString& EditorSoundName = PackageTools::SanitizePackageName(ProbableEditorSoundName);

		USoundWave* SoundFile = nullptr;
		for (const FAssetData& SoundAsset : AllSoundAssets)
		{
			if (SoundAsset.AssetName.ToString() == EditorSoundName)
			{
				USoundWave* Sound = Cast<USoundWave>(SoundAsset.GetAsset());
				if (Sound)
				{
					SoundFile = Sound;
				}
			}
		}

		if (SoundFile == nullptr)
		{
			continue;
		}

		const FString& LoopString = Node->GetAttribute(TEXT("loop"));
		if (LoopString == TEXT("true") && SoundFile->bLooping != true)
		{
			SoundFile->Modify();
			SoundFile->bLooping = true;
			SoundFile->MarkPackageDirty();
		}

		/*
		const FString& TypeString = Node->GetAttribute(TEXT("type"));
		const FString& PriorityString = Node->GetAttribute(TEXT("priority"));
		const FString& MaxdistString = Node->GetAttribute(TEXT("maxdist"));
		const FString& VolumeString = Node->GetAttribute(TEXT("volume"));

		if (TypeString == TEXT("bgm") && SoundFile->GetSoundClass() != MusicSC)
		{
			//~ Set SoundClassObject to MusicSC;
			// SoundFile->AssetImportData
		}
		*/
	}
}
