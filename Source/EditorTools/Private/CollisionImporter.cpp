// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "CollisionImporter.h"
#include "RaiderzXmlUtilities.h"
#include "EditorFunctionLibrary.h"

#include "Engine/SkeletalMesh.h"

UCollisionImporter::UCollisionImporter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCollisionImporter::ImportCollisionForSkeletalMesh(USkeletalMesh* Mesh)
{
	if (Mesh == nullptr)
	{
		return;
	}

	FString FullMeshName = Mesh->GetFName().ToString();
	FString MeshName = FullMeshName.RightChop(3);

	FXmlFile NPCFileObj(URaiderzXmlUtilities::NPCXmlFilePath);
	FXmlNode* NPCRootNode = NPCFileObj.GetRootNode();

	FString NPCID;
	bool bFound =  GetNPCID(NPCRootNode, MeshName, NPCID);
	if (!bFound)
	{
		return;
	}

	//~ @todo XmlParser can't parse talent.xml file
	FXmlFile TalentFileObj(URaiderzXmlUtilities::TalentXmlFilePath);
	FXmlNode* TalentRootNode = TalentFileObj.GetRootNode();

	TArray<FXmlNode*> TalentNodes = GetNPCTalents(TalentRootNode, NPCID);
	if (TalentNodes.Num() == 0)
	{
		return;
	}







	TArray<FCollisionInfo> CollsionInfoArray;
	for (FXmlNode* Node : TalentNodes)
	{
		if (!Node)
		{
			continue;
		}

		FCollisionInfo CollisionInfo;
		CollisionInfo.AnimationName = Node->GetAttribute(TEXT("UseAni"));




		const FString& TalentID = Node ? Node->GetAttribute(TEXT("id")) : TEXT("");
		if (TalentID == TEXT(""))
		{
			continue;
		}

		UE_LOG(LogTemp, Log, TEXT("%s"), *TalentID);
	}
}

bool UCollisionImporter::GetNPCID(FXmlNode* NPCRootNode, const FString& MeshName, FString& OutNPCID)
{
	TArray<FXmlNode*> NPCNodes = UEditorFunctionLibrary::GetNodesWithTag(NPCRootNode, TEXT("NPC"));
	for (FXmlNode* Node : NPCNodes)
	{
		if (Node->GetAttribute(TEXT("MeshName")) == MeshName)
		{
			OutNPCID = Node->GetAttribute(TEXT("id"));
			return true;
		}
	}
	return false;
}

TArray<FXmlNode*> UCollisionImporter::GetNPCTalents(FXmlNode* TalentRootNode, const FString& InNPCID)
{
	TArray<FXmlNode*> AllTalentNodes = UEditorFunctionLibrary::GetNodesWithTag(TalentRootNode, TEXT("TALENT"));
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
