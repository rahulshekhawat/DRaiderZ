// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "NotifyParser.h"
#include "EditorFunctionLibrary.h"

#include "AssetData.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Animation/AnimSequenceBase.h"

UNotifyParser::UNotifyParser(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UNotifyParser::DeleteAllNotifies(USkeletalMesh* SkeletalMesh)
{
	TArray<FAssetData> AnimationAssets = UEditorFunctionLibrary::GetAllAnimationsForSkeletalMesh(SkeletalMesh);
	int32 AnimNum = AnimationAssets.Num();
	if (AnimNum == 0)
	{
		return;
	}

	for (int i = 0; i < AnimNum; i++)
	{
		const FAssetData& AssetData = AnimationAssets[i];
		UAnimSequenceBase* Animation = Cast<UAnimSequenceBase>(AssetData.GetAsset());
		
		if (Animation && Animation->Notifies.Num() > 0)
		{
			Animation->Modify();
			Animation->Notifies.Empty();
			Animation->AnimNotifyTracks.Empty();
			Animation->MarkPackageDirty();
		}
	}
}

void UNotifyParser::DeleteAllNotifiesOfClass(USkeletalMesh* SkeletalMesh, TSubclassOf<UAnimNotify> NotifyClass)
{
	TArray<FAssetData> AnimationAssets = UEditorFunctionLibrary::GetAllAnimationsForSkeletalMesh(SkeletalMesh);
	int32 AnimNum = AnimationAssets.Num();
	if (AnimNum == 0)
	{
		return;
	}

	for (int i = 0; i < AnimNum; i++)
	{
		const FAssetData& AssetData = AnimationAssets[i];
		UAnimSequenceBase* Animation = Cast<UAnimSequenceBase>(AssetData.GetAsset());
		if (!Animation)
		{
			continue;
		}

		int32 NotifyNum = Animation->Notifies.Num();
		for (int j = NotifyNum - 1; j >= 0; j--)
		{
			const FAnimNotifyEvent& NotifyEvent = Animation->Notifies[j];
			if (NotifyEvent.Notify && NotifyEvent.Notify->IsA(NotifyClass))
			{
				Animation->Modify();
				Animation->Notifies.RemoveAt(j);
				//~ @todo notify tracks
				// Animation->AnimNotifyTracks.Empty();
				Animation->MarkPackageDirty();
			}
		}
	}
}

void UNotifyParser::DeleteAllNotifyStatesOfClass(USkeletalMesh* SkeletalMesh, TSubclassOf<UAnimNotifyState> NotifyClass)
{
	TArray<FAssetData> AnimationAssets = UEditorFunctionLibrary::GetAllAnimationsForSkeletalMesh(SkeletalMesh);
	int32 AnimNum = AnimationAssets.Num();
	if (AnimNum == 0)
	{
		return;
	}

	for (int i = 0; i < AnimNum; i++)
	{
		const FAssetData& AssetData = AnimationAssets[i];
		UAnimSequenceBase* Animation = Cast<UAnimSequenceBase>(AssetData.GetAsset());
		if (!Animation)
		{
			continue;
		}

		int32 NotifyNum = Animation->Notifies.Num();
		for (int j = NotifyNum - 1; j >= 0; j--)
		{
			const FAnimNotifyEvent& NotifyEvent = Animation->Notifies[j];
			if (NotifyEvent.NotifyStateClass && NotifyEvent.NotifyStateClass->IsA(NotifyClass))
			{
				Animation->Modify();
				Animation->Notifies.RemoveAt(j);
				//~ @todo notify tracks
				// Animation->AnimNotifyTracks.Empty();
				Animation->MarkPackageDirty();
			}
		}
	}
}
