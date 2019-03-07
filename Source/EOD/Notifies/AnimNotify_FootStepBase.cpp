// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_FootStepBase.h"
#include "EOD/Core/EODPreprocessors.h"

#include "CollisionQueryParams.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

/*

////////
////////

FOR SOME EXTREMELY WEIRD REASON CALLING NOTIFY FROM C++ CLASS DIDN'T PLAY THE SOUND IN PLAYFOOTSTEPSOUND,
EVEN THOUGH SoundAssetToPlay WAS NOT NULL.
GetFootsetpHitResults IS NOTHING BUT A REROUTE FOR THE SAME CODE THROUGH BLUEPRINT'S NOTIFY.
THIS WAY THE NOTIFY WORKS AS EXPECTED.

////////
////////

void UAnimNotify_FootStepBase::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	FVector TraceStart = MeshComp->GetOwner()->GetActorLocation();
	FVector TraceEnd = TraceStart + FVector(0.f, 0.f, -95.f);

	FHitResult LineHitResult;
	FCollisionQueryParams Params;
	Params.bTraceComplex = false;
	Params.bReturnPhysicalMaterial = true;
	Params.bReturnFaceIndex = false;
	Params.bTraceAsyncScene = true;
	Params.AddIgnoredActor(MeshComp->GetOwner());
	MeshComp->GetWorld()->LineTraceSingleByChannel(LineHitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
	PlayFootstepSound(LineHitResult);
}
*/

void UAnimNotify_FootStepBase::GetFootstepHitResults(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, FHitResult& OutHitResults)
{
	FVector TraceStart = MeshComp->GetOwner()->GetActorLocation();
	FVector TraceEnd = TraceStart + FVector(0.f, 0.f, -95.f);

	FCollisionQueryParams Params;
	Params.bTraceComplex = false;
	Params.bReturnPhysicalMaterial = true;
	Params.bReturnFaceIndex = false;
	// Params.bTraceAsyncScene = true;
	Params.AddIgnoredActor(MeshComp->GetOwner());
	MeshComp->GetWorld()->LineTraceSingleByChannel(OutHitResults, TraceStart, TraceEnd, ECC_Visibility, Params);
}

void UAnimNotify_FootStepBase::PlayFootstepSound(const FHitResult& HitResult)
{
	USoundBase* SoundAssetToPlay = nullptr;
	UPhysicalMaterial* PhysMat = HitResult.PhysMaterial.Get();

	if (!PhysMat)
	{
		if (bLeftFoot)
		{
			SoundAssetToPlay = DefaultLeftFootSound;
		}
		else
		{
			SoundAssetToPlay = DefaultRightFootSound;
		}
	}
	else if (PhysMat && bLeftFoot)
	{
		if (PhysMat->SurfaceType == SURFACETYPE_WOOD)
		{
			SoundAssetToPlay = WoodLeftFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_WATER)
		{
			SoundAssetToPlay = WaterLeftFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_SNOW)
		{
			SoundAssetToPlay = SnowLeftFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_SAND)
		{
			SoundAssetToPlay = SandLeftFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_FOLIAGE)
		{
			SoundAssetToPlay = FoliageLeftFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_METAL)
		{
			SoundAssetToPlay = MetalLeftFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_DIRT)
		{
			SoundAssetToPlay = DirtLeftFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_CONCRETE)
		{
			SoundAssetToPlay = ConcreteLeftFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_STONE)
		{
			SoundAssetToPlay = StoneLeftFootSound;
		}
		else
		{
			SoundAssetToPlay = DefaultLeftFootSound;
		}
	}
	else if (PhysMat && !bLeftFoot)
	{
		if (PhysMat->SurfaceType == SURFACETYPE_WOOD)
		{
			SoundAssetToPlay = WoodRightFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_WATER)
		{
			SoundAssetToPlay = WaterRightFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_SNOW)
		{
			SoundAssetToPlay = SnowRightFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_SAND)
		{
			SoundAssetToPlay = SandRightFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_FOLIAGE)
		{
			SoundAssetToPlay = FoliageRightFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_METAL)
		{
			SoundAssetToPlay = MetalRightFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_DIRT)
		{
			SoundAssetToPlay = DirtRightFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_CONCRETE)
		{
			SoundAssetToPlay = ConcreteRightFootSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_STONE)
		{
			SoundAssetToPlay = StoneRightFootSound;
		}
		else
		{
			SoundAssetToPlay = DefaultRightFootSound;
		}
	}

	if (SoundAssetToPlay)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, SoundAssetToPlay, HitResult.ImpactPoint, FRotator::ZeroRotator, 1.f, 1.f, 0.f, SoundAttenuation);
	}
}
