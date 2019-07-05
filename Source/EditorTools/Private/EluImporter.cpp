// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "EluImporter.h"
#include "EOD.h"
#include "RaiderzXmlUtilities.h"

#include "ReferenceSkeleton.h"
#include "RenderCommandFence.h"
#include "PackageTools.h"
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
#include "Animation/Skeleton.h"
#include "Rendering/SkeletalMeshModel.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"


UEluImporter::UEluImporter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEluImporter::ImportEluStaticMesh()
{
	FString EluFile;
	bool bSuccess = PickEluFile(EluFile);
	if (!bSuccess)
	{
		return;
	}

	bool bImportSuccess = ImportEluStaticMesh_Internal(EluFile);
}

void UEluImporter::ImportEluSkeletalMesh()
{
	FString EluFile;
	bool bSuccess = PickEluFile(EluFile);
	if (!bSuccess)
	{
		return;
	}

	bool bImportSuccess = ImportEluSkeletalMesh_Internal(EluFile);
}

bool UEluImporter::PickEluFile(FString& OutFilePath)
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
		PrintWarning(TEXT("User failed to select any elu file!"));
		return false;
	}

	OutFilePath = SelectedFiles[0];
	return true;
}

FEluFileData UEluImporter::LoadEluData(const FString& EluFilePath)
{
	FEluFileData EluData;

	TArray<uint8> BinaryData;
	bool bSuccess = FFileHelper::LoadFileToArray(BinaryData, *EluFilePath);
	if (!bSuccess)
	{
		EluData.bLoadSuccess = false;
		return EluData;
	}

	UINT Offset = 0;
	FEluHeader EluHeader;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&EluHeader, sizeof(EluHeader), BinaryData, Offset);

	if (EluHeader.Signature != EXPORTER_SIG)
	{
		FString LogMessage = TEXT("Failed to verify elu signatures for file: ") + FPaths::GetCleanFilename(EluFilePath); +TEXT(". File signature: ") +
			FString::FromInt(EluHeader.Signature) + TEXT(", expected signature: ") + FString::FromInt(EXPORTER_SIG);
		PrintError(LogMessage);

		EluData.bLoadSuccess = false;
		return EluData;
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

		EluData.bLoadSuccess = false;
		return EluData;
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

	EluData.bLoadSuccess = true;
	EluData.EluHeader = EluHeader;
	EluData.EluMeshNodes = EluMeshNodes;

	return EluData;
}

bool UEluImporter::ImportEluStaticMesh_Internal(const FString& EluFilePath)
{
	FEluFileData EluData = LoadEluData(EluFilePath);
	if (!EluData.bLoadSuccess)
	{
		return false;
	}

	const TArray<TSharedPtr<FEluMeshNode>>& EluMeshNodes = EluData.EluMeshNodes;

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

			FString LogMessage = TEXT("Processing node: ") + MeshNode->NodeName;
			PrintWarning(LogMessage);
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
			for (int j = PolyNum - 1; j >= 0; j--)
			{
				const FMeshPolygonData& PolyData = MeshNode->PolygonTable[j];

				RawMesh.FaceMaterialIndices.Add(PolyData.MaterialID);
				RawMesh.FaceSmoothingMasks.Add(1);

				int32 SubNum = PolyData.FaceSubDatas.Num();
				for (int k = SubNum - 1; k >= 0; k--)
				{
					const FFaceSubData& FaceData = PolyData.FaceSubDatas[k];

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
						RawMesh.WedgeTangentZ.Add(Normal);
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

bool UEluImporter::ImportEluSkeletalMesh_Internal(const FString& EluFilePath)
{
	FEluFileData EluData = LoadEluData(EluFilePath);
	if (!EluData.bLoadSuccess)
	{
		return false;
	}

	const TArray<TSharedPtr<FEluMeshNode>>& EluMeshNodes = EluData.EluMeshNodes;
	FString SkelPackName = FString("/Game/RaiderZ/Zunk/Skel");
	SkelPackName = PackageTools::SanitizePackageName(SkelPackName);
	UPackage* SkelPack = CreatePackage(nullptr, *SkelPackName);
	SkelPack->FullyLoad();

	USkeleton* Skel = NewObject<USkeleton>(SkelPack, *FString("Skel"), EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
	const FReferenceSkeleton& RefSkel = Skel->GetReferenceSkeleton();
	// horrible hack to modify the skeleton in place
	FReferenceSkeletonModifier SkelMod((FReferenceSkeleton&)RefSkel, Skel);

	SkelMod.Add(FMeshBoneInfo(TEXT("root_bone"), TEXT("root_bone"), INDEX_NONE), FTransform());

	for (int i = 0; i < EluMeshNodes.Num(); i++)
	{
		TSharedPtr<FEluMeshNode> MeshNode = EluMeshNodes[i];
		FTransform Transform(MeshNode->LocalMatrix);
		FString SanitizedNodeName = UPackageTools::SanitizePackageName(MeshNode->NodeName);
		SkelMod.Add(FMeshBoneInfo(FName(*SanitizedNodeName), SanitizedNodeName, MeshNode->ParentNodeID + 1), Transform);
	}


	Skel->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(Skel);

	
	
	/*
	for (int i = 0; i < EluMeshNodes.Num(); i++)
	{
		TSharedPtr<FEluMeshNode> MeshNode = EluMeshNodes[i];
		FString BoneName = PackageTools::SanitizePackageName(MeshNode->NodeName);
		int32 BoneIndex = Skel->GetReferenceSkeleton().FindBoneIndex(FName(*BoneName));
		int32 RawBoneIndex = Skel->GetReferenceSkeleton().FindRawBoneIndex(FName(*BoneName));

		FString LogMessage = TEXT("Node name: ") + BoneName + TEXT(", Bone index: ") + FString::FromInt(BoneIndex) + TEXT(", Raw bone index: ") + FString::FromInt(RawBoneIndex);
		PrintWarning(LogMessage);
	}
	*/

	FString PackageName = FString("/Game/RaiderZ/Zunk/SkelMesh");
	bool bPackageExists = FPackageName::DoesPackageExist(PackageName);

	// If package doesn't exist, it's safe to create new package
	PackageName = PackageTools::SanitizePackageName(PackageName);
	UPackage* Package = CreatePackage(nullptr, *PackageName);
	Package->FullyLoad();

	USkeletalMesh* SkeletalMesh = NewObject<USkeletalMesh>(Package, USkeletalMesh::StaticClass(), *FString("WAKA"), EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
	check(SkeletalMesh);

	SkeletalMesh->ReleaseResources();
	SkeletalMesh->ReleaseResourcesFence.Wait();

	SkeletalMesh->Skeleton = Skel;
	SkeletalMesh->RefSkeleton = Skel->GetReferenceSkeleton();
	// SkeletalMesh->RefSkeleton.RebuildNameToIndexMap();
	SkeletalMesh->RefSkeleton.RebuildRefSkeleton(SkeletalMesh->Skeleton, true);

	SkeletalMesh->RefBasesInvMatrix.Empty();
	SkeletalMesh->CalculateInvRefMatrices();

	SkeletalMesh->InitResources();


	//~ @todo set skeleton for skeletal mesh

	FSkeletalMeshModel* ImportedModel = SkeletalMesh->GetImportedModel();
	SkeletalMesh->PreEditChange(nullptr);

	ImportedModel->LODModels.Add(new FSkeletalMeshLODModel());
	SkeletalMesh->AddLODInfo();

	FSkeletalMeshLODModel& LODModel = ImportedModel->LODModels[0];

	SkeletalMesh->GetLODInfo(0)->LODHysteresis = 0.02;

	FSkeletalMeshOptimizationSettings Settings;
	SkeletalMesh->GetLODInfo(0)->ReductionSettings = Settings;


	LODModel.NumTexCoords = 1;

	TArray<FSoftSkinVertex> SoftVertices;
	TArray<FVector> Points;
	TArray<SkeletalMeshImportData::FMeshWedge> Wedges;
	TArray<SkeletalMeshImportData::FMeshFace> Faces;
	TArray<SkeletalMeshImportData::FVertInfluence> Influences;

	TArray<int32> PointsMap;
	TArray<FVector> TangentX;
	TArray<FVector> TangentY;
	TArray<FVector> TangentZ;
	TArray<uint16> MatIndices;
	TArray<uint32> SmoothingGroups;

	// FSoftSkinVertex SkinVertex;

	TSharedPtr<FEluMeshNode> ArekNode;
	for (TSharedPtr<FEluMeshNode> Node : EluMeshNodes)
	{
		if (Node->NodeName == TEXT("arek"))
		{
			ArekNode = Node;
			break;
		}
	}

	check(ArekNode.IsValid());

	for (const FVector& Point : ArekNode->PointsTable)
	{
		const FVector& TransformedPoint = ArekNode->LocalMatrix.TransformPosition(Point);
		int32 VertIndex = Points.Add(TransformedPoint);
		PointsMap.Add(VertIndex);
	}

	int32 PointsOffset = 0;
	int32 PolyNum = ArekNode->PolygonTable.Num();
	for (int j = PolyNum - 1; j >= 0; j--)
	{
		const FMeshPolygonData& PolyData = ArekNode->PolygonTable[j];
		int32 SubNum = PolyData.FaceSubDatas.Num();
		check(SubNum == 3);

		SkeletalMeshImportData::FMeshFace MeshFace;
		MeshFace.MeshMaterialIndex = PolyData.MaterialID;
		MeshFace.SmoothingGroups = 1;

		for (int k = SubNum - 1; k >= 0; k--)
		{
			const FFaceSubData& FaceData = PolyData.FaceSubDatas[k];

			SkeletalMeshImportData::FMeshWedge MeshWedge;
			MeshWedge.iVertex = PointsOffset + FaceData.p;
			MeshWedge.Color = FColor(0, 0, 0);

			// RawMesh.WedgeColors.Add(FColor(0, 0, 0));

			if (ArekNode->TexCoordTable.Num() > FaceData.uv)
			{
				FVector TexCoord = ArekNode->TexCoordTable[FaceData.uv];
				MeshWedge.UVs[0] = FVector2D(TexCoord.X, TexCoord.Y);
				MeshWedge.UVs[1] = FVector2D(0, 0);
				// RawMesh.WedgeTexCoords[0].Add(FVector2D(TexCoord.X, TexCoord.Y));
				// RawMesh.WedgeTexCoords[1].Add(FVector2D(0, 0));
			}
			else
			{
				MeshWedge.UVs[0] = FVector2D(0, 0);
				MeshWedge.UVs[1] = FVector2D(0, 0);
				// RawMesh.WedgeTexCoords[0].Add(FVector2D(0, 0));
				// RawMesh.WedgeTexCoords[1].Add(FVector2D(0, 0));
			}

			int32 WedgeIndex = Wedges.Add(MeshWedge);

			FString LogMessage = TEXT("Wedge Index: ") + FString::FromInt(WedgeIndex);
			LogMessage += TEXT(", Wedges.Num(): ") + FString::FromInt(Wedges.Num());
			PrintWarning(LogMessage);


			MeshFace.iWedge[2 - k] = (uint32)WedgeIndex;
			// RawMesh.WedgeIndices.Add(PointsOffset + FaceData.p);

			if (ArekNode->TangentTanTable.Num() > FaceData.n_tan)
			{
				const FVector& Tangent_X = ArekNode->TangentTanTable[FaceData.n_tan];
				MeshFace.TangentX[2 - k] = Tangent_X;
				// RawMesh.WedgeTangentX.Add(Tangent_X);
			}
			else
			{
				MeshFace.TangentX[2 - k] = FVector::ZeroVector;
				// RawMesh.WedgeTangentX.Add(FVector::ZeroVector);
			}


			if (ArekNode->TangentBinTable.Num() > FaceData.n_bin)
			{
				const FVector& Tangent_Y = ArekNode->TangentBinTable[FaceData.n_bin];
				MeshFace.TangentY[2 - k] = Tangent_Y;
				// RawMesh.WedgeTangentY.Add(Tangent_Y);
			}
			else
			{
				MeshFace.TangentY[2 - k] = FVector::ZeroVector;
				// RawMesh.WedgeTangentY.Add(FVector::ZeroVector);
			}

			if (ArekNode->NormalsTable.Num() > FaceData.n)
			{
				const FVector& Normal = ArekNode->NormalsTable[FaceData.n];
				MeshFace.TangentZ[2 - k] = Normal;
				// RawMesh.WedgeTangentZ.Add(Normal);
			}
			else
			{
				MeshFace.TangentZ[2 - k] = FVector::ZeroVector;
				// RawMesh.WedgeTangentZ.Add(FVector::ZeroVector);
			}
		}
		Faces.Add(MeshFace);

	}

	int32 PhysiqueTableNum = ArekNode->PhysiqueTable.Num();
	for (int i = 0; i < PhysiqueTableNum; i++)
	{
		int32 VertexIndex = (PhysiqueTableNum - 1) - i;

		const FPhysiqueInfo& PhysiqueInfo = ArekNode->PhysiqueTable[i];
		for (const FPhysiqueSubData& PhysiqueSubData : PhysiqueInfo.PhysiqueSubDatas)
		{
			FString BoneName = EluMeshNodes[ArekNode->BoneTableIndex[PhysiqueSubData.cid]]->NodeName;
			FString SanitizedBoneName = PackageTools::SanitizePackageName(BoneName);

			// int32 BoneIndex = Skel->GetReferenceSkeleton().FindBoneIndex(FName(*SanitizedBoneName));
			int32 BoneIndex = Skel->GetReferenceSkeleton().FindRawBoneIndex(FName(*SanitizedBoneName));
			if (BoneIndex < 0)
			{
				FString LogMessage = TEXT("Couldn't find bone index for bone name: ") + SanitizedBoneName;
				PrintError(LogMessage);
			}

			SkeletalMeshImportData::FVertInfluence VertInfluence;
			VertInfluence.VertIndex = PointsOffset + VertexIndex;
			VertInfluence.Weight = PhysiqueSubData.weight;
			VertInfluence.BoneIndex = BoneIndex;

			// Influences.AddUnique(VertInfluence);
			Influences.Add(VertInfluence);
		}
	}

	PrintWarning("Finished bone processing");


	// now create a map from vert indices to faces
	for (int32 FaceIndex = 0; FaceIndex < Faces.Num(); FaceIndex++)
	{
		const SkeletalMeshImportData::FMeshFace& Face = Faces[FaceIndex];
		for (int32 VertexIndex = 0; VertexIndex < 3; VertexIndex++)
		{
			int32 WedgeIndex = Face.iWedge[VertexIndex];
			FString Message = TEXT("Wedge Index: ") + FString::FromInt(WedgeIndex);
			PrintWarning(Message);

			if (WedgeIndex >= Wedges.Num())
			{
				PrintError("YO WEDGE INDEX IS MORE THAN WEDGET NUM");
			}

			// Wedges[Face.iWedge[VertexIndex]].iVertex;
			// Vert2Faces.AddUnique(Wedges[Face.iWedge[VertexIndex]].iVertex, FaceIndex);
		}
	}

	PrintWarning("That was successful");

	/*
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

		PointsOffset = Points.Num();
		for (const FVector& Point : MeshNode->PointsTable)
		{
			const FVector& TransformedPoint = MeshNode->LocalMatrix.TransformPosition(Point);
			int32 VertIndex = Points.Add(TransformedPoint);
			PointsMap.Add(VertIndex);
		}

		int32 PolyNum = MeshNode->PolygonTable.Num();
		for (int j = PolyNum - 1; j >= 0; j--)
		{
			const FMeshPolygonData& PolyData = MeshNode->PolygonTable[j];
			int32 SubNum = PolyData.FaceSubDatas.Num();
			check(SubNum == 3);

			SkeletalMeshImportData::FMeshFace MeshFace;

			MeshFace.MeshMaterialIndex = PolyData.MaterialID;
			MeshFace.SmoothingGroups = 1;

			for (int k = SubNum - 1; k >= 0; k--)
			{
				const FFaceSubData& FaceData = PolyData.FaceSubDatas[k];

				MeshFace.iWedge[2 - k] = PointsOffset + FaceData.p;
				// RawMesh.WedgeIndices.Add(PointsOffset + FaceData.p);

				if (MeshNode->TangentTanTable.Num() > FaceData.n_tan)
				{
					const FVector& Tangent_X = MeshNode->TangentTanTable[FaceData.n_tan];
					MeshFace.TangentX[2 - k] = Tangent_X;
					// RawMesh.WedgeTangentX.Add(Tangent_X);
				}
				else
				{
					MeshFace.TangentX[2 - k] = FVector::ZeroVector;
					// RawMesh.WedgeTangentX.Add(FVector::ZeroVector);
				}


				if (MeshNode->TangentBinTable.Num() > FaceData.n_bin)
				{
					const FVector& Tangent_Y = MeshNode->TangentBinTable[FaceData.n_bin];
					MeshFace.TangentY[2 - k] = Tangent_Y;
					// RawMesh.WedgeTangentY.Add(Tangent_Y);
				}
				else
				{
					MeshFace.TangentY[2 - k] = FVector::ZeroVector;
					// RawMesh.WedgeTangentY.Add(FVector::ZeroVector);
				}

				if (MeshNode->NormalsTable.Num() > FaceData.n)
				{
					const FVector& Normal = MeshNode->NormalsTable[FaceData.n];
					MeshFace.TangentZ[2 - k] = Normal;
					// RawMesh.WedgeTangentZ.Add(Normal);
				}
				else
				{
					MeshFace.TangentZ[2 - k] = FVector::ZeroVector;
					// RawMesh.WedgeTangentZ.Add(FVector::ZeroVector);
				}

				SkeletalMeshImportData::FMeshWedge MeshWedge;
				MeshWedge.iVertex = PointsOffset + FaceData.p;
				MeshWedge.Color = FColor(0, 0, 0);

				// RawMesh.WedgeColors.Add(FColor(0, 0, 0));

				if (MeshNode->TexCoordTable.Num() > FaceData.uv)
				{
					FVector TexCoord = MeshNode->TexCoordTable[FaceData.uv];
					MeshWedge.UVs[0] = FVector2D(TexCoord.X, TexCoord.Y);
					MeshWedge.UVs[1] = FVector2D(0, 0);
					// RawMesh.WedgeTexCoords[0].Add(FVector2D(TexCoord.X, TexCoord.Y));
					// RawMesh.WedgeTexCoords[1].Add(FVector2D(0, 0));
				}
				else
				{
					MeshWedge.UVs[0] = FVector2D(0, 0);
					MeshWedge.UVs[1] = FVector2D(0, 0);
					// RawMesh.WedgeTexCoords[0].Add(FVector2D(0, 0));
					// RawMesh.WedgeTexCoords[1].Add(FVector2D(0, 0));
				}

				Wedges.Add(MeshWedge);
			}
			Faces.Add(MeshFace);
		}

		//~ Begin bone
		int32 PhysiqueTableNum = MeshNode->PhysiqueTable.Num();
		for (int i = 0; i < PhysiqueTableNum; i++)
		{
			int32 VertexIndex = (PhysiqueTableNum - 1) - i;
			
			const FPhysiqueInfo& PhysiqueInfo = MeshNode->PhysiqueTable[i];
			for (const FPhysiqueSubData& PhysiqueSubData : PhysiqueInfo.PhysiqueSubDatas)
			{
				FString BoneName = EluMeshNodes[MeshNode->BoneTableIndex[PhysiqueSubData.cid]]->NodeName;
				FString SanitizedBoneName = PackageTools::SanitizePackageName(BoneName);

				int32 BoneIndex = Skel->GetReferenceSkeleton().FindBoneIndex(FName(*SanitizedBoneName));
				if (BoneIndex < 0)
				{
					FString LogMessage = TEXT("Couldn't find bone index for bone name: ") + SanitizedBoneName;
					PrintError(LogMessage);
				}

				SkeletalMeshImportData::FVertInfluence VertInfluence;
				VertInfluence.VertIndex = PointsOffset + VertexIndex;
				VertInfluence.Weight = PhysiqueSubData.weight;
				VertInfluence.BoneIndex = BoneIndex;

				// Influences.AddUnique(VertInfluence);
				Influences.Add(VertInfluence);
			}
		}

		/*
		const FPhysiqueInfo& PhysiqueInfo = MeshNode->PhysiqueTable[FaceData.p];
		for (const FPhysiqueSubData& PhysiqueSubData : PhysiqueInfo.PhysiqueSubDatas)
		{
			FString BoneName = EluMeshNodes[MeshNode->BoneTableIndex[PhysiqueSubData.cid]]->NodeName;
			FString SanitizedBoneName = PackageTools::SanitizePackageName(BoneName);

			int32 BoneIndex = Skel->GetReferenceSkeleton().FindBoneIndex(FName(*SanitizedBoneName));
			if (BoneIndex < 0)
			{
				FString LogMessage = TEXT("Couldn't find bone index for bone name: ") + SanitizedBoneName;
				PrintError(LogMessage);
			}

			SkeletalMeshImportData::FVertInfluence VertInfluence;
			VertInfluence.VertIndex = PointsOffset + FaceData.p;
			VertInfluence.Weight = PhysiqueSubData.weight;
			VertInfluence.BoneIndex = BoneIndex;

			// Influences.AddUnique(VertInfluence);
			Influences.Add(VertInfluence);
		}
		/////////////
		//~ End bone
	}
	*/

	IMeshUtilities::MeshBuildOptions BuildSettings;
	BuildSettings.bUseMikkTSpace = false;
	BuildSettings.bComputeNormals = false;
	BuildSettings.bComputeTangents = false;
	BuildSettings.bRemoveDegenerateTriangles = true;
	

	
	IMeshUtilities& MeshUtilities = FModuleManager::Get().LoadModuleChecked<IMeshUtilities>("MeshUtilities");
	bool success = MeshUtilities.BuildSkeletalMesh(LODModel, SkeletalMesh->RefSkeleton, Influences, Wedges, Faces, Points, PointsMap, BuildSettings);

	if (!success)
	{
		PrintError("Unable to create new skeletal LOD");
		//~ @todo return
	}

	SkeletalMesh->CalculateRequiredBones(LODModel, SkeletalMesh->RefSkeleton, nullptr);
	SkeletalMesh->CalculateInvRefMatrices();

	SkeletalMesh->Skeleton->RecreateBoneTree(SkeletalMesh);
	SkeletalMesh->Skeleton->SetPreviewMesh(SkeletalMesh);

	// calculate bounds from points
	// mesh->SetImportedBounds(FBoxSphereBounds(points.GetData(), points.Num()));

	SkeletalMesh->Skeleton->PostEditChange();
	SkeletalMesh->Skeleton->MarkPackageDirty();

	SkeletalMesh->PostEditChange();
	SkeletalMesh->MarkPackageDirty();
	

	return true;
}
