// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "NotifyManager.h"

#include "AssetRegistryModule.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimNotifies/AnimNotify.h"

void UNotifyManager::CleanUpNotifiesFromAllAnimations(TSubclassOf<class UAnimNotify> NotifyClass)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	
	TArray<FAssetData> AnimationSequenceAssetData;
	bool bFoundAssets;
	bFoundAssets = AssetRegistryModule.Get().GetAssetsByClass(FName("AnimSequence"), AnimationSequenceAssetData, false);

	if (bFoundAssets)
	{
		for (FAssetData& AssetData : AnimationSequenceAssetData)
		{
			UAnimSequence* CurrentAnimSequence = Cast<UAnimSequence>(AssetData.GetAsset());

			TArray<int32> NotifyEventIndices;

			for (FAnimNotifyEvent& NotifyEvent : CurrentAnimSequence->Notifies)
			{
				if (NotifyEvent.Notify->IsA(NotifyClass))
				{
					NotifyEventIndices.Add(CurrentAnimSequence->Notifies.Find(NotifyEvent));
				}
			}

			NotifyEventIndices.Sort();
			
			for (int i = NotifyEventIndices.Num() - 1; i >= 0; i--)
			{
				CurrentAnimSequence->Modify();
				CurrentAnimSequence->Notifies.RemoveAt(i);
				CurrentAnimSequence->MarkPackageDirty();
					
				FString Message = CurrentAnimSequence->GetName() + FString(" has been cleaned");
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
			}
		}
	}
}

void UNotifyManager::CleanUpNotifyStatesFromAllAnimations(TSubclassOf<class UAnimNotifyState> NotifyStateClass)
{
}
