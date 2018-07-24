// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CollisionParser.h"
#include "Notifies/AnimNotify_RaidCollision.h"

#include "XmlFile.h"
#include "Misc/MessageDialog.h"
#include "DOM/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "AssetRegistryModule.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
// #include "Kismet2/KismetEditorUtilities.h"


const FName UCollisionParser::EditorDir_Models = FName("/Game/EOD/Model");
const FString UCollisionParser::AnimationNamesFilePath = FString("F:/Game Dev/Unreal Projects/EOD/data_files/animation_names.json");
const FString UCollisionParser::CollisionImportSettingsFilePath = FString("F:/Game Dev/Unreal Projects/EOD/data_files/CollisionImportSettings.json");
const FString UCollisionParser::AnimationsNotFoundFilePath = FString("F:/Game Dev/Unreal Projects/EOD/data_files/AnimationsNotFound.txt");
const FString UCollisionParser::ProcessedAnimationsListFilePath = FString("F:/Game Dev/Unreal Projects/EOD/data_files/ProcessedAnimationsList.txt");


void UCollisionParser::ImportAndApplyCollisionInfo()
{
	FString CollisionImportSettingsFileString;
	FFileHelper::LoadFileToString(CollisionImportSettingsFileString, *UCollisionParser::CollisionImportSettingsFilePath);
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(CollisionImportSettingsFileString);
	
	FString FilePath_CollisionInfo;
	FString FilePath_AnimationInfo;
	FString FilePath_NPCInfo;
	int NumberOfCollisionsToParse;
	
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		FilePath_CollisionInfo = JsonObject->GetStringField("XmlFile_TalentHitInfo");
		FilePath_AnimationInfo = JsonObject->GetStringField("XmlFile_Talent");
		FilePath_NPCInfo = JsonObject->GetStringField("XmlFile_NPC");
		NumberOfCollisionsToParse = JsonObject->GetIntegerField("NumberOfCollisionsToParse");
	}
	else
	{
		// If settings json file doesn't exist OR if we were unable to parse json file, then simply return without doing anything.
		return;
	}
	
	FXmlFile CollisionInfoXmlFilePath(FilePath_CollisionInfo);

	FXmlNode* CollisionInfoRoot = CollisionInfoXmlFilePath.GetRootNode();

	// If we are unable to parse collision info file
	if (!CollisionInfoRoot)
	{
		UE_LOG(LogTemp, Warning, TEXT("unable to parse talent_hit_info.xml"));
		return;
	}
	
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	
	TArray<FAssetData> AssetData_Temp;
	bool bFoundAssets;
	bFoundAssets = AssetRegistryModule.Get().GetAssetsByClass(FName("AnimSequence"), AssetData_Temp, false);

	TMap<FString, FAssetData> AssetDataMap_Animations;
	if (bFoundAssets)
	{
		for (FAssetData AssetData : AssetData_Temp)
		{
			FString FullName = AssetData.GetFullName();
			// AssetData.GetPackage()
			// UE_LOG(LogTemp, Warning, TEXT("%s"), *FullName);
			AssetDataMap_Animations.Add(FullName, AssetData);
		}
	}
	
	TArray<FString> Read_ProcessedAnimationsList;
	TArray<FString> Write_ProcessedAnimationsList;

	FFileHelper::LoadFileToStringArray(Read_ProcessedAnimationsList, *UCollisionParser::ProcessedAnimationsListFilePath);
	Write_ProcessedAnimationsList += Read_ProcessedAnimationsList;

	int NumOfParsedCollision = 0;
	TArray<FAnimationHitInfo> AnimHitInfoArray;
	TArray<FXmlNode*> TALENT_HIT_Nodes = CollisionInfoRoot->GetChildrenNodes();
	for (FXmlNode* TALENT_HIT_node : TALENT_HIT_Nodes)
	{
		if (!TALENT_HIT_node->FindChildNode(FString("HitSegment")))
		{
			continue;
		}


		FAnimationHitInfo AnimHitInfo;
		AnimHitInfo.TALENT_HIT_id = TALENT_HIT_node->GetAttribute(FString("id"));
		FString ModeString = TALENT_HIT_node->GetAttribute(FString("Mode"));

		TArray<FString> AnimationNames;

		if (ModeString == FString(""))
		{
			AnimationNames = UCollisionParser::GetAnimationNamesFromTalentID(AnimHitInfo.TALENT_HIT_id);
		}
		else
		{
			AnimationNames = UCollisionParser::GetAnimationNamesFromTalentID(AnimHitInfo.TALENT_HIT_id + FString("_") + ModeString);
		}

		for (FString& AnimationName : AnimationNames)
		{
			FString StringSearch = FString("AnimSequence ") + AnimationName;
			if (!AssetDataMap_Animations.Contains(StringSearch))
			{
				continue;
			}
			
			if (Read_ProcessedAnimationsList.Contains(AnimationName))
			{
				continue;
			}

			TArray<FXmlNode*> HitSegment_Nodes = TALENT_HIT_node->GetChildrenNodes();
			for (FXmlNode* HitSegment_node : HitSegment_Nodes)
			{
				FRaidCollision RaidCollisionInfo;
				RaidCollisionInfo.Time = FCString::Atof(*HitSegment_node->GetAttribute(FString("CheckTime")));
			
				TArray<FXmlNode*> Capsule_Nodes = HitSegment_node->GetChildrenNodes();

				for (FXmlNode* Capsule_node : Capsule_Nodes)
				{
					FRaidCapsule CapsuleInfo;

					FString CapsuleString = Capsule_node->GetContent();
					TArray<FString> CapsuleContentArray;
					CapsuleString.ParseIntoArray(CapsuleContentArray, TEXT(" "));

					CapsuleInfo.Bottom.X = FCString::Atof(*CapsuleContentArray[0]);
					CapsuleInfo.Bottom.Y = FCString::Atof(*CapsuleContentArray[1]);
					CapsuleInfo.Bottom.Z = FCString::Atof(*CapsuleContentArray[2]);
					CapsuleInfo.Top.X = FCString::Atof(*CapsuleContentArray[3]);
					CapsuleInfo.Top.Y = FCString::Atof(*CapsuleContentArray[4]);
					CapsuleInfo.Top.Z = FCString::Atof(*CapsuleContentArray[5]);
					CapsuleInfo.Radius = FCString::Atof(*CapsuleContentArray[6]);

					RaidCollisionInfo.Capsules.Add(CapsuleInfo);
				}

				AnimHitInfo.RaidCollisions.Add(RaidCollisionInfo);
			}
			
			UAnimSequence* CurrentAnimSequence = Cast<UAnimSequence>(AssetDataMap_Animations[StringSearch].GetAsset());

			for (FRaidCollision& CollisionInfo : AnimHitInfo.RaidCollisions)
			{
				bool bNotifyExists = false;
				for (FAnimNotifyEvent& NotifyEvent : CurrentAnimSequence->Notifies)
				{
					if (NotifyEvent.NotifyName == FName("RaidCollision") && NotifyEvent.GetTriggerTime() == CollisionInfo.Time)
					{
						UAnimNotify_RaidCollision* Notify = Cast<UAnimNotify_RaidCollision>(NotifyEvent.Notify);

						if (!Notify)
						{
							continue;
						}
						
						for (FRaidCapsule& NotifyCapsule : Notify->CollisionCapsules)
						{
							for (FRaidCapsule& InfoCapsule : CollisionInfo.Capsules)
							{
								if (NotifyCapsule == InfoCapsule)
								{
									bNotifyExists = true;
									break;
								}
							}

							if (bNotifyExists)
							{
								break;
							}
						}
					}
				}

				// If notify doesn't exist, create it
				if (!bNotifyExists)
				{
					CurrentAnimSequence->Modify();
					// CurrentAnimSequence->MarkPackageDirty();
					int32 NewNotifyIndex = CurrentAnimSequence->Notifies.Add(FAnimNotifyEvent());
					FAnimNotifyEvent& NewEvent = CurrentAnimSequence->Notifies[NewNotifyIndex];				
					NewEvent.NotifyName = FName("RaidCollision");

					NewEvent.Link(CurrentAnimSequence, CollisionInfo.Time);
					NewEvent.TriggerTimeOffset = GetTriggerTimeOffsetForType(CurrentAnimSequence->CalculateOffsetForNotify(CollisionInfo.Time));
					NewEvent.TrackIndex = 0; // Let's create a global index convention perhaps?

					UObject* AnimNotify = NewObject<UObject>(CurrentAnimSequence, UAnimNotify_RaidCollision::StaticClass(), NAME_None, RF_Transactional);
					NewEvent.Notify = Cast<UAnimNotify>(AnimNotify);

					if (NewEvent.Notify)
					{
						UAnimNotify_RaidCollision* RaidCollisionNotify = Cast<UAnimNotify_RaidCollision>(NewEvent.Notify);
						if (RaidCollisionNotify)
						{
							RaidCollisionNotify->CollisionCapsules = CollisionInfo.Capsules;
						}

						NewEvent.NotifyName = FName(*NewEvent.Notify->GetNotifyName());

						// @todo since we are not creating the anim notify in editor, this might mess everything up. See what it does
						NewEvent.Notify->OnAnimNotifyCreatedInEditor(NewEvent);
					}

					CurrentAnimSequence->MarkPackageDirty();

				}
			}


			++NumOfParsedCollision;
			Write_ProcessedAnimationsList.Add(AnimationName);
			FString Message = FString("Processed animation: ") + AnimationName;
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
		}

		if (NumOfParsedCollision >= NumberOfCollisionsToParse)
		{
			FString DialogString = FString::FromInt(NumberOfCollisionsToParse) + FString(" animations have been parsed. Continue?");
			FText DialogText = FText::FromString(DialogString);
			EAppReturnType::Type AppReturn = FMessageDialog::Open(EAppMsgType::YesNo, DialogText);

			if (AppReturn == EAppReturnType::No)
			{
				break;
			}
			else
			{
				NumOfParsedCollision = 0;
			}
		}

	}

	FFileHelper::SaveStringArrayToFile(Write_ProcessedAnimationsList, *UCollisionParser::ProcessedAnimationsListFilePath);

}

TArray<FString> UCollisionParser::GetAnimationNamesFromTalentID(FString TalentID)
{
	TArray<FString> AnimationNames;

	FString AnimationNamesString;
	FFileHelper::LoadFileToString(AnimationNamesString, *UCollisionParser::AnimationNamesFilePath);
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(AnimationNamesString);
	
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject->HasField(TalentID))
	{
		TArray<TSharedPtr<FJsonValue>> AnimationsArray = JsonObject->GetArrayField(TalentID);

		for (int i = 0; i < AnimationsArray.Num(); i++)
		{
			FString AnimationName = AnimationsArray[i]->AsString();
			AnimationNames.Add(AnimationName);
		}
	}

	return AnimationNames;
}

