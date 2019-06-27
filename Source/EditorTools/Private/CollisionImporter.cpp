// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "CollisionImporter.h"
#include "EOD.h"
#include "RaiderzXmlUtilities.h"
#include "EditorFunctionLibrary.h"
#include "AnimNotify_CapsuleCollision.h"

#include "Engine/SkeletalMesh.h"
#include "Misc/ScopedSlowTask.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimSequence.h"

FString UCollisionImporter::CurrentMeshName(TEXT(""));

UCollisionImporter::UCollisionImporter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCollisionImporter::ImportCollisionForSkeletalMesh(USkeletalMesh* Mesh)
{
	if (Mesh == nullptr)
	{
		PrintError(TEXT("Import failed because skeletal mesh is not valid"));
		return;
	}

	CurrentMeshName = UEditorFunctionLibrary::GetRaiderZMeshName(Mesh);
	check(CurrentMeshName != TEXT(""));

	FScopedSlowTask SlowTask(4, FText::FromString("Finding and parsing XML files!"));
	SlowTask.MakeDialog();

	FXmlFile NPCFileObj(URaiderzXmlUtilities::NPCXmlFilePath);
	FXmlNode* NPCRootNode = NPCFileObj.GetRootNode();
	FXmlNode* NPCNode = GetNPCNode(NPCRootNode, CurrentMeshName);
	if (NPCNode == nullptr)
	{
		PrintError(TEXT("Import failed because we couldn't find a proper NPC ID for the given skeletal mesh"));
		return;
	}
	SlowTask.EnterProgressFrame();

	const FString& NPCID = NPCNode->GetAttribute(TEXT("id"));
	const FString& NPCAniPrefix = NPCNode->GetAttribute(TEXT("AniPrefix"));

	FXmlFile TalentFileObj(URaiderzXmlUtilities::TalentXmlFilePath);
	FXmlNode* TalentRootNode = TalentFileObj.GetRootNode();
	TArray<FXmlNode*> TalentNodes = GetNPCTalents(TalentRootNode, NPCID);
	if (TalentNodes.Num() == 0)
	{
		PrintWarning(TEXT("Couldn't find any talent associated with the given skeletal mesh"));
		return;
	}
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

	FXmlFile TalentHitInfoFileObj(URaiderzXmlUtilities::TalentHitInfoXmlFilePath);
	FXmlNode* RootTalentHitInfoNode = TalentHitInfoFileObj.GetRootNode();
	TArray<FXmlNode*> TalentHitNodes = URaiderzXmlUtilities::GetNodesWithTag(RootTalentHitInfoNode, TEXT("TALENT_HIT"));
	check(TalentHitNodes.Num() != 0);
	SlowTask.EnterProgressFrame();

	TArray<FAssetData> MeshAnimAssets = UEditorFunctionLibrary::GetAllAnimationsForSkeletalMesh(Mesh);
	TArray<FCollisionInfo> CollisionInfoArray = GenerateCollisionInfoArray(NPCNode, TalentNodes, AddAnimationNodes, TalentHitNodes, MeshAnimAssets);

	CreateAndApplyCollisionNotifies(CollisionInfoArray);
}

TArray<FCollisionInfo> UCollisionImporter::GenerateCollisionInfoArray(
	FXmlNode* NPCNode,
	const TArray<FXmlNode*>& TalentNodes,
	const TArray<FXmlNode*>& AddAnimNodes,
	const TArray<FXmlNode*>& TalentHitNodes,
	const TArray<FAssetData>& MeshAnimAssets)
{
	FScopedSlowTask GenTask(TalentNodes.Num(), FText::FromString("Generating CollisionInfo Array!"));
	GenTask.MakeDialog();

	if (TalentNodes.Num() == 0)
	{
		return TArray<FCollisionInfo>();
	}

	TArray<FCollisionInfo> CollisionInfoArray;
	for (FXmlNode* TalentNode : TalentNodes)
	{
		if (!TalentNode)
		{
			GenTask.EnterProgressFrame();
			continue;
		}

		FString AnimationFileName;
		bool bFoundFileName = GetAnimationFileName(AddAnimNodes, TalentNode, NPCNode, AnimationFileName);
		if (!bFoundFileName)
		{
			GenTask.EnterProgressFrame();
			continue;
		}

		FCollisionInfo CollisionInfo;
		bool bSuccess = GetCollisionInfo(NPCNode, TalentNode, AddAnimNodes, TalentHitNodes, AnimationFileName, MeshAnimAssets, CollisionInfo);
		if (bSuccess)
		{
			CollisionInfoArray.Add(CollisionInfo);
		}
		else
		{
			FString Warning = TEXT("Unable to generate collision info for TalentID: ") + TalentNode->GetAttribute("id");
			PrintWarning(Warning);
		}

		GenTask.EnterProgressFrame();
	}
	return CollisionInfoArray;
}

bool UCollisionImporter::GetCollisionInfo(
	FXmlNode* NPCNode,
	FXmlNode* TalentNode,
	const TArray<FXmlNode*>& AddAnimNodes,
	const TArray<FXmlNode*>& TalentHitNodes,
	const FString& AnimationFileName,
	const TArray<FAssetData>& MeshAnimAssets,
	FCollisionInfo& OutCollisionInfo)
{
	check(TalentNode);
	const FString& AnimationName = TalentNode->GetAttribute(TEXT("UseAni"));
	const FString& TalentID = TalentNode->GetAttribute(TEXT("id"));

	FString EditorAnimFileName = TEXT("A_") + URaiderzXmlUtilities::GetRaiderzBaseFileName(AnimationFileName);
	for (const FAssetData& AssetData : MeshAnimAssets)
	{
		if (AssetData.AssetName.ToString() != EditorAnimFileName)
		{
			continue;
		}

		FString LogMessage = FString("Found animation file: ") + EditorAnimFileName;
		PrintLog(LogMessage);

		OutCollisionInfo.TalentID = TalentID;
		OutCollisionInfo.AnimationName = AnimationName;
		OutCollisionInfo.AnimationFileName = AnimationFileName;
		OutCollisionInfo.AnimationAssetData = AssetData;
		OutCollisionInfo.FrameToCollisionStringMap = GetFrameToCollisionStringMap(TalentHitNodes, TalentNode, NPCNode);
		return true;
	}

	return false;
}

TMap<FString, TArray<FString>> UCollisionImporter::GetFrameToCollisionStringMap(const TArray<FXmlNode*>& TalentHitNodes, FXmlNode* TalentNode, FXmlNode* NPCNode)
{
	check(NPCNode && TalentNode);
	check(TalentHitNodes.Num() != 0);

	const FString& TalentID = TalentNode->GetAttribute(TEXT("id"));
	FXmlNode* TalentHitNode = GetTalentHitNodeWithID(TalentHitNodes, TalentID);

	TMap<FString, TArray<FString>> FrameToCollisionStringMap;
	TArray<FXmlNode*> HitSegmentNodes = URaiderzXmlUtilities::GetNodesWithTag(TalentHitNode, TEXT("HitSegment"));
	for (FXmlNode* HitSegmentNode : HitSegmentNodes)
	{
		check(HitSegmentNode);
		const FString& CheckTime = HitSegmentNode->GetAttribute(TEXT("CheckTime"));

		FrameToCollisionStringMap.Add(CheckTime);
		TArray<FXmlNode*> CapsuleNodes = URaiderzXmlUtilities::GetNodesWithTag(HitSegmentNode, TEXT("Capsule"));
		for (FXmlNode* CapsuleNode : CapsuleNodes)
		{
			check(CapsuleNode);
			const FString& CapsuleStr = CapsuleNode->GetContent();
			FrameToCollisionStringMap[CheckTime].Add(CapsuleStr);
		}
	}
	return FrameToCollisionStringMap;
}

FXmlNode* UCollisionImporter::GetTalentHitNodeWithID(const TArray<FXmlNode*>& TalentHitNodes, const FString& TalentID)
{
	for (FXmlNode* HitNode : TalentHitNodes)
	{
		if (HitNode && HitNode->GetAttribute(TEXT("id")) == TalentID)
		{
			return HitNode;
		}
	}

	return nullptr;
}

bool UCollisionImporter::GetAnimationFileName(const TArray<FXmlNode*>& AddAnimNodes, FXmlNode* TalentNode, FXmlNode* NPCNode, FString& OutFileName)
{
	check(TalentNode);
	check(NPCNode);

	const FString& AniPrefix = NPCNode->GetAttribute(TEXT("AniPrefix"));
	const FString& AnimationName = TalentNode->GetAttribute(TEXT("UseAni"));
	const FString& PrefixedAniName = AniPrefix + AnimationName;

	//~ @todo CastingAni
	if (AnimationName == TEXT(""))
	{
		return false;
	}

	for (FXmlNode* Node : AddAnimNodes)
	{
		check(Node);
		if (Node->GetAttribute(TEXT("name")) == AnimationName || Node->GetAttribute("name") == PrefixedAniName)
		{
			OutFileName = Node->GetAttribute(TEXT("filename"));
			return true;
		}
	}

	// If we couldn't find the animation file in previous loop, try again (sometimes the npc.xml doesn't contain the AniPrefix)
	for (FXmlNode* Node : AddAnimNodes)
	{
		check(Node);
		const FString& NodeAttri = Node->GetAttribute(TEXT("name"));
		if (NodeAttri.Contains(AnimationName))
		{
			OutFileName = Node->GetAttribute(TEXT("filename"));
			return true;
		}
	}

	return false;
}

void UCollisionImporter::CreateAndApplyCollisionNotifies(const TArray<FCollisionInfo>& CollisionInfoArray)
{
	FScopedSlowTask AddNotifyTask(CollisionInfoArray.Num(), FText::FromString("Adding collision notifies!"));
	AddNotifyTask.MakeDialog();

	for (const FCollisionInfo& CollisionInfo : CollisionInfoArray)
	{
		UAnimSequenceBase* Animation = Cast<UAnimSequenceBase>(CollisionInfo.AnimationAssetData.GetAsset());
		if (!Animation)
		{
			continue;
			AddNotifyTask.EnterProgressFrame();			
		}

		AddCollisionNotifiesToAnimation(Animation, CollisionInfo.FrameToCollisionStringMap);
		AddNotifyTask.EnterProgressFrame();
	}
}

void UCollisionImporter::AddCollisionNotifiesToAnimation(UAnimSequenceBase* Animation, const TMap<FString, TArray<FString>>& FrameToCollisionStringMap)
{
	check(Animation);
	for (const TPair<FString, TArray<FString>>& FrameCollisionPair : FrameToCollisionStringMap)
	{
		float FrameTime = FCString::Atof(*FrameCollisionPair.Key);
		TArray<FRaidCapsule> RaidCapsules = GenerateRaidCapsules(FrameCollisionPair.Value);

		bool bCollsionNotifyExists = HasCollisionNotify(Animation, FrameTime, RaidCapsules);
		if (bCollsionNotifyExists)
		{
			// Skip adding notify if it already exists
			continue;
		}

		Animation->Modify();
		int32 NewNotifyIndex = Animation->Notifies.Add(FAnimNotifyEvent());
		FAnimNotifyEvent& NewEvent = Animation->Notifies[NewNotifyIndex];
		NewEvent.NotifyName = TEXT("Capsule Collision");

		NewEvent.Link(Animation, FrameTime);
		NewEvent.TriggerTimeOffset = GetTriggerTimeOffsetForType(Animation->CalculateOffsetForNotify(FrameTime));
		NewEvent.TrackIndex = 0; // Let's create a global index convention perhaps?

		UAnimNotify_CapsuleCollision* AnimNotify = NewObject<UAnimNotify_CapsuleCollision>(Animation, UAnimNotify_CapsuleCollision::StaticClass(), NAME_None, RF_NoFlags);
		NewEvent.Notify = AnimNotify;
		if (AnimNotify)
		{
			AnimNotify->InitializeFromRaidCapsules(RaidCapsules);
			NewEvent.NotifyName = FName(*NewEvent.Notify->GetNotifyName());
			NewEvent.Notify->OnAnimNotifyCreatedInEditor(NewEvent);			
		}

		Animation->MarkPackageDirty();
	}
}

TArray<FRaidCapsule> UCollisionImporter::GenerateRaidCapsules(const TArray<FString>& CapsuleStrings)
{
	TArray<FRaidCapsule> RaidCapsules;
	for (const FString& CapString : CapsuleStrings)
	{
		TArray<FString> CapsuleContentArray;
		CapString.ParseIntoArray(CapsuleContentArray, TEXT(" "));
		check(CapsuleContentArray.Num() == 7);

		FRaidCapsule RaidCapsule;
		RaidCapsule.Bottom.X = FCString::Atof(*CapsuleContentArray[0]);
		RaidCapsule.Bottom.Y = FCString::Atof(*CapsuleContentArray[1]);
		RaidCapsule.Bottom.Z = FCString::Atof(*CapsuleContentArray[2]);
		RaidCapsule.Top.X = FCString::Atof(*CapsuleContentArray[3]);
		RaidCapsule.Top.Y = FCString::Atof(*CapsuleContentArray[4]);
		RaidCapsule.Top.Z = FCString::Atof(*CapsuleContentArray[5]);
		RaidCapsule.Radius = FCString::Atof(*CapsuleContentArray[6]);

		RaidCapsules.Add(RaidCapsule);
	}
	return RaidCapsules;
}

bool UCollisionImporter::HasCollisionNotify(UAnimSequenceBase* Animation, float FrameTime, const TArray<FRaidCapsule>& RaidCapsules)
{
	return false;
}

FXmlNode* UCollisionImporter::GetNPCNode(FXmlNode* NPCRootNode, const FString& MeshName)
{
	TArray<FXmlNode*> NPCNodes = URaiderzXmlUtilities::GetNodesWithTag(NPCRootNode, TEXT("NPC"));
	for (FXmlNode* Node : NPCNodes)
	{
		if (Node && Node->GetAttribute(TEXT("MeshName")) == MeshName)
		{
			return Node;
		}
	}
	return nullptr;
}

TArray<FXmlNode*> UCollisionImporter::GetNPCTalents(FXmlNode* TalentRootNode, const FString& InNPCID)
{
	TArray<FXmlNode*> AllTalentNodes = URaiderzXmlUtilities::GetNodesWithTag(TalentRootNode, TEXT("TALENT"));
	TArray<FXmlNode*> ResultNodes;
	for (FXmlNode* Node : AllTalentNodes)
	{
		FString NPCAttribute = Node ? Node->GetAttribute(TEXT("NPC")) : TEXT("");
		if (NPCAttribute == TEXT(""))
		{
			continue;
		}

		TArray<FString> NPCIDs;
		NPCAttribute.ParseIntoArray(NPCIDs, TEXT(","));

		for (FString NPCID : NPCIDs)
		{
			FString TrimmedNPCID = NPCID.TrimStartAndEnd();
			if (TrimmedNPCID == InNPCID)
			{
				ResultNodes.Add(Node);
				break;
			}
		}
	}
	return ResultNodes;
}
