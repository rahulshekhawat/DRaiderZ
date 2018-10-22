// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_RaidCollision.h"
#include "Core/EODPreprocessors.h"
#include "Statics/CombatLibrary.h"
#include "Player/EODCharacterBase.h"
#include "Core/CombatZoneModeBase.h"
#include "Core/CombatManager.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_RaidCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// only process this notify if called from server (dedicated or listen)
	if (!MeshComp->GetOwner() || MeshComp->GetOwner()->GetNetMode() == NM_Client)
	{
		return;
	}

	for (FRaidCapsule& Capsule : CollisionCapsules)
	{
		FTransform WorldTransform = MeshComp->GetComponentTransform();

		FVector CorrectedBottom = Capsule.Bottom.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));
		FVector CorrectedTop = Capsule.Top.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));
		FVector HalfHeightVector = (CorrectedTop - CorrectedBottom) / 2;
		FVector NormalizedHeightVector = HalfHeightVector.GetSafeNormal();
		FVector Center = CorrectedBottom + HalfHeightVector;
		FRotator CapsuleRotation = FRotationMatrix::MakeFromZ(HalfHeightVector).Rotator();

		// Transformation from object space to world space
		FVector TransformedCenter = WorldTransform.TransformPosition(Center);
		FRotator TransformedRotation = WorldTransform.TransformRotation(CapsuleRotation.Quaternion()).Rotator();

#if DEBUG_SHAPES_ENABLED
		UKismetSystemLibrary::DrawDebugCapsule(MeshComp, TransformedCenter, HalfHeightVector.Size(), Capsule.Radius, TransformedRotation, FLinearColor::White, 5.f, 1.f);
#endif // DEBUG_SHAPES_ENABLED

		// Do collisions only if the current game mode is ACombatZoneModeBase
		ACombatZoneModeBase* CombatZoneGameMode = Cast<ACombatZoneModeBase>(MeshComp->GetWorld()->GetAuthGameMode());
		if (!CombatZoneGameMode)
		{
			return;
		}
		
		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(Capsule.Radius, HalfHeightVector.Size());
		FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(MeshComp->GetOwner());
		TArray<FHitResult> HitResults;

		bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(HitResults, TransformedCenter, TransformedCenter, TransformedRotation.Quaternion(), COLLISION_COMBAT, CollisionShape, Params);
		CombatZoneGameMode->GetCombatManager()->OnMeleeAttack(MeshComp->GetOwner(), bHit, HitResults);

	}

}
