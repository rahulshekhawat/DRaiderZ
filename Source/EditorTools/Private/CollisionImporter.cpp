// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "CollisionImporter.h"
#include "EditorFunctionLibrary.h"

#include "Engine/SkeletalMesh.h"

const FString UCollisionImporter::NPCFilePath(TEXT("F:/Zunk/Zunk_Tests/datadump/Data/system/npc.xml"));
const FString UCollisionImporter::TalentFilePath(TEXT("F:/Zunk/Zunk_Tests/datadump/Data/system/talent.xml"));

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

	UE_LOG(LogTemp, Warning, TEXT("%s"), *NPCID);

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
