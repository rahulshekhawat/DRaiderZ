// Fill out your copyright notice in the Description page of Project Settings.

#include "AniLoader.h"
#include "EOD.h"
#include "EluLibrary.h"
#include "RaiderzXmlUtilities.h"

void UAnimationFileLoadImpl_v6::LoadVertexAniBoundingBox(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset)
{
	//~ pass
}

bool UAnimationFileLoadImpl_v6::LoadVertexAni(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version)
{
	if (!URaiderzXmlUtilities::ReadStringFromBinaryData(Node->NodeName, BinaryData, Offset))
	{
		return false;
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Node->VertexCount, sizeof(Node->VertexCount), BinaryData, Offset))
	{
		return false;
	}

	int VertexPointCount;
	URaiderzXmlUtilities::WriteBinaryDataToBuffer(&VertexPointCount, sizeof(VertexPointCount), BinaryData, Offset);
	Node->VertexPointCount = VertexPointCount;

	for (int i = 0; i < Node->VertexCount; i++)
	{
		DWORD Val;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Val, sizeof(Val), BinaryData, Offset))
		{
			return false;
		}
		Node->Vertexframe.Add(Val);
	}

	for (int i = 0; i < Node->VertexCount; i++)
	{
		for (int j = 0; j < VertexPointCount; j++)
		{
			FVector TempVec;
			if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&TempVec, sizeof(TempVec), BinaryData, Offset))
			{
				return false;
			}
			Node->VertexTable.Add(TempVec);
		}
	}

	LoadVertexAniBoundingBox(Node, BinaryData, Offset);

	return true;
}

bool UAnimationFileLoadImpl_v6::LoadBoneAni(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version)
{
	if (!URaiderzXmlUtilities::ReadStringFromBinaryData(Node->NodeName, BinaryData, Offset))
	{
		return false;
	}

	if (Version >= EXPORTER_ANI_VER6)
	{
		if (!URaiderzXmlUtilities::ReadStringFromBinaryData(Node->ParentNodeName, BinaryData, Offset))
		{
			return false;
		}
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Node->LocalMatrix, sizeof(Node->LocalMatrix), BinaryData, Offset))
	{
		return false;
	}

	int pos_key_num = 0;
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&pos_key_num, sizeof(pos_key_num), BinaryData, Offset))
	{
		return false;
	}

	for (int i = 0; i < pos_key_num; i++)
	{
		FVecKey PosKey;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&PosKey, sizeof(PosKey), BinaryData, Offset))
		{
			return false;
		}
		Node->PositionKeyTrack.Add(PosKey);
	}

	if (Node->PositionKeyTrack.Num() > 0)
	{
		FVecKey PosKey = Node->PositionKeyTrack[ Node->PositionKeyTrack.Num() -1];
		Node->PositionKeyTrack.Add(PosKey);
	}

	int rot_key_num = 0;
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&rot_key_num, sizeof(rot_key_num), BinaryData, Offset))
	{
		return false;
	}

	for (int i = 0; i < rot_key_num; i++)
	{
		FRotKey RotKey;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&RotKey, sizeof(RotKey), BinaryData, Offset))
		{
			return false;
		}
		Node->RotationKeyTrack.Add(RotKey);
	}

	if (Node->RotationKeyTrack.Num() > 0)
	{
		FRotKey RotKey = Node->RotationKeyTrack[Node->RotationKeyTrack.Num() - 1];
		Node->RotationKeyTrack.Add(RotKey);
	}

	if (Version >= EXPORTER_ANI_VER5)
	{
		int scale_key_num = 0;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&scale_key_num, sizeof(scale_key_num), BinaryData, Offset))
		{
			return false;
		}

		for (int i = 0; i < scale_key_num; i++)
		{
			FVecKey ScaleKey;
			if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&ScaleKey, sizeof(ScaleKey), BinaryData, Offset))
			{
				return false;
			}
			Node->ScaleKeyTrack.Add(ScaleKey);
		}
	}

	return true;
}

bool UAnimationFileLoadImpl_v6::LoadVisibilityKey(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version)
{
	if (Version >= EXPORTER_ANI_VER5)
	{
		int vis_key_num = 0;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&vis_key_num, sizeof(vis_key_num), BinaryData, Offset))
		{
			return false;
		}

		for (int i = 0; i < vis_key_num; i++)
		{
			FVisKey VisKey;
			if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&VisKey, sizeof(VisKey), BinaryData, Offset))
			{
				return false;
			}
			Node->VisibilityKeyTrack.Add(VisKey);
		}

		for (FVisKey& VisKey : Node->VisibilityKeyTrack)
		{
			if (VisKey.Vis < 0.f)
			{
				VisKey.Vis = 0.f;
			}
		}
	}
	else
	{
		int vis_key_num = 0;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&vis_key_num, sizeof(vis_key_num), BinaryData, Offset))
		{
			return false;
		}

		struct FOldVisKey
		{
			float Vis;
			int Frame;
		} OldKey;

		for (int i = 0; i < vis_key_num; i++)
		{
			if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&OldKey, sizeof(OldKey), BinaryData, Offset))
			{
				return false;
			}

			FVisKey VisKey;
			VisKey.Frame = OldKey.Frame;
			VisKey.Vis = OldKey.Vis;

			Node->VisibilityKeyTrack.Add(VisKey);
		}

		for (FVisKey& VisKey : Node->VisibilityKeyTrack)
		{
			if (VisKey.Vis < 0.f)
			{
				VisKey.Vis = 0.f;
			}
		}
	}

	return true;
}

bool UAnimationFileLoadImpl_v9::LoadVisibilityKey(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version)
{
	int vis_key_num = 0;
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&vis_key_num, sizeof(vis_key_num), BinaryData, Offset))
	{
		return false;
	}

	for (int i = 0; i < vis_key_num; i++)
	{
		FVisKey VisKey;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&VisKey, sizeof(VisKey), BinaryData, Offset))
		{
			return false;
		}
		Node->VisibilityKeyTrack.Add(VisKey);
	}

	for (FVisKey& VisKey : Node->VisibilityKeyTrack)
	{
		if (VisKey.Vis < 0.f)
		{
			VisKey.Vis = 0.f;
		}
	}

	return true;
}

bool UAnimationFileLoadImpl_v11::LoadBoneAni(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version)
{
	if (!URaiderzXmlUtilities::ReadStringFromBinaryData(Node->NodeName, BinaryData, Offset))
	{
		return false;
	}

	if (!URaiderzXmlUtilities::ReadStringFromBinaryData(Node->ParentNodeName, BinaryData, Offset))
	{
		return false;
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Node->LocalMatrix, sizeof(Node->LocalMatrix), BinaryData, Offset))
	{
		return false;
	}

	FAnimType AnimType1;
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&AnimType1, sizeof(AnimType1), BinaryData, Offset))
	{
		return false;
	}

	for (int i = 0; i < AnimType1.Count; i++)
	{
		FVecKey PosKey;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&PosKey, sizeof(PosKey), BinaryData, Offset))
		{
			return false;
		}
		Node->PositionKeyTrack.Add(PosKey);
	}

	if (Node->PositionKeyTrack.Num() > 0)
	{
		FVecKey PosKey = Node->PositionKeyTrack[Node->PositionKeyTrack.Num() - 1];
		Node->PositionKeyTrack.Add(PosKey);
	}

	FAnimType AnimType2;
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&AnimType2, sizeof(AnimType2), BinaryData, Offset))
	{
		return false;
	}

	for (int i = 0; i < AnimType2.Count; i++)
	{
		FRotKey RotKey;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&RotKey, sizeof(RotKey), BinaryData, Offset))
		{
			return false;
		}
		Node->RotationKeyTrack.Add(RotKey);
	}

	if (Node->RotationKeyTrack.Num() > 0)
	{
		FRotKey RotKey = Node->RotationKeyTrack[Node->RotationKeyTrack.Num() - 1];
		Node->RotationKeyTrack.Add(RotKey);
	}

	FAnimType AnimType3;
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&AnimType3, sizeof(AnimType3), BinaryData, Offset))
	{
		return false;
	}

	for (int i = 0; i < AnimType3.Count; i++)
	{
		FVecKey ScaleKey;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&ScaleKey, sizeof(ScaleKey), BinaryData, Offset))
		{
			return false;
		}
		Node->ScaleKeyTrack.Add(ScaleKey);
	}

	return true;
}

bool UAnimationFileLoadImpl_v11::LoadVisibilityKey(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version)
{
	FAnimType AnimType;
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&AnimType, sizeof(AnimType), BinaryData, Offset))
	{
		return false;
	}

	for (int i = 0; i < AnimType.Count; i++)
	{
		FVisKey VisKey;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&VisKey, sizeof(VisKey), BinaryData, Offset))
		{
			return false;
		}
		Node->VisibilityKeyTrack.Add(VisKey);
	}

	for (FVisKey& VisKey : Node->VisibilityKeyTrack)
	{
		if (VisKey.Vis < 0.f)
		{
			VisKey.Vis = 0.f;
		}
	}

	return true;
}

bool UAnimationFileLoadImpl_v12::LoadBoneAni(TSharedPtr<FAniNode> Node, TArray<uint8>& BinaryData, UINT& Offset, DWORD Version)
{
	if (!URaiderzXmlUtilities::ReadStringFromBinaryData(Node->NodeName, BinaryData, Offset))
	{
		return false;
	}

	int Unk = 0;
	FVector BaseTrans;

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Node->BaseTranslation, sizeof(Node->BaseTranslation), BinaryData, Offset))
	{
		return false;
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Node->BaseRotation, sizeof(Node->BaseRotation), BinaryData, Offset))
	{
		return false;
	}

	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&Node->BaseScale, sizeof(Node->BaseScale), BinaryData, Offset))
	{
		return false;
	}

	FAnimType AnimType1;
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&AnimType1, sizeof(AnimType1), BinaryData, Offset))
	{
		return false;
	}
	
	if (AnimType1.Count > 0)
	{
		if (AnimType1.CountType == 10)
		{
			TArray<FShortVecKey> ShortPosKeys;
			for (int i = 0; i < AnimType1.Count; i++)
			{
				FShortVecKey ShortVecKey;
				if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&ShortVecKey, sizeof(ShortVecKey), BinaryData, Offset))
				{
					return false;
				}
				ShortPosKeys.Add(ShortVecKey);
			}

			for (const FShortVecKey& ShortVecKey : ShortPosKeys)
			{
				FVecKey PosKey;

				PosKey.Frame = ShortVecKey.Frame;
				PosKey.Key.X = ShortVecKey.X();
				PosKey.Key.Y = ShortVecKey.Y();
				PosKey.Key.Z = ShortVecKey.Z();


				Node->PositionKeyTrack.Add(PosKey);
			}

			if (Node->PositionKeyTrack.Num() > 0)
			{
				FVecKey PosKey = Node->PositionKeyTrack[Node->PositionKeyTrack.Num() - 1];
				Node->PositionKeyTrack.Add(PosKey);
			}
		}
		else if (AnimType1.CountType == 16)
		{
			for (int i = 0; i < AnimType1.Count; i++)
			{
				FVecKey PosKey;
				if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&PosKey, sizeof(PosKey), BinaryData, Offset))
				{
					return false;
				}
				Node->PositionKeyTrack.Add(PosKey);
			}

			if (Node->PositionKeyTrack.Num() > 0)
			{
				FVecKey PosKey = Node->PositionKeyTrack[Node->PositionKeyTrack.Num() - 1];
				Node->PositionKeyTrack.Add(PosKey);
			}
		}
		else
		{
			PrintError(TEXT("Invalid elu attempted to be loaded"));
			return false;
		}
	}

	FAnimType AnimType2;
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&AnimType2, sizeof(AnimType2), BinaryData, Offset))
	{
		return false;
	}

	if (AnimType2.Count > 0)
	{
		if (AnimType2.CountType == 10)
		{
			TArray<FShortVecKey> ShortVecKeys;
			for (int i = 0; i < AnimType2.Count; i++)
			{
				FShortVecKey ShortVecKey;
				if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&ShortVecKey, sizeof(ShortVecKey), BinaryData, Offset))
				{
					return false;
				}
				ShortVecKeys.Add(ShortVecKey);
			}

			for (const FShortVecKey& ShortVecKey : ShortVecKeys)
			{
				FRotKey RotKey;

				RotKey.Frame = ShortVecKey.Frame;
				RotKey.Quat.X = ShortVecKey.X();
				RotKey.Quat.Y = ShortVecKey.Y();
				RotKey.Quat.Z = ShortVecKey.Z();

				float fTol = RotKey.Quat.X * RotKey.Quat.X + RotKey.Quat.Y * RotKey.Quat.Y + RotKey.Quat.Z * RotKey.Quat.Z;
				if (fTol <= 1.f)
				{
					float sub1 = 1.0f - fTol;
					float fSqrt = FMath::Sqrt(sub1);
					RotKey.Quat.W = fSqrt;
				}
				else
				{
					RotKey.Quat.W = 0.f;
				}

				Node->RotationKeyTrack.Add(RotKey);
			}

			if (Node->RotationKeyTrack.Num() > 0)
			{
				FRotKey RotKey = Node->RotationKeyTrack[Node->RotationKeyTrack.Num() - 1];
				Node->RotationKeyTrack.Add(RotKey);
			}
		}
		else if (AnimType2.CountType == 16)
		{
			TArray<FUniqueVecKey> UniVecKeys;
			for (int i = 0; i < AnimType2.Count; i++)
			{
				FUniqueVecKey uvecKey;
				if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&uvecKey, sizeof(uvecKey), BinaryData, Offset))
				{
					return false;
				}
				UniVecKeys.Add(uvecKey);
			}

			for (const FUniqueVecKey& uvecKey : UniVecKeys)
			{
				FRotKey RotKey;
				RotKey.Frame = uvecKey.Frame;
				RotKey.Quat.X = uvecKey.Key.X;
				RotKey.Quat.Y = uvecKey.Key.Y;
				RotKey.Quat.Z = uvecKey.Key.Z;

				float fTol = RotKey.Quat.X * RotKey.Quat.X + RotKey.Quat.Y * RotKey.Quat.Y + RotKey.Quat.Z * RotKey.Quat.Z;
				if (fTol <= 1.f)
				{
					float sub1 = 1.0f - fTol;
					float fSqrt = FMath::Sqrt(sub1);
					RotKey.Quat.W = fSqrt;
				}
				else
				{
					RotKey.Quat.W = 0.f;
				}

				Node->RotationKeyTrack.Add(RotKey);
			}

			if (Node->RotationKeyTrack.Num() > 0)
			{
				FRotKey RotKey = Node->RotationKeyTrack[Node->RotationKeyTrack.Num() - 1];
				Node->RotationKeyTrack.Add(RotKey);
			}
		}
		else if (AnimType2.CountType == 20)
		{
			for (int i = 0; i < AnimType2.Count; i++)
			{
				FRotKey RotKey;
				if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&RotKey, sizeof(RotKey), BinaryData, Offset))
				{
					return false;
				}
				Node->RotationKeyTrack.Add(RotKey);
			}

			if (Node->RotationKeyTrack.Num() > 0)
			{
				FRotKey RotKey = Node->RotationKeyTrack[Node->RotationKeyTrack.Num() - 1];
				Node->RotationKeyTrack.Add(RotKey);
			}
		}
	}

	FAnimType AnimType3;
	if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&AnimType3, sizeof(AnimType3), BinaryData, Offset))
	{
		return false;
	}

	for (int i = 0; i < AnimType3.Count; i++)
	{
		FVecKey ScaleKey;
		if (!URaiderzXmlUtilities::WriteBinaryDataToBuffer(&ScaleKey, sizeof(ScaleKey), BinaryData, Offset))
		{
			return false;
		}
		Node->ScaleKeyTrack.Add(ScaleKey);
	}

	return true;
}
