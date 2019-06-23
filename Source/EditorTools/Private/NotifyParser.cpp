// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "NotifyParser.h"
#include "EditorFunctionLibrary.h"

#include "AssetData.h"
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
		
		if (Animation)
		{
			Animation->Modify();


			Animation->MarkPackageDirty();
		}
	}
}
