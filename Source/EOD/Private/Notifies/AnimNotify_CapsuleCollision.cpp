// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_CapsuleCollision.h"
#include "CombatLibrary.h"
#include "EODCharacterBase.h"
#include "CombatZoneModeBase.h"
#include "CombatManager.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_CapsuleCollision::InitializeFromRaidCapsules(const TArray<FRaidCapsule>& RaidCapsules)
{
	CollisionCapsules.Empty();
	
	for (const FRaidCapsule& RaidCapsule : RaidCapsules)
	{
		FEODCapsule EODCapsule;
		EODCapsule.Radius = RaidCapsule.Radius;

		FVector CorrectedBottom = RaidCapsule.Bottom.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));
		FVector CorrectedTop = RaidCapsule.Top.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));
		FVector HalfHeightVector = (CorrectedTop - CorrectedBottom) / 2;

		EODCapsule.HalfHeight = HalfHeightVector.Size();
		EODCapsule.Center = CorrectedBottom + HalfHeightVector;
		EODCapsule.Rotation = FRotationMatrix::MakeFromZ(HalfHeightVector).Rotator();

		CollisionCapsules.Add(EODCapsule);
	}
}

void UAnimNotify_CapsuleCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UWorld* World = MeshComp ? MeshComp->GetWorld() : nullptr;
	if (!World)
	{
		return;
	}

#if EOD_DRAWING_DEBUG_SHAPES_ENABLED
	FTransform DrawWorldTransform = MeshComp->GetComponentTransform();
	for (const FEODCapsule& EODCapsule : CollisionCapsules)
	{
		FVector TransformedCenter = DrawWorldTransform.TransformPosition(EODCapsule.Center);
		FQuat TransformedQuat = DrawWorldTransform.TransformRotation(EODCapsule.Rotation.Quaternion());
		UKismetSystemLibrary::DrawDebugCapsule(MeshComp, TransformedCenter, EODCapsule.HalfHeight, EODCapsule.Radius, TransformedQuat.Rotator(), FLinearColor::White, 5.f, 1.f);
	}
#endif

	// Only process this notify if the current game mode is ACombatZoneModeBase
	ACombatZoneModeBase* CombatZoneGameMode = Cast<ACombatZoneModeBase>(World->GetAuthGameMode());
	ACombatManager* CombatManager = CombatZoneGameMode ? CombatZoneGameMode->GetCombatManager() : nullptr;
	if (!CombatManager)
	{
		return;
	}

	TArray<FHitResult> AllHitResults;
	bool bAnyHit = false;
	AActor* Owner = MeshComp->GetOwner();
	FTransform WorldTransform = MeshComp->GetComponentTransform();
	FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(Owner);
	for (const FEODCapsule& EODCapsule : CollisionCapsules)
	{
		FVector TransformedCenter = WorldTransform.TransformPosition(EODCapsule.Center);
		FQuat TransformedQuat = WorldTransform.TransformRotation(EODCapsule.Rotation.Quaternion());

		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(EODCapsule.Radius, EODCapsule.HalfHeight);
		TArray<FHitResult> HitResults;

		// If trace start and end position is same, the trace doesn't hit anything.
		FVector End = TransformedCenter + FVector(0.f, 0.f, 1.f);
		bool bHit = World->SweepMultiByChannel(HitResults, TransformedCenter, End, TransformedQuat, COLLISION_COMBAT, CollisionShape, Params);
		bAnyHit = bAnyHit || bHit;
	}

	CombatManager->OnMeleeAttack(Owner, bAnyHit, AllHitResults, SkillInfo);

}
