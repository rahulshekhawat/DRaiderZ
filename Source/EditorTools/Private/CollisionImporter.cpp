// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "CollisionImporter.h"
#include "EOD.h"
#include "RaiderzXmlUtilities.h"
#include "EditorFunctionLibrary.h"

#include "Engine/SkeletalMesh.h"

FString UCollisionImporter::CurrentMeshName(TEXT(""));

UCollisionImporter::UCollisionImporter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCollisionImporter::ImportCollisionForSkeletalMesh(USkeletalMesh* Mesh)
{
	if (Mesh == nullptr)
	{
		return;
	}

	CurrentMeshName = UEditorFunctionLibrary::GetRaiderZMeshName(Mesh);
	check(CurrentMeshName != TEXT(""));

	FXmlFile NPCFileObj(URaiderzXmlUtilities::NPCXmlFilePath);
	FXmlNode* NPCRootNode = NPCFileObj.GetRootNode();

	FXmlNode* NPCNode = GetNPCNode(NPCRootNode, CurrentMeshName);
	if (NPCNode == nullptr)
	{
		return;
	}

	const FString& NPCID = NPCNode->GetAttribute(TEXT("id"));
	const FString& NPCAniPrefix = NPCNode->GetAttribute(TEXT("AniPrefix"));

	FXmlFile TalentFileObj(URaiderzXmlUtilities::TalentXmlFilePath);
	FXmlNode* TalentRootNode = TalentFileObj.GetRootNode();
	TArray<FXmlNode*> TalentNodes = GetNPCTalents(TalentRootNode, NPCID);
	if (TalentNodes.Num() == 0)
	{
		return;
	}

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

	FXmlFile TalentHitInfoFileObj(URaiderzXmlUtilities::TalentHitInfoXmlFilePath);
	FXmlNode* RootTalentHitInfoNode = TalentHitInfoFileObj.GetRootNode();
	TArray<FXmlNode*> TalentHitNodes = URaiderzXmlUtilities::GetNodesWithTag(RootTalentHitInfoNode, TEXT("TALENT_HIT"));
	if (TalentHitNodes.Num() == 0)
	{
		return;
	}

	TArray<FAssetData> MeshAnimAssets = UEditorFunctionLibrary::GetAllAnimationsForSkeletalMesh(Mesh);
	TArray<FCollisionInfo> CollisionInfoArray = GenerateCollisionInfoArray(NPCNode, TalentNodes, AddAnimationNodes, TalentHitNodes, MeshAnimAssets);

}

TArray<FCollisionInfo> UCollisionImporter::GenerateCollisionInfoArray(
	FXmlNode* NPCNode,
	const TArray<FXmlNode*>& TalentNodes,
	const TArray<FXmlNode*>& AddAnimNodes,
	const TArray<FXmlNode*>& TalentHitNodes,
	const TArray<FAssetData>& MeshAnimAssets)
{
	if (TalentNodes.Num() == 0)
	{
		return TArray<FCollisionInfo>();
	}

	TArray<FCollisionInfo> CollisionInfoArray;
	for (FXmlNode* TalentNode : TalentNodes)
	{
		if (!TalentNode)
		{
			continue;
		}

		FString AnimationFileName;
		bool bFoundFileName = GetAnimationFileName(AddAnimNodes, TalentNode, AnimationFileName);
		if (!bFoundFileName)
		{
			continue;
		}

		FCollisionInfo CollisionInfo = GetCollisionInfo(NPCNode, TalentNode, AddAnimNodes, TalentHitNodes, AnimationFileName, MeshAnimAssets);
		CollisionInfoArray.Add(CollisionInfo);
	}
	return CollisionInfoArray;
}

FCollisionInfo UCollisionImporter::GetCollisionInfo(
	FXmlNode* NPCNode,
	FXmlNode* TalentNode,
	const TArray<FXmlNode*>& AddAnimNodes,
	const TArray<FXmlNode*>& TalentHitNodes,
	const FString& AnimationFileName,
	const TArray<FAssetData>& MeshAnimAssets)
{
	check(TalentNode);
	const FString& AnimationName = TalentNode->GetAttribute(TEXT("UseAni"));
	const FString& TalentID = TalentNode->GetAttribute(TEXT("id"));
	
	FCollisionInfo CollisionInfo;
	FString EditorAnimFileName = TEXT("A_") + URaiderzXmlUtilities::GetRaiderzBaseFileName(AnimationFileName);
	for (const FAssetData& AssetData : MeshAnimAssets)
	{
		if (AssetData.AssetName.ToString() != EditorAnimFileName)
		{
			continue;
		}

		FString LogMessage = FString("Found animation file: ") + EditorAnimFileName;
		PrintLog(LogMessage);

		CollisionInfo.TalentID = TalentID;
		CollisionInfo.AnimationName = AnimationName;
		CollisionInfo.AnimationFileName = AnimationFileName;
		CollisionInfo.AnimationAssetData = AssetData;
		CollisionInfo.FrameToCollisionStringMap = GetFrameToCollisionStringMap(TalentHitNodes, TalentNode, NPCNode);
		break;
	}

	return CollisionInfo;
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

bool UCollisionImporter::GetAnimationFileName(const TArray<FXmlNode*>& AddAnimNodes, FXmlNode* TalentNode, FString& OutFileName)
{
	check(TalentNode);

	const FString& AnimationName = TalentNode->GetAttribute(TEXT("UseAni"));
	if (AnimationName == TEXT(""))
	{
		return false;
	}

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
