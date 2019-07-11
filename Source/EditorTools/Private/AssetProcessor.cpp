// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "AssetProcessor.h"
#include "EOD.h"

#include "RawMesh.h"
#include "MeshUtilities.h"
#include "AssetRegistryModule.h"
#include "Engine/StaticMesh.h"

UAssetProcessor::UAssetProcessor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UAssetProcessor::GenerateUniqueUVForStaticMesh(UStaticMesh* StaticMesh)
{
	if (!StaticMesh)
	{
		return false;
	}

	StaticMesh->Modify();
	IMeshUtilities& MeshUtilities = FModuleManager::Get().LoadModuleChecked<IMeshUtilities>("MeshUtilities");
	TArray<FVector2D> OutUniqueUVs;

	FRawMesh RawMesh;
	StaticMesh->SourceModels[0].LoadRawMesh(RawMesh);
	bool bResult = MeshUtilities.GenerateUniqueUVsForStaticMesh(RawMesh, 2048, OutUniqueUVs);
	if (bResult)
	{
		FString Message = TEXT("Unique UV generation succeeded for ") + StaticMesh->GetName();
		PrintLog(Message);
	}
	else
	{
		FString Message = TEXT("Unique UV generation failed for ") + StaticMesh->GetName();
		PrintLog(Message);
	}

	// RawMesh.WedgeTexCoords[0] = OutUniqueUVs;
	RawMesh.WedgeTexCoords[1] = OutUniqueUVs;		// lightmap uvs

	StaticMesh->SourceModels[0].SaveRawMesh(RawMesh);

	TArray<FText> ErrorText;
	StaticMesh->Build(false, &ErrorText);

	FAssetRegistryModule::AssetCreated(StaticMesh);
	return bResult;
}
