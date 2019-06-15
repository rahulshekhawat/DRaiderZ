// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_RaidCollision.h"
#include "EOD.h"
#include "CombatLibrary.h"
#include "EODCharacterBase.h"
#include "CombatZoneModeBase.h"
#include "CombatManager.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_RaidCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UWorld* World = MeshComp ? MeshComp->GetWorld() : nullptr;

#if EOD_DRAWING_DEBUG_SHAPES_ENABLED
	if (World)
	{
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

			UKismetSystemLibrary::DrawDebugCapsule(MeshComp, TransformedCenter, HalfHeightVector.Size(), Capsule.Radius, TransformedRotation, FLinearColor::White, 5.f, 1.f);
		}
	}
#endif

	if (World)
	{
		// Only process this notify if the current game mode is ACombatZoneModeBase
		ACombatZoneModeBase* CombatZoneGameMode = Cast<ACombatZoneModeBase>(World->GetAuthGameMode());
		ACombatManager* CombatManager = CombatZoneGameMode ? CombatZoneGameMode->GetCombatManager() : nullptr;
		if (!CombatManager)
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

			AActor* Owner = MeshComp->GetOwner();

			FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(Capsule.Radius, HalfHeightVector.Size());
			FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(Owner);
			TArray<FHitResult> HitResults;

			// If trace start and end position is same, the trace doesn't hit anything.
			FVector End = TransformedCenter + FVector(0.f, 0.f, 1.f);
			bool bHit = World->SweepMultiByChannel(HitResults, TransformedCenter, End, TransformedRotation.Quaternion(), COLLISION_COMBAT, CollisionShape, Params);
			CombatManager->OnMeleeAttack(Owner, bHit, HitResults, SkillInfo);
		}
	}
}
