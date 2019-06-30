// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "EluImporter.h"
#include "EOD.h"
#include "EluLibrary.h"
#include "EluMeshNodeLoader.h"
#include "RaiderzXmlUtilities.h"

#include "Editor.h"
#include "RawMesh.h"
#include "Engine/StaticMesh.h"
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

	FString PackageName = FString("/Game/RaiderZ/Zunk/TestPackage");
	bool bPackageExists = FPackageName::DoesPackageExist(PackageName);

	if (!bPackageExists && EluMeshNodes.Num() != 0)
	{



	}

	// Unselect all actors.
	// GEditor->SelectNone(false, false);
	// GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPreImport(this, Class, InParent, Name, Type);

	// FbxImporter->ImportFromFile(FbxImportFileName, Type, true)
	// ImportAllSkeletalMesh(RootNodeToImport, FbxImporter, Flags, NodeIndex, InterestingNodeCount, SceneInfoPtr);
	// ImportAllStaticMesh(RootNodeToImport, FbxImporter, Flags, NodeIndex, InterestingNodeCount, SceneInfoPtr);

	


	//~ @todo fix static mesh import for 4.22 (this only works in 4.21, not 4.22)
	/*
	if (!bPackageExists && EluMeshNodes.Num() != 0)
	{
		// If package doesn't exist, it's safe to create new package
		PackageName = PackageTools::SanitizePackageName(PackageName);
		UPackage* Package = CreatePackage(nullptr, *PackageName);
		Package->FullyLoad();

		UStaticMesh* NewObj = NewObject<UStaticMesh>(Package, UStaticMesh::StaticClass(), *FString("TestPackage"), EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
		NewObj->Modify();
		// WTF IS GOING ON IN THE FOLLOWING LINE?
		// UPDATE: most likely an override of new operator to allow directly adding FStaticMeshSourceModel() to the array;
		new(NewObj->SourceModels) FStaticMeshSourceModel();

		int32 PointsOffset = 0;
		FRawMesh RawMesh;

		for (int i = 0; i < EluMeshNodes.Num(); ++i)
		{
			TSharedPtr<FEluMeshNode> EluMeshNode = EluMeshNodes[i];
			if (EluMeshNode->PointsTable.Num() == 0)
			{
				continue;
			}

			PointsOffset = RawMesh.VertexPositions.Num();
			RawMesh.VertexPositions += EluMeshNode->PointsTable;

			for (int j = 0; j < EluMeshNode->VertexIndexTable.Num(); ++j)
			{
				RawMesh.FaceSmoothingMasks.Add(1);
				RawMesh.FaceMaterialIndices.Add(EluMeshNode->MaterialID);

				RawMesh.WedgeIndices.Add(PointsOffset + EluMeshNode->VertexIndexTable[j].p);

				RawMesh.WedgeTangentX.Add(FVector(0, 0, 0));
				RawMesh.WedgeTangentY.Add(FVector(0, 0, 0));

				if (EluMeshNode->NormalsTable.Num() > 0)
				{
					FVector Normal = EluMeshNode->NormalsTable[EluMeshNode->VertexIndexTable[j].n];
					RawMesh.WedgeTangentZ.Add(Normal);
				}

				if (EluMeshNode->TexCoordTable.Num() > 0)
				{
					FVector TexCoord = EluMeshNode->TexCoordTable[EluMeshNode->VertexIndexTable[j].uv];
					RawMesh.WedgeTexCoords[0].Add(FVector2D(TexCoord.X, TexCoord.Y));
					RawMesh.WedgeTexCoords[1].Add(FVector2D(0, 0));
				}


				RawMesh.WedgeColors.Add(FColor(0, 0, 0));

			}
		}

		NewObj->SourceModels[0].SaveRawMesh(RawMesh);

		TArray<FText> ErrorText;
		NewObj->Build(false, &ErrorText);
		NewObj->MarkPackageDirty();
		PrintWarning(TEXT("Finished importing elu mesh"));

		// FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	}
	*/
	return true;
}
