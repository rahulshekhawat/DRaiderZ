// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "CollisionImporter.h"
#include "EditorFunctionLibrary.h"

#include "Engine/SkeletalMesh.h"

const FString UCollisionImporter::NPCFilePath(TEXT("F:/Zunk/Zunk_Tests/datadump/Data/system/npc.xml"));
// FXmlParser can't parse talent.xml, so it has been replaced with talent_unrealengine.xml which removed a few TALENT IDs that FXmlParser was unable to parse
// const FString UCollisionImporter::TalentFilePath(TEXT("F:/Zunk/Zunk_Tests/datadump/Data/system/talent.xml"));
const FString UCollisionImporter::TalentFilePath(TEXT("F:/Game Dev/asset_src/XMLFiles/talent_unrealengine.xml"));

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

	FXmlFile NPCFileObj(NPCFilePath);
	FXmlNode* NPCRootNode = NPCFileObj.GetRootNode();

	FString NPCID;
	bool bFound =  GetNPCID(NPCRootNode, MeshName, NPCID);
	if (!bFound)
	{
		return;
	}

	//~ @todo XmlParser can't parse talent.xml file
	FXmlFile TalentFileObj(TalentFilePath);
	FXmlNode* TalentRootNode = TalentFileObj.GetRootNode();

	TArray<FXmlNode*> TalentNodes = GetNPCTalents(TalentRootNode, NPCID);
	if (TalentNodes.Num() == 0)
	{
		return;
	}

	for (FXmlNode* Node : TalentNodes)
	{
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
