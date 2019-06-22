// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SoundParser.h"

#include "Engine/SkeletalMesh.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimSequence.h"

USoundParser::USoundParser(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USoundParser::ImportSoundForSkeletalMesh(USkeletalMesh* Mesh)
{
	if (Mesh == nullptr || Mesh->Skeleton == nullptr)
	{
		return;
	}

	USkeleton* BaseSkeleton = Mesh->Skeleton;
	// Mesh->Skeleton.Animation


}
