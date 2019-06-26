// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SoundImporter.h"
#include "EOD.h"
#include "EditorFunctionLibrary.h"
#include "RaiderzXmlUtilities.h"

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


USoundImporter::USoundImporter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USoundImporter::ImportSoundForSkeletalMesh(USkeletalMesh* Mesh, USoundAttenuation* AttenuationToApply)
{
	bool bIsPlayerMesh = UEditorFunctionLibrary::IsHumanPlayerMesh(Mesh);

	TArray<FAssetData> MeshAnimAssets = UEditorFunctionLibrary::GetAllAnimationsForSkeletalMesh(Mesh);
	if (MeshAnimAssets.Num() == 0)
	{
		PrintError(TEXT("Import failed because we couldn't find any animations"));
		return;
	}

	FString FullMeshName = Mesh->GetFName().ToString();
	FString MeshName = FullMeshName.RightChop(3);

	FString SoundEventFilePath;
	bool bFoundSoundXml = URaiderzXmlUtilities::GetRaiderzFilePath(MeshName + URaiderzXmlUtilities::EluAnimationSoundEventXmlExt, SoundEventFilePath);
	if (!bFoundSoundXml)
	{
		PrintError(TEXT("Couldn't find animationsoundevent.xml file for the given skeletal mesh"));
		return;
	}
	FXmlFile SoundEventFileObj(SoundEventFilePath);
	FXmlNode* RootSoundEventNode = SoundEventFileObj.GetRootNode();
	TArray<FXmlNode*> AnimationNodes = URaiderzXmlUtilities::GetNodesWithTag(RootSoundEventNode, TEXT("Animation"));


	FString AnimXmlFilePath;
	bool bFoundAnimXml = URaiderzXmlUtilities::GetRaiderzFilePath(MeshName + URaiderzXmlUtilities::EluAnimationXmlExt, AnimXmlFilePath);
	if (!bFoundAnimXml)
	{
		PrintError(TEXT("Couldn't find .elu.animation.xml file for the given skeletal mesh"));
		return;
	}
	FXmlFile AnimFileObj(AnimXmlFilePath);
	FXmlNode* RootAnimNode = AnimFileObj.GetRootNode();
	TArray<FXmlNode*> AddAnimationNodes = URaiderzXmlUtilities::GetNodesWithTag(RootAnimNode, TEXT("AddAnimation"));

	TArray<FAssetData> AllSoundAssets = UEditorFunctionLibrary::GetAllSoundAssets();
	TArray<FAnimSoundInfo> AnimSoundInfoArray = GenerateAnimSoundInfoArray(AnimationNodes, AddAnimationNodes, MeshAnimAssets, AllSoundAssets);

	CreateAndApplySoundNotifies(AnimSoundInfoArray, AttenuationToApply);
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

		FAnimSoundInfo AnimSoundInfo = GetAnimSoundInfo(AnimNode, AnimationFileName, MeshAnimAssets, AllSoundAssets);

		AnimSoundInfoArray.Add(AnimSoundInfo);
		GenTask.EnterProgressFrame();
	}
	return AnimSoundInfoArray;
}

FAnimSoundInfo USoundImporter::GetAnimSoundInfo(
	FXmlNode* AnimNode,
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
		AnimSoundInfo.FrameToSoundAssetMap = GetFrameToSoundAssetMap(AnimNode, AllSoundAssets);
		break;
	}
	return AnimSoundInfo;
}

TMap<float, FAssetData> USoundImporter::GetFrameToSoundAssetMap(FXmlNode* AnimNode, const TArray<FAssetData>& AllSoundAssets)
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

		FAssetData SoundAssetData = GetSoundAsset(EventNode, AllSoundAssets);
		if (SoundAssetData.IsValid())
		{
			FrameToSoundAssetMap.Add(ActualFrame, SoundAssetData);
		}
	}
	return FrameToSoundAssetMap;
}

FAssetData USoundImporter::GetSoundAsset(FXmlNode* EventNode, const TArray<FAssetData>& AllSoundAssets)
{
	if (!EventNode)
	{
		return FAssetData();
	}

	const FString& SoundName = EventNode->GetAttribute(TEXT("param1"));
	if (SoundName == TEXT(""))
	{
		return FAssetData();
	}

	for (const FAssetData& SoundAsset : AllSoundAssets)
	{
		if (SoundAsset.AssetName.ToString() == SoundName)
		{
			return SoundAsset;
		}
	}

	return FAssetData();
}

void USoundImporter::CreateAndApplySoundNotifies(const TArray<FAnimSoundInfo>& AnimSoundInfoArray, USoundAttenuation* AttenuationToApply)
{
	for (const FAnimSoundInfo& AnimSoundInfo : AnimSoundInfoArray)
	{
		UAnimSequenceBase* Animation = Cast<UAnimSequenceBase>(AnimSoundInfo.AnimationAssetData.GetAsset());
		if (!Animation)
		{
			continue;
		}

		AddSoundNotifiesToAnimation(Animation, AnimSoundInfo.FrameToSoundAssetMap, AttenuationToApply);
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
			// Skip adding notify if it already exists
			continue;
		}

		Animation->Modify();
		int32 NewNotifyIndex = Animation->Notifies.Add(FAnimNotifyEvent());
		FAnimNotifyEvent& NewEvent = Animation->Notifies[NewNotifyIndex];
		NewEvent.NotifyName = TEXT("Play Sound");

		NewEvent.Link(Animation, FrameTime);
		NewEvent.TriggerTimeOffset = GetTriggerTimeOffsetForType(Animation->CalculateOffsetForNotify(FrameTime));
		NewEvent.TrackIndex = 0; // Let's create a global index convention perhaps?

		UObject* AnimNotify = NewObject<UObject>(Animation, UAnimNotify_PlaySound::StaticClass(), SoundAssetData.AssetName, RF_NoFlags);
		UAnimNotify_PlaySound* SoundNotify = Cast<UAnimNotify_PlaySound>(AnimNotify);
		NewEvent.Notify = SoundNotify;
		if (SoundNotify)
		{
			Sound->Modify();
			Sound->AttenuationSettings = AttenuationToApply;
			Sound->MarkPackageDirty();

			SoundNotify->Sound = Sound;
			SoundNotify->bFollow = true;
			NewEvent.NotifyName = FName(*NewEvent.Notify->GetNotifyName());
			NewEvent.Notify->OnAnimNotifyCreatedInEditor(NewEvent);
		}

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
