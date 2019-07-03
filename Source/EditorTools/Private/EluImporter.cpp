// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "EluImporter.h"
#include "EOD.h"
#include "EluLibrary.h"
#include "EluMeshNodeLoader.h"
#include "RaiderzXmlUtilities.h"

#include "MeshUtilities.h"
#include "Editor.h"
#include "RawMesh.h"
#include "AssetRegistryModule.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/Package.h"
#include "PackageTools.h"
#include "Misc/PackageName.h"
#include "Misc/FileHelper.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"


UEluImporter::UEluImporter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEluImporter::ImportEluFile()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	bool bSuccess = false;
	TArray<FString> SelectedFiles;
	if (DesktopPlatform)
	{
		const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
		bSuccess = DesktopPlatform->OpenFileDialog(
			ParentWindowWindowHandle,
			TEXT("Select FBX file to import"),
			URaiderzXmlUtilities::DataFolderPath,
			TEXT(""),
			TEXT("RaiderZ files|*.elu"),
			EFileDialogFlags::None,
			SelectedFiles
		);
	}

	if (!bSuccess || SelectedFiles.Num() == 0)
	{
		PrintWarning(TEXT("User failed to select any files!"));
	}

	const FString& EluFilePath = SelectedFiles[0];
	bool bImportSuccessful = ImportEluFile_Internal(EluFilePath);
}

bool UEluImporter::ImportEluFile_Internal(const FString& EluFilePath)
{
	TArray<uint8> BinaryData;
	bool bSuccess = FFileHelper::LoadFileToArray(BinaryData, *EluFilePath);
	if (!bSuccess)
	{
		return false;
	}

	UINT Offset = 0;
	FEluHeader EluHeader;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&EluHeader, sizeof(EluHeader), BinaryData, Offset);

	if (EluHeader.Signature != EXPORTER_SIG)
	{
		FString LogMessage = TEXT("Failed to verify elu signatures for file: ") + FPaths::GetCleanFilename(EluFilePath); +TEXT(". File signature: ") +
			FString::FromInt(EluHeader.Signature) + TEXT(", expected signature: ") + FString::FromInt(EXPORTER_SIG);
		PrintError(LogMessage);
		return false;
	}

	if (EluHeader.Version != EXPORTER_CURRENT_MESH_VER)
	{
		FString LogMessage = TEXT("File '") + FPaths::GetCleanFilename(EluFilePath) + TEXT("' is not the latest file version.");
		PrintWarning(LogMessage);
	}

	FEluMeshNodeLoader_v12 EluMeshNodeLoaderObj_v12;
	FEluMeshNodeLoader_v13 EluMeshNodeLoaderObj_v13;
	FEluMeshNodeLoader_v14 EluMeshNodeLoaderObj_v14;
	FEluMeshNodeLoader_v15 EluMeshNodeLoaderObj_v15;
	FEluMeshNodeLoader_v16 EluMeshNodeLoaderObj_v16;
	FEluMeshNodeLoader_v17 EluMeshNodeLoaderObj_v17;
	FEluMeshNodeLoader_v18 EluMeshNodeLoaderObj_v18;
	FEluMeshNodeLoader_v20 EluMeshNodeLoaderObj_v20;

	FEluMeshNodeLoader* EluMeshNodeLoader = nullptr;
	if (EluHeader.Version == EXPORTER_MESH_VER20)
	{
		EluMeshNodeLoader = &EluMeshNodeLoaderObj_v20;
		EluMeshNodeLoader->CurrentEluVersion = EXPORTER_MESH_VER20;
	}
	else if (EluHeader.Version == EXPORTER_MESH_VER18)
	{
		EluMeshNodeLoader = &EluMeshNodeLoaderObj_v18;
		EluMeshNodeLoader->CurrentEluVersion = EXPORTER_MESH_VER18;
	}
	else if (EluHeader.Version == EXPORTER_MESH_VER17)
	{
		EluMeshNodeLoader = &EluMeshNodeLoaderObj_v17;
		EluMeshNodeLoader->CurrentEluVersion = EXPORTER_MESH_VER17;
	}
	else if (EluHeader.Version == EXPORTER_MESH_VER16)
	{
		EluMeshNodeLoader = &EluMeshNodeLoaderObj_v16;
		EluMeshNodeLoader->CurrentEluVersion = EXPORTER_MESH_VER16;
	}
	else if (EluHeader.Version == EXPORTER_MESH_VER15)
	{
		EluMeshNodeLoader = &EluMeshNodeLoaderObj_v15;
		EluMeshNodeLoader->CurrentEluVersion = EXPORTER_MESH_VER15;
	}
	else if (EluHeader.Version == EXPORTER_MESH_VER14)
	{
		EluMeshNodeLoader = &EluMeshNodeLoaderObj_v14;
		EluMeshNodeLoader->CurrentEluVersion = EXPORTER_MESH_VER14;
	}
	else if (EluHeader.Version == EXPORTER_MESH_VER13)
	{
		EluMeshNodeLoader = &EluMeshNodeLoaderObj_v13;
		EluMeshNodeLoader->CurrentEluVersion = EXPORTER_MESH_VER13;
	}
	else if (EluHeader.Version <= EXPORTER_MESH_VER12)
	{
		EluMeshNodeLoader = &EluMeshNodeLoaderObj_v12;
		EluMeshNodeLoader->CurrentEluVersion = EXPORTER_MESH_VER12;
	}
	else
	{
		FString LogMessage = TEXT("elu version of file '") + FPaths::GetCleanFilename(EluFilePath) + TEXT("' is not supported");
		PrintError(LogMessage);
		return false;
	}

	TArray<TSharedPtr<FEluMeshNode>> EluMeshNodes;
	for (int i = 0; i < EluHeader.MeshNum; i++)
	{
		TSharedPtr<FEluMeshNode> EluMeshNode(new FEluMeshNode());
		bool bLoadSuccessful = EluMeshNodeLoader->Load(EluMeshNode, BinaryData, Offset);

		if (bLoadSuccessful)
		{
			EluMeshNodes.Add(EluMeshNode);
		}
	}

	FString PackageName = FString("/Game/RaiderZ/Zunk/WAKA");
	bool bPackageExists = FPackageName::DoesPackageExist(PackageName);


	// If package doesn't exist, it's safe to create new package
	PackageName = PackageTools::SanitizePackageName(PackageName);
	UPackage* Package = CreatePackage(nullptr, *PackageName);
	Package->FullyLoad();

	UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Package, UStaticMesh::StaticClass(), *FString("WAKA"), EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
	StaticMesh->AddSourceModel();
	StaticMesh->LightingGuid = FGuid::NewGuid();
	StaticMesh->LightMapCoordinateIndex = 1;
	StaticMesh->LightMapResolution = 64;


	// const FStaticMeshSourceModel& SrcModel = StaticMesh->SourceModels[0];
	// SrcModel.MeshDescription.Get().

	FRawMesh RawMesh;
	// int32 NodeNums = EluMeshNodes.Num();
	TSharedPtr<FEluMeshNode> MeshNode = EluMeshNodes[0];
	// PrintWarning(MeshNode->ToString());

	RawMesh.VertexPositions.Empty();
	RawMesh.VertexPositions += MeshNode->PointsTable;

	for (const FMeshPolygonData& PolyData : MeshNode->PolygonTable)
	{
		RawMesh.FaceMaterialIndices.Add(PolyData.MaterialID);
		RawMesh.FaceSmoothingMasks.Add(1);

		for (const FFaceSubData& FaceData : PolyData.FaceSubDatas)
		{
			RawMesh.WedgeIndices.Add(FaceData.p);

			if (MeshNode->TangentTanTable.Num() > FaceData.n_tan)
			{
				const FVector& Tangent_X = MeshNode->TangentTanTable[FaceData.n_tan];
				RawMesh.WedgeTangentX.Add(Tangent_X);
			}
			else
			{
				RawMesh.WedgeTangentX.Add(FVector::ZeroVector);
			}


			if (MeshNode->TangentBinTable.Num() > FaceData.n_bin)
			{
				const FVector& Tangent_Y = MeshNode->TangentBinTable[FaceData.n_bin];
				RawMesh.WedgeTangentY.Add(Tangent_Y);
			}
			else
			{
				RawMesh.WedgeTangentY.Add(FVector::ZeroVector);
			}

			const FVector& Normal = MeshNode->NormalsTable[FaceData.p];
			RawMesh.WedgeTangentZ.Add(Normal);

			RawMesh.WedgeColors.Add(FColor(0, 0, 0));

			/*
			if (MeshNode->PointColorTable.Num() > FaceData.)
			{

			}
			else
			{

			}
			*/
			

			FVector TexCoord = MeshNode->TexCoordTable[FaceData.uv];
			RawMesh.WedgeTexCoords[0].Add(FVector2D(TexCoord.X, TexCoord.Y));
			RawMesh.WedgeTexCoords[1].Add(FVector2D(0, 0));

		}
	}

	if (RawMesh.IsValid())
	{
		PrintWarning("MESH IS VALID");
	}
	else if (RawMesh.IsValidOrFixable())
	{
		PrintWarning("MESH IS VALID OR FIXABLE");
	}
	else
	{
		PrintError("MESH INVLAID");
	}

	IMeshUtilities& MeshUtilities = FModuleManager::Get().LoadModuleChecked<IMeshUtilities>("MeshUtilities");
	TArray<FVector2D> OutUniqueUVs;
	bool bResult = MeshUtilities.GenerateUniqueUVsForStaticMesh(RawMesh, 1024, OutUniqueUVs);
	if (bResult)
	{
		PrintWarning("Unique UV generation succeeded!");
	}
	else
	{
		PrintError("Unique UV generation failed!");
	}

	RawMesh.WedgeTexCoords[0] = OutUniqueUVs;

	// StaticMesh->SourceModels[0].RawMeshBulkData->SaveRawMesh(RawMesh);

	StaticMesh->SourceModels[0].SaveRawMesh(RawMesh);

	TArray<FText> ErrorText;
	StaticMesh->Build(false, &ErrorText);
	StaticMesh->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(StaticMesh);



	//~ temporarily disabled
	/*
	if (!bPackageExists && EluMeshNodes.Num() != 0)
	{
		// If package doesn't exist, it's safe to create new package
		PackageName = PackageTools::SanitizePackageName(PackageName);
		UPackage* Package = CreatePackage(nullptr, *PackageName);
		Package->FullyLoad();

		UStaticMesh* NewObj = NewObject<UStaticMesh>(Package, UStaticMesh::StaticClass(), *FString("WAKA"), EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
		NewObj->AddSourceModel();
		// new(NewObj->SourceModels) FStaticMeshSourceModel();

		int32 PointsOffset = 0;
		FRawMesh RawMesh;
		// RawMesh.VertexPositions = TArray<FVector>();

		for (int i = 0; i < EluMeshNodes.Num(); ++i)
		{
			TSharedPtr<FEluMeshNode>EluMeshNode = EluMeshNodes[i];
			// FEluMeshNode* EluMeshNode = EluMeshNodes[i];
			if (EluMeshNode->PointsTable.Num() == 0)
			{
				continue;
			}

			PointsOffset = RawMesh.VertexPositions.Num();
			RawMesh.VertexPositions += EluMeshNode->PointsTable;

			for (int i = 0; i < EluMeshNode->VertexIndexTable.Num(); ++i)
			{
				RawMesh.FaceSmoothingMasks.Add(1);
				RawMesh.FaceMaterialIndices.Add(EluMeshNode->MaterialID);

				RawMesh.WedgeIndices.Add(PointsOffset + EluMeshNode->VertexIndexTable[i].p);

				RawMesh.WedgeTangentX.Add(FVector(0, 0, 0));
				RawMesh.WedgeTangentY.Add(FVector(0, 0, 0));

				if (EluMeshNode->NormalsTable.Num() > 0)
				{
					FVector Normal = EluMeshNode->NormalsTable[EluMeshNode->VertexIndexTable[i].n];
					RawMesh.WedgeTangentZ.Add(Normal);
				}

				if (EluMeshNode->TexCoordTable.Num() > 0)
				{
					FVector TexCoord = EluMeshNode->TexCoordTable[EluMeshNode->VertexIndexTable[i].uv];
					RawMesh.WedgeTexCoords[0].Add(FVector2D(TexCoord.X, TexCoord.Y));
					RawMesh.WedgeTexCoords[1].Add(FVector2D(0, 0));
				}


				RawMesh.WedgeColors.Add(FColor(0, 0, 0));

			}
		}

		NewObj->SourceModels[0].RawMeshBulkData->SaveRawMesh(RawMesh);

		TArray<FText> ErrorText;
		NewObj->Build(false, &ErrorText);
		NewObj->MarkPackageDirty();
		FAssetRegistryModule::AssetCreated(NewObj);

		UE_LOG(LogTemp, Warning, TEXT("BAAAL"));



	}
	*/


	//~ @todo check UnFbx::FFbxImporter::BuildStaticMeshFromGeometry
	
	// Unselect all actors.
	// GEditor->SelectNone(false, false);
	// GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPreImport(this, Class, InParent, Name, Type);

	// FbxImporter->ImportFromFile(FbxImportFileName, Type, true)
	// ImportAllSkeletalMesh(RootNodeToImport, FbxImporter, Flags, NodeIndex, InterestingNodeCount, SceneInfoPtr);
	// ImportAllStaticMesh(RootNodeToImport, FbxImporter, Flags, NodeIndex, InterestingNodeCount, SceneInfoPtr);

	
	return true;
}
