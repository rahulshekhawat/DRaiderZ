// Fill out your copyright notice in the Description page of Project Settings.

#include "EluMeshNodeLoader.h"
#include "RaiderzXmlUtilities.h"
#include "EluLibrary.h"

FEluMeshNodeLoader::FEluMeshNodeLoader()
{
}

FEluMeshNodeLoader::~FEluMeshNodeLoader()
{
}

bool FEluMeshNodeLoader_v12::Load(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	check(MeshNode);
	check(BinaryData.GetData());

	if (!LoadName(MeshNode, BinaryData, Offset))
	{
		return false;
	}
	if (!LoadInfo(MeshNode, BinaryData, Offset))
	{
		return false;
	}
	if (!LoadVertex(MeshNode, BinaryData, Offset))
	{
		return false;
	}
	if (!LoadFace(MeshNode, BinaryData, Offset))
	{
		return false;
	}
	if (!LoadVertexInfo(MeshNode, BinaryData, Offset))
	{
		return false;
	}
	if (!LoadEtc(MeshNode, BinaryData, Offset))
	{
		return false;
	}

	if(MeshNode->BoundingBox.vmin.X == FLT_MAX)	
	{
		MeshNode->BoundingBox.vmin = FVector(-0.1f,-0.1f,-0.1f);
		MeshNode->BoundingBox.vmax = FVector( 0.1f, 0.1f, 0.1f);
	}

	return true;
}

bool FEluMeshNodeLoader_v12::LoadName(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	if (!URaiderzXmlUtilities::ReadStringFromBinaryData(MeshNode->NodeName, BinaryData, Offset))
	{
		return false;
	}
	if (!URaiderzXmlUtilities::ReadStringFromBinaryData(MeshNode->NodeParentName, BinaryData, Offset))
	{
		return false;
	}
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->ParentNodeID, sizeof(MeshNode->ParentNodeID), BinaryData, Offset))
	{
		return false;
	}
	return true;
}

bool FEluMeshNodeLoader_v12::LoadInfo(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->dwFlag, sizeof(MeshNode->dwFlag), BinaryData, Offset))
	{
		return false;
	}
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->MeshAlign, sizeof(MeshNode->MeshAlign), BinaryData, Offset))
	{
		return false;
	}

	if (CurrentEluVersion < EXPORTER_MESH_VER11)
	{
		// What the fuck are these used for?
		int AniPartsType;
		URaiderzXmlUtilities::WriteBinaryDataToBuffer(&AniPartsType, sizeof(AniPartsType), BinaryData, Offset);
		int PartsPosInfoType;
		URaiderzXmlUtilities::WriteBinaryDataToBuffer(&PartsPosInfoType, sizeof(PartsPosInfoType), BinaryData, Offset);
		int PartsType;
		URaiderzXmlUtilities::WriteBinaryDataToBuffer(&PartsType, sizeof(PartsType), BinaryData, Offset);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->LocalMatrix, sizeof(MeshNode->LocalMatrix), BinaryData, Offset))
	{
		return false;
	}

	if (CurrentEluVersion >= EXPORTER_MESH_VER11)
	{
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->BaseVisibility, sizeof(MeshNode->BaseVisibility), BinaryData, Offset))
		{
			return false;
		}
	}

	return true;
}

bool FEluMeshNodeLoader_v12::LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->PointsCount, sizeof(MeshNode->PointsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->PointsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->PointsTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->NormalsCount, sizeof(MeshNode->NormalsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->NormalsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->NormalsTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentTanCount, sizeof(MeshNode->TangentTanCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentTanCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentTanTable.Add(FVector4(Point));
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentBinCount, sizeof(MeshNode->TangentBinCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentBinCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentBinTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TexCoordCount, sizeof(MeshNode->TexCoordCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TexCoordCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TexCoordTable.Add(Point);
	}

	return true;
}

bool FEluMeshNodeLoader_v12::LoadFace(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->FaceCount, sizeof(MeshNode->FaceCount), BinaryData, Offset))
	{
		return false;
	}

	if (MeshNode->FaceCount)
	{
		if (CurrentEluVersion < EXPORTER_MESH_VER12)
		{
			for (int i = 0; i < MeshNode->FaceCount; i++)
			{
				FMeshPolygonData PolyData;
				PolyData.Vertices = 3;
				for (int j = 0; j < 3; j++)
				{
					FFaceSubData FaceSubData;
					URaiderzXmlUtilities::WriteBinaryDataToBuffer(&FaceSubData, sizeof(FaceSubData), BinaryData, Offset);
					PolyData.FaceSubDatas.Add(FaceSubData);

				}
				
				URaiderzXmlUtilities::WriteBinaryDataToBuffer(&PolyData.MaterialID, sizeof(PolyData.MaterialID), BinaryData, Offset);
				MeshNode->PolygonTable.Add(PolyData);
			}
			
			MeshNode->TotalDegree = MeshNode->FaceCount * 3;
			MeshNode->TotalTriangles = MeshNode->FaceCount;
		}
		else
		{
			URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TotalDegree, sizeof(MeshNode->TotalDegree), BinaryData, Offset);
			URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TotalTriangles, sizeof(MeshNode->TotalTriangles), BinaryData, Offset);

			for (int i = 0; i < MeshNode->FaceCount; i++)
			{
				int Deg = 0;
				URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Deg, sizeof(Deg), BinaryData, Offset);

				FMeshPolygonData PolyData;
				PolyData.Vertices = Deg;

				for (int j = 0; j < Deg; j++)
				{
					FFaceSubData FaceSubData;
					URaiderzXmlUtilities::WriteBinaryDataToBuffer(&FaceSubData, sizeof(FaceSubData), BinaryData, Offset);
					PolyData.FaceSubDatas.Add(FaceSubData);
				}

				URaiderzXmlUtilities::WriteBinaryDataToBuffer(&PolyData.MaterialID, sizeof(PolyData.MaterialID), BinaryData, Offset);
				MeshNode->PolygonTable.Add(PolyData);
			}

			//~ Begin MeshNode->TotalDegree verification
			int Total = 0;
			for (FMeshPolygonData PolyData : MeshNode->PolygonTable)
			{
				for (FFaceSubData FaceSubData : PolyData.FaceSubDatas)
				{
					Total++;
				}
			}

			check(Total == MeshNode->TotalDegree);
			//~ End MeshNode->TotalDegree verification

		}
	}

	return true;
}

bool FEluMeshNodeLoader_v12::LoadVertexInfo(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->PointColorCount, sizeof(MeshNode->PointColorCount), BinaryData, Offset))
	{
		return false;
	}

	for (int i = 0; i < MeshNode->PointColorCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->PointColorTable.Add(Point);
	}

	if (MeshNode->PointsCount == 0 || MeshNode->FaceCount == 0)
	{
		MeshNode->AddFlag(RM_FLAG_DUMMY_MESH);
	}

	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->MaterialID, sizeof(MeshNode->MaterialID), BinaryData, Offset);

	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->PhysiqueCount, sizeof(MeshNode->PhysiqueCount), BinaryData, Offset);

	if (MeshNode->PhysiqueCount)
	{
		check(MeshNode->PointsCount == MeshNode->PhysiqueCount);
	}

	for (int i = 0; i < MeshNode->PhysiqueCount; i++)
	{
		FPhysiqueInfo PhysiqueInfo;
		URaiderzXmlUtilities::WriteBinaryDataToBuffer(&PhysiqueInfo.Num, sizeof(PhysiqueInfo.Num), BinaryData, Offset);

		for (int k = 0; k < PhysiqueInfo.Num; k++)
		{
			FPhysiqueSubData PhysiqueSubData;
			URaiderzXmlUtilities::WriteBinaryDataToBuffer(&PhysiqueSubData, sizeof(PhysiqueSubData), BinaryData, Offset);
			PhysiqueInfo.PhysiqueSubDatas.Add(PhysiqueSubData);
		}

		if (PhysiqueInfo.Num > PHYSIQUE_MAX_WEIGHT)
		{
			for (int m = 0; m < PhysiqueInfo.Num; m++)
			{
				for (int n = m + 1; n < PhysiqueInfo.Num; n++)
				{
					if (PhysiqueInfo.PhysiqueSubDatas[m].weight < PhysiqueInfo.PhysiqueSubDatas[n].weight)
					{
						FPhysiqueSubData temp;
						temp = PhysiqueInfo.PhysiqueSubDatas[m];
						PhysiqueInfo.PhysiqueSubDatas[m] = PhysiqueInfo.PhysiqueSubDatas[n];
						PhysiqueInfo.PhysiqueSubDatas[n] = temp;

					}
				}
			}

#if PHYSIQUE_MAX_WEIGHT == 4
			float fSum3 = PhysiqueInfo.PhysiqueSubDatas[0].weight + PhysiqueInfo.PhysiqueSubDatas[1].weight + PhysiqueInfo.PhysiqueSubDatas[2].weight + PhysiqueInfo.PhysiqueSubDatas[3].weight;
			PhysiqueInfo.PhysiqueSubDatas[0].weight = PhysiqueInfo.PhysiqueSubDatas[0].weight / fSum3;
			PhysiqueInfo.PhysiqueSubDatas[1].weight = PhysiqueInfo.PhysiqueSubDatas[1].weight / fSum3;
			PhysiqueInfo.PhysiqueSubDatas[2].weight = PhysiqueInfo.PhysiqueSubDatas[2].weight / fSum3;
			PhysiqueInfo.PhysiqueSubDatas[3].weight = PhysiqueInfo.PhysiqueSubDatas[3].weight / fSum3;
			PhysiqueInfo.Num = 4;
#else
			float fSum3 = PhysiqueInfo.PhysiqueSubDatas[0].weight + PhysiqueInfo.PhysiqueSubDatas[1].weight + PhysiqueInfo.PhysiqueSubDatas[2].weight;
			PhysiqueInfo.PhysiqueSubDatas[0].weight = PhysiqueInfo.PhysiqueSubDatas[0].weight / fSum3;
			PhysiqueInfo.PhysiqueSubDatas[1].weight = PhysiqueInfo.PhysiqueSubDatas[1].weight / fSum3;
			PhysiqueInfo.PhysiqueSubDatas[2].weight = PhysiqueInfo.PhysiqueSubDatas[2].weight / fSum3;
			PhysiqueInfo.Num = 3;
#endif
		}

		MeshNode->PhysiqueTable.Add(PhysiqueInfo);

	}

	return true;
}

bool FEluMeshNodeLoader_v12::LoadEtc(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->BoneCount, sizeof(MeshNode->BoneCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->BoneCount; i++)
	{
		FMatrix BoneMatrix;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&BoneMatrix, sizeof(BoneMatrix), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->BoneTable.Add(BoneMatrix);
	}
	for (int i = 0; i < MeshNode->BoneCount; i++)
	{
		WORD BoneIndex;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&BoneIndex, sizeof(BoneIndex), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->BoneTableIndex.Add(BoneIndex);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->VertexIndexCount, sizeof(MeshNode->VertexIndexCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->VertexIndexCount; i++)
	{
		FVertexIndex VertexIndex;
		FVertexIndex_v12 VertexIndex_v12;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&VertexIndex_v12, sizeof(VertexIndex_v12), BinaryData, Offset))
		{
			return false;
		}

		VertexIndex.p = VertexIndex_v12.p;
		VertexIndex.n = VertexIndex_v12.n;
		VertexIndex.uv = VertexIndex_v12.uv;
		VertexIndex.uv2 = -1;
		VertexIndex.n_bin = VertexIndex_v12.n_bin;
		VertexIndex.n_tan = VertexIndex_v12.n_tan;

		MeshNode->VertexIndexTable.Add(VertexIndex);
	}

	if (CurrentEluVersion < EXPORTER_MESH_VER12)
	{
		MeshNode->FaceIndexCount = MeshNode->FaceCount * 3;

		for (int i = 0; i < MeshNode->FaceIndexCount; i++)
		{
			WORD FaceIndex;
			if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&FaceIndex, sizeof(FaceIndex), BinaryData, Offset))
			{
				return false;
			}
			MeshNode->FaceIndexTable.Add(FaceIndex);
		}

	}
	else
	{
		//~ Begin neglected data
		int PrimitiveType;
		URaiderzXmlUtilities::WriteBinaryDataToBuffer(&PrimitiveType, sizeof(PrimitiveType), BinaryData, Offset);
		//~ End neglected data

		URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->FaceIndexCount, sizeof(MeshNode->FaceIndexCount), BinaryData, Offset);

		for (int i = 0; i < MeshNode->FaceIndexCount; i++)
		{
			WORD FaceIndex;
			if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&FaceIndex, sizeof(FaceIndex), BinaryData, Offset))
			{
				return false;
			}
			MeshNode->FaceIndexTable.Add(FaceIndex);
		}

	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->MaterialInfoCount, sizeof(MeshNode->MaterialInfoCount), BinaryData, Offset))
	{
		return false;
	}
	if (CurrentEluVersion < EXPORTER_MESH_VER9)
	{
		for (int i = 0; i < MeshNode->MaterialInfoCount; i++)
		{
			FMtrlTableInfo MtrlTableInfo;
			URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MtrlTableInfo.mtrlid, sizeof(MtrlTableInfo.mtrlid), BinaryData, Offset);
			URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MtrlTableInfo.offset, sizeof(MtrlTableInfo.offset), BinaryData, Offset);
			URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MtrlTableInfo.count, sizeof(MtrlTableInfo.count), BinaryData, Offset);
			MtrlTableInfo.nSubMaterialIDForDrawMasking = 0;
			
			MeshNode->MaterialInfoTable.Add(MtrlTableInfo);
		}
	}
	else
	{
		for (int i = 0; i < MeshNode->MaterialInfoCount; i++)
		{
			FMtrlTableInfo MtrlTableInfo;

			URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MtrlTableInfo, sizeof(MtrlTableInfo), BinaryData, Offset);

			MeshNode->MaterialInfoTable.Add(MtrlTableInfo);
		}
	}

	// @todo add BipID if it's needed anywhere
	// MeshNode->BipID = 

	return true;
}

bool FEluMeshNodeLoader_v13::Load(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	check(MeshNode);
	check(BinaryData.GetData());

	if (!LoadName(MeshNode, BinaryData, Offset))
	{
		return false;
	}
	if (!LoadInfo(MeshNode, BinaryData, Offset))
	{
		return false;
	}
	if (!LoadVertex(MeshNode, BinaryData, Offset))
	{
		return false;
	}
	if (!LoadFace(MeshNode, BinaryData, Offset))
	{
		return false;
	}
	if (!LoadVertexInfo(MeshNode, BinaryData, Offset))
	{
		return false;
	}
	if (!LoadEtc(MeshNode, BinaryData, Offset))
	{
		return false;
	}

	// Bounding box value is loaded in version 13.

	return true;
}

bool FEluMeshNodeLoader_v13::LoadEtc(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	if (!FEluMeshNodeLoader_v12::LoadEtc(MeshNode, BinaryData, Offset))
	{
		return false;
	}

	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->BoundingBox.vmin, sizeof(FVector), BinaryData, Offset);
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->BoundingBox.vmax, sizeof(FVector), BinaryData, Offset);

	return true;
}

bool FEluMeshNodeLoader_v15::LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	// @unknown
	DWORD dwFVF;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&dwFVF, sizeof(dwFVF), BinaryData, Offset);

	//~ Begin neglected data
	int LightMapID;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&LightMapID, sizeof(LightMapID), BinaryData, Offset);
	//~ End neglected data

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->PointsCount, sizeof(MeshNode->PointsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->PointsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->PointsTable.Add(Point);
	}

	MeshNode->CalculateLocalBoundingBox();

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->NormalsCount, sizeof(MeshNode->NormalsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->NormalsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->NormalsTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentTanCount, sizeof(MeshNode->TangentTanCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentTanCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentTanTable.Add(FVector4(Point));
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentBinCount, sizeof(MeshNode->TangentBinCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentBinCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentBinTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TexCoordCount, sizeof(MeshNode->TexCoordCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TexCoordCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TexCoordTable.Add(Point);
	}

	//~ Begin neglected data
	int LightMapTexCoordTableCount;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&LightMapTexCoordTableCount, sizeof(LightMapTexCoordTableCount), BinaryData, Offset);
	Offset += sizeof(FVector) * LightMapTexCoordTableCount;
	//~ End neglected data

	return true;
}

bool FEluMeshNodeLoader_v15::LoadFace(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->FaceCount, sizeof(MeshNode->FaceCount), BinaryData, Offset))
	{
		return false;
	}
	if (MeshNode->FaceCount)
	{
			URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TotalDegree, sizeof(MeshNode->TotalDegree), BinaryData, Offset);
			URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TotalTriangles, sizeof(MeshNode->TotalTriangles), BinaryData, Offset);

			for (int i = 0; i < MeshNode->FaceCount; i++)
			{
				int Deg = 0;
				URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Deg, sizeof(Deg), BinaryData, Offset);

				FMeshPolygonData PolyData;
				PolyData.Vertices = Deg;

				for (int j = 0; j < Deg; j++)
				{
					FFaceSubData FaceSubData;
					URaiderzXmlUtilities::WriteBinaryDataToBuffer(&FaceSubData, sizeof(FaceSubData), BinaryData, Offset);
					PolyData.FaceSubDatas.Add(FaceSubData);
				}

				URaiderzXmlUtilities::WriteBinaryDataToBuffer(&PolyData.MaterialID, sizeof(PolyData.MaterialID), BinaryData, Offset);
				MeshNode->PolygonTable.Add(PolyData);
			}

			//~ Begin MeshNode->TotalDegree verification
			int Total = 0;
			for (FMeshPolygonData PolyData : MeshNode->PolygonTable)
			{
				for (FFaceSubData FaceSubData : PolyData.FaceSubDatas)
				{
					Total++;
				}
			}

			check(Total == MeshNode->TotalDegree);
			//~ End MeshNode->TotalDegree verification
	}

	return true;
}

bool FEluMeshNodeLoader_v15::LoadEtc(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->BoneCount, sizeof(MeshNode->BoneCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->BoneCount; i++)
	{
		FMatrix BoneMatrix;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&BoneMatrix, sizeof(BoneMatrix), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->BoneTable.Add(BoneMatrix);
	}
	for (int i = 0; i < MeshNode->BoneCount; i++)
	{
		WORD BoneIndex;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&BoneIndex, sizeof(BoneIndex), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->BoneTableIndex.Add(BoneIndex);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->VertexIndexCount, sizeof(MeshNode->VertexIndexCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->VertexIndexCount; i++)
	{
		FVertexIndex VertexIndex;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&VertexIndex, sizeof(VertexIndex), BinaryData, Offset))
		{
			return false;
		}

		MeshNode->VertexIndexTable.Add(VertexIndex);
	}

	if (CurrentEluVersion < EXPORTER_MESH_VER12)
	{
		MeshNode->FaceIndexCount = MeshNode->FaceCount * 3;

		for (int i = 0; i < MeshNode->FaceIndexCount; i++)
		{
			WORD FaceIndex;
			if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&FaceIndex, sizeof(FaceIndex), BinaryData, Offset))
			{
				return false;
			}
			MeshNode->FaceIndexTable.Add(FaceIndex);
		}

	}
	else
	{
		//~ Begin neglected data
		int PrimitiveType;
		URaiderzXmlUtilities::WriteBinaryDataToBuffer(&PrimitiveType, sizeof(PrimitiveType), BinaryData, Offset);
		//~ End neglected data

		URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->FaceIndexCount, sizeof(MeshNode->FaceIndexCount), BinaryData, Offset);

		for (int i = 0; i < MeshNode->FaceIndexCount; i++)
		{
			WORD FaceIndex;
			if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&FaceIndex, sizeof(FaceIndex), BinaryData, Offset))
			{
				return false;
			}
			MeshNode->FaceIndexTable.Add(FaceIndex);
		}

	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->MaterialInfoCount, sizeof(MeshNode->MaterialInfoCount), BinaryData, Offset))
	{
		return false;
	}
	if (CurrentEluVersion < EXPORTER_MESH_VER9)
	{
		for (int i = 0; i < MeshNode->MaterialInfoCount; i++)
		{
			FMtrlTableInfo MtrlTableInfo;
			URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MtrlTableInfo.mtrlid, sizeof(MtrlTableInfo.mtrlid), BinaryData, Offset);
			URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MtrlTableInfo.offset, sizeof(MtrlTableInfo.offset), BinaryData, Offset);
			URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MtrlTableInfo.count, sizeof(MtrlTableInfo.count), BinaryData, Offset);
			MtrlTableInfo.nSubMaterialIDForDrawMasking = 0;

			MeshNode->MaterialInfoTable.Add(MtrlTableInfo);
		}
	}
	else
	{
		for (int i = 0; i < MeshNode->MaterialInfoCount; i++)
		{
			FMtrlTableInfo MtrlTableInfo;

			URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MtrlTableInfo, sizeof(MtrlTableInfo), BinaryData, Offset);

			MeshNode->MaterialInfoTable.Add(MtrlTableInfo);
		}
	}

	// @todo add BipID if it's needed anywhere
	// MeshNode->BipID = 

	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->BoundingBox.vmin, sizeof(FVector), BinaryData, Offset);
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->BoundingBox.vmax, sizeof(FVector), BinaryData, Offset);

	return true;
}

bool FEluMeshNodeLoader_v16::LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	// @unknown
	DWORD dwFVF;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&dwFVF, sizeof(dwFVF), BinaryData, Offset);

	//~ Begin neglected data
	int LightMapID;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&LightMapID, sizeof(LightMapID), BinaryData, Offset);
	//~ End neglected data

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->PointsCount, sizeof(MeshNode->PointsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->PointsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->PointsTable.Add(Point);
	}

	MeshNode->CalculateLocalBoundingBox();

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->NormalsCount, sizeof(MeshNode->NormalsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->NormalsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->NormalsTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentTanCount, sizeof(MeshNode->TangentTanCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentTanCount; i++)
	{
		FVector4 Point4;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point4, sizeof(Point4), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentTanTable.Add(Point4);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentBinCount, sizeof(MeshNode->TangentBinCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentBinCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentBinTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TexCoordCount, sizeof(MeshNode->TexCoordCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TexCoordCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TexCoordTable.Add(Point);
	}

	//~ Begin neglected data
	int LightMapTexCoordTableCount;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&LightMapTexCoordTableCount, sizeof(LightMapTexCoordTableCount), BinaryData, Offset);
	Offset += sizeof(FVector) * LightMapTexCoordTableCount;
	//~ End neglected data

	return true;
}

bool FEluMeshNodeLoader_v17::LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	// @unknown
	DWORD dwFVF;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&dwFVF, sizeof(dwFVF), BinaryData, Offset);

	//~ Begin neglected data
	int LightMapID;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&LightMapID, sizeof(LightMapID), BinaryData, Offset);
	//~ End neglected data

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->PointsCount, sizeof(MeshNode->PointsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->PointsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->PointsTable.Add(Point);
	}

	MeshNode->CalculateLocalBoundingBox();

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->NormalsCount, sizeof(MeshNode->NormalsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->NormalsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->NormalsTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentTanCount, sizeof(MeshNode->TangentTanCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentTanCount; i++)
	{
		FVector4 Point4;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point4, sizeof(Point4), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentTanTable.Add(Point4);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentBinCount, sizeof(MeshNode->TangentBinCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentBinCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentBinTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TexCoordCount, sizeof(MeshNode->TexCoordCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TexCoordCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TexCoordTable.Add(Point);
	}

	return true;
}

bool FEluMeshNodeLoader_v18::LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->PointsCount, sizeof(MeshNode->PointsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->PointsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->PointsTable.Add(Point);
	}

	MeshNode->CalculateLocalBoundingBox();

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->NormalsCount, sizeof(MeshNode->NormalsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->NormalsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->NormalsTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentTanCount, sizeof(MeshNode->TangentTanCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentTanCount; i++)
	{
		FVector4 Point4;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point4, sizeof(Point4), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentTanTable.Add(Point4);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentBinCount, sizeof(MeshNode->TangentBinCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentBinCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentBinTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TexCoordCount, sizeof(MeshNode->TexCoordCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TexCoordCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TexCoordTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TexCoordExtraCount, sizeof(MeshNode->TexCoordExtraCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TexCoordExtraCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TexCoordExtraTable.Add(Point);
	}

	return true;
}

bool FEluMeshNodeLoader_v20::LoadName(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	if (!URaiderzXmlUtilities::ReadStringFromBinaryData(MeshNode->NodeName, BinaryData, Offset))
	{
		// UE_LOG(LogTemp, Warning, TEXT("v20 : Reading node name failed"));
		return false;
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->ParentNodeID, sizeof(MeshNode->ParentNodeID), BinaryData, Offset))
	{
		// UE_LOG(LogTemp, Warning, TEXT("v20 : Reading parent node ID failed"));
		return false;
	}

	if (!URaiderzXmlUtilities::ReadStringFromBinaryData(MeshNode->NodeParentName, BinaryData, Offset))
	{
		// UE_LOG(LogTemp, Warning, TEXT("v20 : Reading parent node name failed"));
		return false;
	}
	return true;
}

bool FEluMeshNodeLoader_v20::LoadInfo(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->LocalMatrix, sizeof(MeshNode->LocalMatrix), BinaryData, Offset))
	{
		return false;
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->BaseVisibility, sizeof(MeshNode->BaseVisibility), BinaryData, Offset))
	{
		return false;
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->dwFlag, sizeof(MeshNode->dwFlag), BinaryData, Offset))
	{
		return false;
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->MeshAlign, sizeof(MeshNode->MeshAlign), BinaryData, Offset))
	{
		return false;
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->LODProjectIndex, sizeof(MeshNode->LODProjectIndex), BinaryData, Offset))
	{
		return false;
	}

	return true;
}

bool FEluMeshNodeLoader_v20::LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->PointsCount, sizeof(MeshNode->PointsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->PointsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->PointsTable.Add(Point);
	}

	MeshNode->CalculateLocalBoundingBox();

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TexCoordCount, sizeof(MeshNode->TexCoordCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TexCoordCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TexCoordTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TexCoordExtraCount, sizeof(MeshNode->TexCoordExtraCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TexCoordExtraCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TexCoordExtraTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->NormalsCount, sizeof(MeshNode->NormalsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->NormalsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->NormalsTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentTanCount, sizeof(MeshNode->TangentTanCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentTanCount; i++)
	{
		FVector4 Point4;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point4, sizeof(Point4), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentTanTable.Add(Point4);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentBinCount, sizeof(MeshNode->TangentBinCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentBinCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentBinTable.Add(Point);
	}

	return true;
}

bool FEluMeshNodeLoader_v20::LoadEtc(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	//~ Begin neglected data
	int PrimitiveType;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&PrimitiveType, sizeof(PrimitiveType), BinaryData, Offset);
	//~ End neglected data

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->VertexIndexCount, sizeof(MeshNode->VertexIndexCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->VertexIndexCount; i++)
	{
		FVertexIndex VertexIndex;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&VertexIndex, sizeof(VertexIndex), BinaryData, Offset))
		{
			return false;
		}

		MeshNode->VertexIndexTable.Add(VertexIndex);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->BoneCount, sizeof(MeshNode->BoneCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->BoneCount; i++)
	{
		FMatrix BoneMatrix;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&BoneMatrix, sizeof(BoneMatrix), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->BoneTable.Add(BoneMatrix);
	}
	for (int i = 0; i < MeshNode->BoneCount; i++)
	{
		WORD BoneIndex;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&BoneIndex, sizeof(BoneIndex), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->BoneTableIndex.Add(BoneIndex);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->MaterialInfoCount, sizeof(MeshNode->MaterialInfoCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->MaterialInfoCount; i++)
	{
		FMtrlTableInfo MtrlTableInfo;

		URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MtrlTableInfo, sizeof(MtrlTableInfo), BinaryData, Offset);

		MeshNode->MaterialInfoTable.Add(MtrlTableInfo);
	}

	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->FaceIndexCount, sizeof(MeshNode->FaceIndexCount), BinaryData, Offset);
	for (int i = 0; i < MeshNode->FaceIndexCount; i++)
	{
		WORD FaceIndex;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&FaceIndex, sizeof(FaceIndex), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->FaceIndexTable.Add(FaceIndex);
	}

	// @todo add BipID if it's needed anywhere
	// MeshNode->BipID = 

	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->BoundingBox.vmin, sizeof(FVector), BinaryData, Offset);
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->BoundingBox.vmax, sizeof(FVector), BinaryData, Offset);

	if (MeshNode->BoundingBox.vmin.X == FLT_MAX)
	{
		MeshNode->BoundingBox.vmin = FVector(-0.1f, -0.1f, -01.f);
		MeshNode->BoundingBox.vmax = FVector(0.1f, 0.1f, 01.f);
	}

	return true;
}

bool FEluMeshNodeLoader_v14::LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset)
{
	// @unknown
	DWORD dwFVF;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&dwFVF, sizeof(dwFVF), BinaryData, Offset);

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->PointsCount, sizeof(MeshNode->PointsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->PointsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->PointsTable.Add(Point);
	}

	MeshNode->CalculateLocalBoundingBox();

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->NormalsCount, sizeof(MeshNode->NormalsCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->NormalsCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->NormalsTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentTanCount, sizeof(MeshNode->TangentTanCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentTanCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentTanTable.Add(FVector4(Point));
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TangentBinCount, sizeof(MeshNode->TangentBinCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TangentBinCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TangentBinTable.Add(Point);
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&MeshNode->TexCoordCount, sizeof(MeshNode->TexCoordCount), BinaryData, Offset))
	{
		return false;
	}
	for (int i = 0; i < MeshNode->TexCoordCount; i++)
	{
		FVector Point;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Point, sizeof(Point), BinaryData, Offset))
		{
			return false;
		}
		MeshNode->TexCoordTable.Add(Point);
	}

	return true;
}
