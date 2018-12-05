// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_JumpBase.h"
#include "EOD/Core/EODPreprocessors.h"

#include "CollisionQueryParams.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

void UAnimNotify_JumpBase::GetJumpHitResults(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, FHitResult& OutHitResults)
{
	FVector TraceStart = MeshComp->GetOwner()->GetActorLocation();
	FVector TraceEnd = TraceStart + FVector(0.f, 0.f, -100.f);

	FCollisionQueryParams Params;
	Params.bTraceComplex = false;
	Params.bReturnPhysicalMaterial = true;
	Params.bReturnFaceIndex = false;
	Params.bTraceAsyncScene = true;
	Params.AddIgnoredActor(MeshComp->GetOwner());
	MeshComp->GetWorld()->LineTraceSingleByChannel(OutHitResults, TraceStart, TraceEnd, ECC_Visibility, Params);
}

void UAnimNotify_JumpBase::PlayJumpSound(const FHitResult& HitResult)
{
	USoundBase* SoundAssetToPlay = nullptr;
	UPhysicalMaterial* PhysMat = HitResult.PhysMaterial.Get();

	if (!PhysMat)
	{
		if (bJumpStart)
		{
			SoundAssetToPlay = DefaultJumpStartSound;
		}
		else
		{
			SoundAssetToPlay = DefaultJumpEndSound;
		}
	}
	else if (PhysMat && bJumpStart)
	{
		if (PhysMat->SurfaceType == SURFACETYPE_WOOD)
		{
			SoundAssetToPlay = WoodJumpStartSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_WATER)
		{
			SoundAssetToPlay = WaterJumpStartSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_SNOW)
		{
			SoundAssetToPlay = SnowJumpStartSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_SAND)
		{
			SoundAssetToPlay = SandJumpStartSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_FOLIAGE)
		{
			SoundAssetToPlay = FoliageJumpStartSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_METAL)
		{
			SoundAssetToPlay = MetalJumpStartSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_DIRT)
		{
			SoundAssetToPlay = DirtJumpStartSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_CONCRETE)
		{
			SoundAssetToPlay = ConcreteJumpStartSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_STONE)
		{
			SoundAssetToPlay = StoneJumpStartSound;
		}
		else
		{
			SoundAssetToPlay = DefaultJumpStartSound;
		}
	}
	else if (PhysMat && !bJumpStart)
	{
		if (PhysMat->SurfaceType == SURFACETYPE_WOOD)
		{
			SoundAssetToPlay = WoodJumpEndSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_WATER)
		{
			SoundAssetToPlay = WaterJumpEndSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_SNOW)
		{
			SoundAssetToPlay = SnowJumpEndSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_SAND)
		{
			SoundAssetToPlay = SandJumpEndSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_FOLIAGE)
		{
			SoundAssetToPlay = FoliageJumpEndSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_METAL)
		{
			SoundAssetToPlay = MetalJumpEndSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_DIRT)
		{
			SoundAssetToPlay = DirtJumpEndSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_CONCRETE)
		{
			SoundAssetToPlay = ConcreteJumpEndSound;
		}
		else if (PhysMat->SurfaceType == SURFACETYPE_STONE)
		{
			SoundAssetToPlay = StoneJumpEndSound;
		}
		else
		{
			SoundAssetToPlay = DefaultJumpEndSound;
		}
	}

	if (SoundAssetToPlay)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, SoundAssetToPlay, HitResult.ImpactPoint, FRotator::ZeroRotator, 1.f, 1.f, 0.f, SoundAttenuation);
	}
}
