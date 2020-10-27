// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


enum RMESH_ALIGN : int
{
	RMA_NONE = 0,
	RMA_NORMAL = 1,
	RMA_Z_FIXED = 2
};

class EDITORTOOLS_API FFaceSubData
{
public:
	WORD p;		// pos		index	
	WORD uv;	// tex uv	index
	WORD uv2;	// extra uv index
	WORD n;		// normal	index
	WORD n_tan;	// tangent	index
	WORD n_bin;	// binormal  index

	FString ToString()
	{
		FString	FinalString;
		FinalString = TEXT("p: ") + FString::FromInt(p) + TEXT(", uv: ") + FString::FromInt(uv) + TEXT(", uv2: ") + FString::FromInt(uv2) +
			TEXT(", n: ") + FString::FromInt(n) + TEXT(", n_tan: ") + FString::FromInt(n_tan) + TEXT(", n_bin: ") + FString::FromInt(n_bin);
		return FinalString;
	}
};

class EDITORTOOLS_API FVertexIndex
{
public:
	WORD p;		// pos		index
	WORD n;		// normal	index
	WORD uv;	// tex uv	index
	WORD uv2;	// extra uv index
	WORD n_tan;	// tangent	index
	WORD n_bin;	// binormal  index
};

class EDITORTOOLS_API FVertexIndex_v12
{
public:
	WORD p;		// pos		index
	WORD n;		// normal	index
	WORD uv;	// tex uv	index
	WORD n_tan;	// tangent	index
	WORD n_bin;	// binormal  index
};

class EDITORTOOLS_API FMeshPolygonData
{
public:
	int Vertices;
	TArray<FFaceSubData> FaceSubDatas;
	short MaterialID;

	FString ToString()
	{
		FString FinalString;
		FinalString += TEXT("Material ID: ") + FString::FromInt(MaterialID) + TEXT("\n");
		for (int i = 0; i < FaceSubDatas.Num(); i++)
		{
			FFaceSubData& SubData = FaceSubDatas[i];
			FinalString += TEXT("SubData_") + FString::FromInt(i) + TEXT(": ") + SubData.ToString() + TEXT("\n");
		}
		return FinalString;
	}
};

class EDITORTOOLS_API FPhysiqueSubData
{
public:
	// FString		name;
	WORD		cid;
	WORD		pid;
	float		weight;
};

class EDITORTOOLS_API FPhysiqueInfo
{
public:
	int Num;
	TArray<FPhysiqueSubData> PhysiqueSubDatas;
};

class EDITORTOOLS_API FMtrlTableInfo
{
public:
	int		mtrlid;
	WORD	offset;
	WORD	count;
	// In 3D Max, you can specify the sub material ID in polygon units. The value is stored.
	// The engine will use this value to determine if the mesh portion should be drawn.
	// See RActorNode :: SetSubMaterialMask (), RActorNode :: GetSubMaterialMask ().
	int		nSubMaterialIDForDrawMasking;
};

class EDITORTOOLS_API FBoundingBox
{
public:
	FVector vmin;
	FVector vmax;

	void Initilalize()
	{
		vmin = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
		vmax = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	}

	void Add(const FVector& Vector)
	{
		if (vmin.X > Vector.X)	vmin.X = Vector.X;
		if (vmin.Y > Vector.Y)	vmin.Y = Vector.Y;
		if (vmin.Z > Vector.Z)	vmin.Z = Vector.Z;
		if (vmax.X < Vector.X)	vmax.X = Vector.X;
		if (vmax.Y < Vector.Y)	vmax.Y = Vector.Y;
		if (vmax.Z < Vector.Z)	vmax.Z = Vector.Z;
	}

};

class EDITORTOOLS_API FEluMeshNode
{
public:

	int BipID;

	FString NodeName;
	FString NodeParentName;
	int ParentNodeID;

	DWORD dwFlag;
	RMESH_ALIGN MeshAlign;
	float BaseVisibility;
	int LODProjectIndex;

	FMatrix LocalMatrix;

	int PointsCount;
	TArray<FVector> PointsTable;

	int NormalsCount;
	TArray<FVector> NormalsTable;

	int TangentTanCount;
	TArray<FVector4> TangentTanTable;

	int TangentBinCount;
	TArray<FVector> TangentBinTable;

	int TexCoordCount;
	TArray<FVector> TexCoordTable;

	int TexCoordExtraCount;
	TArray<FVector> TexCoordExtraTable;

	int FaceCount;
	TArray<FMeshPolygonData> PolygonTable;

	int TotalDegree;	// I suppose it means total number of angles?
	int TotalTriangles;

	int PointColorCount;
	TArray<FVector> PointColorTable;

	int MaterialID;

	int PhysiqueCount;
	TArray<FPhysiqueInfo> PhysiqueTable;

	int BoneCount;
	TArray<FMatrix> BoneTable;
	TArray<WORD> BoneTableIndex;

	int VertexIndexCount;
	TArray<FVertexIndex> VertexIndexTable;

	int FaceIndexCount;
	TArray<WORD> FaceIndexTable;

	int MaterialInfoCount;
	TArray<FMtrlTableInfo> MaterialInfoTable;

	FBoundingBox BoundingBox;

	void CalculateLocalBoundingBox()
	{
		for (int i = 0; i < PointsTable.Num(); i++)
		{
			BoundingBox.Add(PointsTable[i]);
		}
	}

	FEluMeshNode()
	{
		PointsCount = 0;
		NormalsCount = 0;
		TangentTanCount = 0;
		TangentBinCount = 0;
		TexCoordCount = 0;
		FaceCount = 0;
		TotalDegree = 0;
		TotalTriangles = 0;
		PointColorCount = 0;
		MaterialID = 0;
		PhysiqueCount = 0;
		BoneCount = 0;
		VertexIndexCount = 0;
		FaceIndexCount = 0;
		MaterialInfoCount = 0;
	}

	void AddFlag(DWORD Flag) { dwFlag |= Flag; }

	bool HasFlag(DWORD Flag) { return dwFlag & Flag; }

	FString ToString()
	{
		FString FinalString;
		FinalString += TEXT("Node name: ") + NodeName + TEXT("\n");
		FinalString += TEXT("LODIndex: ") + FString::FromInt(LODProjectIndex)  + TEXT("\n");

		FinalString += TEXT("Num of points: ") + FString::FromInt(PointsTable.Num())  + TEXT("\n");
		FinalString += TEXT("Num of normals: ") + FString::FromInt(NormalsTable.Num())  + TEXT("\n");
		FinalString += TEXT("Num of tangents tan: ") + FString::FromInt(TangentTanTable.Num())  + TEXT("\n");
		FinalString += TEXT("Num of tangents bin: ") + FString::FromInt(TangentBinTable.Num())  + TEXT("\n");

		FinalString += TEXT("Num of tex coords: ") + FString::FromInt(TexCoordTable.Num())  + TEXT("\n");
		FinalString += TEXT("Num of extra tex coords: ") + FString::FromInt(TexCoordExtraTable.Num())  + TEXT("\n");

		FinalString += TEXT("Num of point color: ") + FString::FromInt(PointColorTable.Num())  + TEXT("\n");

		for (int i = 0; i < PolygonTable.Num(); i++)
		{
			FMeshPolygonData& PolyData = PolygonTable[i];
			FinalString += TEXT("PolyData_") + FString::FromInt(i) + TEXT(": ") + PolyData.ToString() + TEXT("\n");
		}

		return FinalString;
	}
};


/**
 * 
 */
class EDITORTOOLS_API FEluMeshNodeLoader
{
public:
	FEluMeshNodeLoader();
	virtual ~FEluMeshNodeLoader();

	int CurrentEluVersion;

	/**
	 * Loads the elu mesh node info from BinaryData stream.
	 * @param MeshNode The EluMeshNode object to write info to.
	 * @param BinaryData Binary stream to read binary data from
	 * @param Offset Offset to start reading binary data from in binary stream. This will get modified.
	 * @return Returns true if binary data writing was successful.
	 */
	virtual bool Load(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) = 0;
};


class EDITORTOOLS_API FEluMeshNodeLoader_v12 : public FEluMeshNodeLoader
{

public:
	virtual bool Load(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset);
	
	//~ 
	virtual bool LoadName(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset);

	virtual bool LoadInfo(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset);

	virtual bool LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset);

	virtual bool LoadFace(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset);

	virtual bool LoadVertexInfo(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset);

	virtual bool LoadEtc(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset);
	//~
};

class EDITORTOOLS_API FEluMeshNodeLoader_v13 : public FEluMeshNodeLoader_v12
{

public:
	virtual bool Load(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) override;
	
	//~
	virtual bool LoadEtc(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) override;
	//~
};

class EDITORTOOLS_API FEluMeshNodeLoader_v14 : public FEluMeshNodeLoader_v13
{

public:
	
	virtual bool LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) override;

};

class EDITORTOOLS_API FEluMeshNodeLoader_v15 : public FEluMeshNodeLoader_v14
{

public:
	virtual bool LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) override;

	virtual bool LoadFace(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) override;
	
	virtual bool LoadEtc(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) override;
};

class EDITORTOOLS_API FEluMeshNodeLoader_v16 : public FEluMeshNodeLoader_v15
{

public:
	virtual bool LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) override;

};

class EDITORTOOLS_API FEluMeshNodeLoader_v17 : public FEluMeshNodeLoader_v16
{

public:
	virtual bool LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) override;
};

class EDITORTOOLS_API FEluMeshNodeLoader_v18 : public FEluMeshNodeLoader_v17
{

public:
	virtual bool LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) override;
};

class EDITORTOOLS_API FEluMeshNodeLoader_v20 : public FEluMeshNodeLoader_v18
{

public:
	virtual bool LoadName(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) override;

	virtual bool LoadInfo(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) override;

	virtual bool LoadVertex(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) override;

	virtual bool LoadEtc(TSharedPtr<FEluMeshNode> MeshNode, TArray<uint8>& BinaryData, UINT& Offset) override;

};
