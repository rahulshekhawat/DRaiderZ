// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class EDITORTOOLS_API FAnimType
{
public:
	int Type;
	int CountType;
	int Count;
};

class EDITORTOOLS_API FShortVecKey
{
public:
	int Frame;
	unsigned short m_x, m_y, m_z;

	float X() const { return halfToFloat(m_x); }
	float Y() const { return halfToFloat(m_y); }
	float Z() const { return halfToFloat(m_z); }

private:

	float halfToFloat(unsigned short var) const
	{
		union { float f; unsigned int i; } v;
		v.i = halfToFloatI(var);
		return v.f;
	}

	unsigned int halfToFloatI(unsigned short var) const
	{
		int s = (var >> 15) & 0x00000001;
		int e = (var >> 10) & 0x0000001f;
		int m = var & 0x000003ff;

		if (e == 0)
		{
			if (m == 0) // Plus or minus zero
			{
				return s << 31;
			}
			else // Denormalized number -- renormalize it
			{
				while (!(m & 0x00000400))
				{
					m <<= 1;
					e -= 1;
				}

				e += 1;
				m &= ~0x00000400;
			}
		}
		else if (e == 31)
		{
			if (m == 0) // Inf
			{
				return (s << 31) | 0x7f800000;
			}
			else // NaN
			{
				return (s << 31) | 0x7f800000 | (m << 13);
			}
		}

		e = e + (127 - 15);
		m = m << 13;

		return (s << 31) | (e << 23) | m;
	}
};

class EDITORTOOLS_API FUniqueVecKey
{
public:
	int Frame;
	FVector Key;
};

class EDITORTOOLS_API FVecKey
{
public:
	FVector Key;
	int Frame;
};

class EDITORTOOLS_API FRotKey
{
public:
	FQuat Quat;
	int Frame;
};

class EDITORTOOLS_API FVisKey
{
public:
	int Frame;
	float Vis;
};

class EDITORTOOLS_API FAniNode
{
public:
	FString NodeName;
	FString ParentNodeName;
	int VertexCount;
	int VertexPointCount;

	FMatrix LocalMatrix;

	TArray<FVector> VertexTable;
	TArray<DWORD> Vertexframe;

	FVector BaseTranslation;
	FQuat BaseRotation;
	FVector BaseScale;

	TArray<FVecKey> PositionKeyTrack;
	TArray<FRotKey> RotationKeyTrack;
	TArray<FVecKey> ScaleKeyTrack;
	TArray<FVisKey> VisibilityKeyTrack;
};

class EDITORTOOLS_API UAnimationFileLoadImpl
{
public:
	virtual bool LoadVertexAni(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version) = 0;
	virtual bool LoadBoneAni(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version) = 0;
	virtual bool LoadVisibilityKey(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version) = 0;
};


class EDITORTOOLS_API UAnimationFileLoadImpl_v6: public UAnimationFileLoadImpl
{
private:
	virtual void LoadVertexAniBoundingBox(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset);

public:
	virtual bool LoadVertexAni(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version) override;
	virtual bool LoadBoneAni(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version)override;
	virtual bool LoadVisibilityKey(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version)override;

};

class EDITORTOOLS_API UAnimationFileLoadImpl_v7 : public UAnimationFileLoadImpl_v6
{
public:
	//~ pass
};


class EDITORTOOLS_API UAnimationFileLoadImpl_v9 : public UAnimationFileLoadImpl_v7
{
public:
	virtual bool LoadVisibilityKey(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version)override;

};

class EDITORTOOLS_API UAnimationFileLoadImpl_v11 : public UAnimationFileLoadImpl_v9
{
public:
	virtual bool LoadBoneAni(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version)override;
	virtual bool LoadVisibilityKey(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version)override;

};

class EDITORTOOLS_API UAnimationFileLoadImpl_v12 : public UAnimationFileLoadImpl_v11
{
public:
	virtual bool LoadBoneAni(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version)override;
};
