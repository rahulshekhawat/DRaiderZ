// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SoundImporter.h"
#include "EOD.h"
#include "EditorFunctionLibrary.h"
#include "RaiderzXmlUtilities.h"

#include "PackageTools.h"
#include "Sound/SoundBase.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "AssetRegistryModule.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimSequence.h"
#include "Misc/ScopedSlowTask.h"
#include "HAL/FileManagerGeneric.h"
#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"

FString USoundImporter::CurrentMeshName(TEXT(""));

USoundImporter::USoundImporter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USoundImporter::ImportSoundForSkeletalMesh(USkeletalMesh* Mesh, USoundAttenuation* AttenuationToApply)
{
	TArray<FAssetData> MeshAnimAssets = UEditorFunctionLibrary::GetAllAnimationsForSkeletalMesh(Mesh);
	if (MeshAnimAssets.Num() == 0)
	{
		PrintError(TEXT("Import failed because we couldn't find any animations"));
		return;
	}

	CurrentMeshName = UEditorFunctionLibrary::GetRaiderZMeshName(Mesh);
	check(CurrentMeshName != TEXT(""));

	FScopedSlowTask SlowTask(3, FText::FromString("Finding and parsing XML files!"));
	SlowTask.MakeDialog();

	FString SoundEventFilePath;
	bool bFoundSoundXml = URaiderzXmlUtilities::GetRaiderzFilePath(CurrentMeshName + URaiderzXmlUtilities::EluAnimationSoundEventXmlExt, SoundEventFilePath);
	if (!bFoundSoundXml)
	{
		PrintError(TEXT("Couldn't find animationsoundevent.xml file for the given skeletal mesh"));
		return;
	}
	FXmlFile SoundEventFileObj(SoundEventFilePath);
	FXmlNode* RootSoundEventNode = SoundEventFileObj.GetRootNode();
	TArray<FXmlNode*> AnimationNodes = URaiderzXmlUtilities::GetNodesWithTag(RootSoundEventNode, TEXT("Animation"));
	SlowTask.EnterProgressFrame();

	FString AnimXmlFilePath;
	bool bFoundAnimXml = URaiderzXmlUtilities::GetRaiderzFilePath(CurrentMeshName + URaiderzXmlUtilities::EluAnimationXmlExt, AnimXmlFilePath);
	if (!bFoundAnimXml)
	{
		PrintError(TEXT("Couldn't find .elu.animation.xml file for the given skeletal mesh"));
		return;
	}
	FXmlFile AnimFileObj(AnimXmlFilePath);
	FXmlNode* RootAnimNode = AnimFileObj.GetRootNode();
	TArray<FXmlNode*> AddAnimationNodes = URaiderzXmlUtilities::GetNodesWithTag(RootAnimNode, TEXT("AddAnimation"));
	SlowTask.EnterProgressFrame();

	FXmlFile SoundFileObj(URaiderzXmlUtilities::SoundXmlFilePath);
	FXmlNode* RootSoundNode = SoundFileObj.GetRootNode();
	TArray<FXmlNode*> SoundNodes = URaiderzXmlUtilities::GetNodesWithTag(RootSoundNode, TEXT("SOUND"));
	SlowTask.EnterProgressFrame();

	TArray<FAssetData> AllSoundAssets = UEditorFunctionLibrary::GetAllSoundAssets();
	TArray<FAnimSoundInfo> AnimSoundInfoArray = GenerateAnimSoundInfoArray(AnimationNodes, AddAnimationNodes, SoundNodes, MeshAnimAssets, AllSoundAssets);

	FilterAnimSoundInfoArray(AnimSoundInfoArray);
	CreateAndApplySoundNotifies(AnimSoundInfoArray, AttenuationToApply);

	CurrentMeshName = TEXT("");
	PrintLog(TEXT("Finished imported sound notifies!"));
}

bool USoundImporter::GetAnimationFileName(const TArray<FXmlNode*>& AddAnimNodes, FXmlNode* AnimNode, FString& OutFileName)
{
	check(AnimNode);
	const FString& AnimationName = AnimNode->GetAttribute(TEXT("name"));
	for (FXmlNode* Node : AddAnimNodes)
	{
		if (Node && Node->GetAttribute(TEXT("name")) == AnimationName)
		{
			OutFileName = Node->GetAttribute(TEXT("filename"));
			return true;
		}
	}
	return false;
}

TArray<FAnimSoundInfo> USoundImporter::GenerateAnimSoundInfoArray(
	const TArray<FXmlNode*>& AnimationNodes,
	const TArray<FXmlNode*>& AddAnimationNodes,
	const TArray<FXmlNode*>& SoundNodes,
	const TArray<FAssetData>& MeshAnimAssets,
	const TArray<FAssetData>& AllSoundAssets)
{
	FScopedSlowTask GenTask(AnimationNodes.Num(), FText::FromString("Generating AnimSoundInfo Array!"));
	GenTask.MakeDialog();

	TArray<FAnimSoundInfo> AnimSoundInfoArray;
	for (FXmlNode* AnimNode : AnimationNodes)
	{
		if (!AnimNode)
		{
			GenTask.EnterProgressFrame();
			continue;
		}

		FString AnimationFileName;
		bool bFoundFileName = GetAnimationFileName(AddAnimationNodes, AnimNode, AnimationFileName);
		if (!bFoundFileName)
		{
			GenTask.EnterProgressFrame();
			continue;
		}

		FAnimSoundInfo AnimSoundInfo = GetAnimSoundInfo(AnimNode, SoundNodes, AnimationFileName, MeshAnimAssets, AllSoundAssets);

		AnimSoundInfoArray.Add(AnimSoundInfo);
		GenTask.EnterProgressFrame();
	}
	return AnimSoundInfoArray;
}

FAnimSoundInfo USoundImporter::GetAnimSoundInfo(
	FXmlNode* AnimNode,
	const TArray<FXmlNode*>& SoundNodes,
	const FString& AnimationFileName,
	const TArray<FAssetData>& MeshAnimAssets,
	const TArray<FAssetData>& AllSoundAssets)
{
	check(AnimNode);
	const FString& AnimationName = AnimNode->GetAttribute(TEXT("name"));

	FAnimSoundInfo AnimSoundInfo;
	FString EditorAnimFileName = TEXT("A_") + URaiderzXmlUtilities::GetRaiderzBaseFileName(AnimationFileName);
	for (const FAssetData& AssetData : MeshAnimAssets)
	{
		if (AssetData.AssetName.ToString() != EditorAnimFileName)
		{
			continue;
		}

		FString LogMessage = FString("Found animation file: ") + EditorAnimFileName;
		PrintLog(LogMessage);

		AnimSoundInfo.AnimationName = AnimationName;
		AnimSoundInfo.AnimationFileName = AnimationFileName;
		AnimSoundInfo.AnimationAssetData = AssetData;
		AnimSoundInfo.FrameToSoundAssetMap = GetFrameToSoundAssetMap(AnimNode, SoundNodes, AllSoundAssets);
		break;
	}
	return AnimSoundInfo;
}

TMap<float, FAssetData> USoundImporter::GetFrameToSoundAssetMap(FXmlNode* AnimNode, const TArray<FXmlNode*>& SoundNodes, const TArray<FAssetData>& AllSoundAssets)
{
	TMap<float, FAssetData> FrameToSoundAssetMap;
	TArray<FXmlNode*> EventNodes = URaiderzXmlUtilities::GetNodesWithTag(AnimNode, TEXT("EVENT"));
	for (FXmlNode* EventNode : EventNodes)
	{
		if (!EventNode && EventNode->GetAttribute(TEXT("name")) != TEXT("sh_sound"))
		{
			continue;
		}

		FString FrameStr = EventNode->GetAttribute(TEXT("frame"));
		int32 Frame = FCString::Atoi(*FrameStr);
		float ActualFrame = float(Frame) / 160.f;

		FAssetData SoundAssetData = GetSoundAsset(EventNode, SoundNodes, AllSoundAssets);
		if (SoundAssetData.IsValid())
		{
			FrameToSoundAssetMap.Add(ActualFrame, SoundAssetData);
		}
	}
	return FrameToSoundAssetMap;
}

FString USoundImporter::GetEditorSoundName(FXmlNode* EventNode, const TArray<FXmlNode*>& SoundNodes)
{
	if (!EventNode || SoundNodes.Num() == 0)
	{
		return TEXT("");	
	}

	FString SoundName = EventNode->GetAttribute(TEXT("param1"));
	if (SoundName.StartsWith(TEXT("$vox_")))
	{
		if (CurrentMeshName == TEXT("hm"))
		{
			SoundName = SoundName.Replace(TEXT("$vox_"), TEXT("m1_"));
		}
		else if (CurrentMeshName == TEXT("hf"))
		{
			SoundName = SoundName.Replace(TEXT("$vox_"), TEXT("w1_"));
		}
	}

	for (FXmlNode* Node : SoundNodes)
	{
		if (!Node)
		{
			continue;
		}

		if (Node->GetAttribute(TEXT("name")) == SoundName)
		{
			const FString& SoundFilePath = Node->GetAttribute(TEXT("filename"));
			const FString& SoundFileName = FPaths::GetCleanFilename(SoundFilePath);
			const FString& ProbableEditorSoundName = URaiderzXmlUtilities::GetRaiderzBaseFileName(SoundFileName);
			const FString& EditorSoundName = PackageTools::SanitizePackageName(ProbableEditorSoundName);
			return  EditorSoundName;
		}
	}

	return TEXT("");
}

FAssetData USoundImporter::GetSoundAsset(FXmlNode* EventNode, const TArray<FXmlNode*>& SoundNodes, const TArray<FAssetData>& AllSoundAssets)
{
	if (!EventNode)
	{
		return FAssetData();
	}

	const FString& EditorSoundName = GetEditorSoundName(EventNode, SoundNodes);
	if (EditorSoundName == TEXT(""))
	{
		return FAssetData();
	}

	for (const FAssetData& SoundAsset : AllSoundAssets)
	{
		if (SoundAsset.AssetName.ToString() == EditorSoundName)
		{
			return SoundAsset;
		}
	}

	return FAssetData();
}

void USoundImporter::CreateAndApplySoundNotifies(const TArray<FAnimSoundInfo>& AnimSoundInfoArray, USoundAttenuation* AttenuationToApply)
{
	FScopedSlowTask AddNotifyTask(AnimSoundInfoArray.Num(), FText::FromString("Adding sound notifies!"));
	AddNotifyTask.MakeDialog();

	for (const FAnimSoundInfo& AnimSoundInfo : AnimSoundInfoArray)
	{
		UAnimSequenceBase* Animation = Cast<UAnimSequenceBase>(AnimSoundInfo.AnimationAssetData.GetAsset());
		if (!Animation)
		{
			continue;
			AddNotifyTask.EnterProgressFrame();
		}

		AddSoundNotifiesToAnimation(Animation, AnimSoundInfo.FrameToSoundAssetMap, AttenuationToApply);
		AddNotifyTask.EnterProgressFrame();
	}
}

void USoundImporter::AddSoundNotifiesToAnimation(UAnimSequenceBase* Animation, const TMap<float, FAssetData>& FrameToSoundAssetMap, USoundAttenuation* AttenuationToApply)
{
	check(Animation);
	for (const TPair<float, FAssetData>& FrameSoundPair : FrameToSoundAssetMap)
	{
		const FAssetData& SoundAssetData = FrameSoundPair.Value;
		USoundBase* Sound = Cast<USoundBase>(SoundAssetData.GetAsset());
		if (!Sound)
		{
			continue;
		}

		float ActualFrame = FrameSoundPair.Key;
		float FrameTime = ActualFrame * (1.f / 30.f);

		bool bNotifyExists = HasSoundNotify(Animation, FrameTime, Sound);
		if (bNotifyExists)
		{
			ApplyAttenuationToNotifySound(Animation, FrameTime, Sound, AttenuationToApply);

			// Skip adding notify if it already exists
			continue;
		}

		Animation->Modify();
		FAnimNotifyEvent NewEvent;
		NewEvent.TriggerTimeOffset = GetTriggerTimeOffsetForType(Animation->CalculateOffsetForNotify(FrameTime));
		//~ @todo proper track index
		NewEvent.TrackIndex = 0;

		UAnimNotify_PlaySound* SoundNotify = NewObject<UAnimNotify_PlaySound>(Animation, UAnimNotify_PlaySound::StaticClass(), SoundAssetData.AssetName, RF_NoFlags);
		if (SoundNotify)
		{
			if (Sound->AttenuationSettings != AttenuationToApply)
			{
				Sound->Modify();
				Sound->AttenuationSettings = AttenuationToApply;
				Sound->MarkPackageDirty();
			}

			SoundNotify->Sound = Sound;
			SoundNotify->bFollow = true;
			NewEvent.NotifyName = FName(*SoundNotify->GetNotifyName());
		}
		NewEvent.Notify = SoundNotify;
		NewEvent.Link(Animation, FrameTime);
		Animation->Notifies.Add(NewEvent);
		Animation->MarkPackageDirty();
	}
}

bool USoundImporter::HasSoundNotify(UAnimSequenceBase* Animation, float NotifyTime, USoundBase* NotifySound)
{
	if (!Animation)
	{
		return false;
	}

	UClass* SoundNotifyClass = UAnimNotify_PlaySound::StaticClass();
	check(SoundNotifyClass);

	for (const FAnimNotifyEvent& NotifyEvent : Animation->Notifies)
	{
		if (NotifyEvent.Notify && NotifyEvent.Notify->IsA(SoundNotifyClass))
		{
			UAnimNotify_PlaySound* SoundNotify = Cast<UAnimNotify_PlaySound>(NotifyEvent.Notify);
			if (SoundNotify && SoundNotify->Sound == NotifySound)
			{
				float Time = NotifyEvent.GetTime();
				if (FMath::IsNearlyEqual(Time, NotifyTime, 0.1f))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void USoundImporter::ApplyAttenuationToNotifySound(UAnimSequenceBase* Animation, float NotifyTime, USoundBase* NotifySound, USoundAttenuation* AttenuationToApply)
{
	if (!Animation || !NotifySound)
	{
		return;
	}

	UClass* SoundNotifyClass = UAnimNotify_PlaySound::StaticClass();
	check(SoundNotifyClass);

	for (const FAnimNotifyEvent& NotifyEvent : Animation->Notifies)
	{
		if (NotifyEvent.Notify && NotifyEvent.Notify->IsA(SoundNotifyClass))
		{
			UAnimNotify_PlaySound* SoundNotify = Cast<UAnimNotify_PlaySound>(NotifyEvent.Notify);
			if (SoundNotify && SoundNotify->Sound == NotifySound)
			{
				float Time = NotifyEvent.GetTime();
				if (FMath::IsNearlyEqual(Time, NotifyTime, 0.1f))
				{
					if (SoundNotify->Sound->AttenuationSettings != AttenuationToApply)
					{
						SoundNotify->Sound->Modify();
						SoundNotify->Sound->AttenuationSettings = AttenuationToApply;
						SoundNotify->Sound->MarkPackageDirty();
					}
					return;
				}
			}
		}
	}
	return;
}

void USoundImporter::FilterAnimSoundInfoArray(TArray<FAnimSoundInfo>& AnimSoundInfoArray)
{
	int32 Num = AnimSoundInfoArray.Num();

	FScopedSlowTask FilterTask(AnimSoundInfoArray.Num(), FText::FromString("Filtering anim sound info array!"));
	FilterTask.MakeDialog();

	for (int i = Num - 1; i >= 0; i--)
	{
		const FAnimSoundInfo& SoundInfo = AnimSoundInfoArray[i];
		for (int j = i - 1; j >= 0; j--)
		{
			const FAnimSoundInfo& ComInfo = AnimSoundInfoArray[j];
			if (SoundInfo.AnimationAssetData == ComInfo.AnimationAssetData)
			{
				AnimSoundInfoArray.RemoveAt(i);
				break;
			}
		}

		FilterTask.EnterProgressFrame();
	}
}
