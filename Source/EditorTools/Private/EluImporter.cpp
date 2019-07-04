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

	/*
	int32 NodeNum = EluMeshNodes.Num();
	for (int i = 0; i < NodeNum; i++)
	{
		TSharedPtr<FEluMeshNode> MeshNode = EluMeshNodes[i];
		if (MeshNode.IsValid())
		{
			FString LogMessage = TEXT("Node name: ") + MeshNode->NodeName + TEXT(", LOD index:  ") + FString::FromInt(MeshNode->LODProjectIndex);
			PrintWarning(LogMessage);
		}
	}
	*/

	FString PackageName = FString("/Game/RaiderZ/Zunk/WAKA");
	bool bPackageExists = FPackageName::DoesPackageExist(PackageName);

	// If package doesn't exist, it's safe to create new package
	PackageName = PackageTools::SanitizePackageName(PackageName);
	UPackage* Package = CreatePackage(nullptr, *PackageName);
	Package->FullyLoad();

	if (!bPackageExists && EluMeshNodes.Num() != 0)
	{
		UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Package, UStaticMesh::StaticClass(), *FString("WAKA"), EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
		StaticMesh->AddSourceModel();
		StaticMesh->LightingGuid = FGuid::NewGuid();
		StaticMesh->LightMapCoordinateIndex = 1;
		StaticMesh->LightMapResolution = 64;

		FRawMesh RawMesh;
		int32 PointsOffset = 0;

		int32 NodeNum = EluMeshNodes.Num();
		for (int i = 0; i < NodeNum; i++)
		{
			TSharedPtr<FEluMeshNode> MeshNode = EluMeshNodes[i];
			check(MeshNode.IsValid());

			if (MeshNode->LODProjectIndex != 0 || MeshNode->NodeName.Contains("hide") || MeshNode->PointsTable.Num() == 0)
			{
				continue;
			}

			// FString LogMessage = TEXT("Processing node: ") + MeshNode->NodeName;
			// PrintWarning(LogMessage);
			// LogMessage = TEXT("Local matrix: \n") + MeshNode->LocalMatrix.ToString();
			// PrintWarning(LogMessage);

			PointsOffset = RawMesh.VertexPositions.Num();
			// RawMesh.VertexPositions += MeshNode->PointsTable;

			for (const FVector& Point : MeshNode->PointsTable)
			{
				const FVector& TransformedPoint = MeshNode->LocalMatrix.TransformPosition(Point);
				RawMesh.VertexPositions.Add(TransformedPoint);
			}

			int32 PolyNum = MeshNode->PolygonTable.Num();
			for (int i = PolyNum - 1; i >= 0; i--)
			{
				const FMeshPolygonData& PolyData = MeshNode->PolygonTable[i];

				RawMesh.FaceMaterialIndices.Add(PolyData.MaterialID);
				RawMesh.FaceSmoothingMasks.Add(1);

				int32 SubNum = PolyData.FaceSubDatas.Num();
				for (int j = SubNum - 1; j >= 0; j--)
				{
					const FFaceSubData& FaceData = PolyData.FaceSubDatas[j];

					RawMesh.WedgeIndices.Add(PointsOffset + FaceData.p);

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

					if (MeshNode->NormalsTable.Num() > FaceData.n)
					{
						const FVector& Normal = MeshNode->NormalsTable[FaceData.n];
						RawMesh.WedgeTangentZ.Add(-Normal);
					}
					else
					{
						RawMesh.WedgeTangentZ.Add(FVector::ZeroVector);
					}

					RawMesh.WedgeColors.Add(FColor(0, 0, 0));

					if (MeshNode->TexCoordTable.Num() > FaceData.uv)
					{
						FVector TexCoord = MeshNode->TexCoordTable[FaceData.uv];
						RawMesh.WedgeTexCoords[0].Add(FVector2D(TexCoord.X, TexCoord.Y));
						RawMesh.WedgeTexCoords[1].Add(FVector2D(0, 0));
					}
					else
					{
						RawMesh.WedgeTexCoords[0].Add(FVector2D(0, 0));
						RawMesh.WedgeTexCoords[1].Add(FVector2D(0, 0));
					}


					// if (MeshNode->PointColorTable.Num() > FaceData.)

				}
			}
		}

		StaticMesh->SourceModels[0].SaveRawMesh(RawMesh);

		TArray<FText> ErrorText;
		StaticMesh->Build(false, &ErrorText);
		StaticMesh->MarkPackageDirty();
		FAssetRegistryModule::AssetCreated(StaticMesh);
	}


	/*
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
