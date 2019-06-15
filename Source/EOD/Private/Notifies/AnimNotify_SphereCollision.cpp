// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_SphereCollision.h"
#include "CombatLibrary.h"
#include "EOD.h"
#include "CombatZoneModeBase.h"
#include "CombatManager.h"
#include "EODCharacterBase.h"

#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_SphereCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UWorld* World = MeshComp ? MeshComp->GetWorld() : nullptr;

#if EOD_DRAWING_DEBUG_SHAPES_ENABLED
	if (World)
	{
		FTransform WorldTransform = MeshComp->GetComponentTransform();
		FVector TransformedCenter = WorldTransform.TransformPosition(Center);

		UKismetSystemLibrary::DrawDebugSphere(MeshComp, TransformedCenter, Radius, 12, FLinearColor::White, 5.f, 1.f);
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

		FTransform WorldTransform = MeshComp->GetComponentTransform();
		FVector TransformedCenter = WorldTransform.TransformPosition(Center);

		AActor* Owner = MeshComp->GetOwner();

		FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
		FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(Owner);
		TArray<FHitResult> HitResults;

		// If trace start and end position is same, the trace doesn't hit anything.
		FVector End = TransformedCenter + FVector(0.f, 0.f, 1.f);
		bool bHit = World->SweepMultiByChannel(HitResults, TransformedCenter, End, FQuat::Identity, COLLISION_COMBAT, CollisionShape, Params);
		CombatManager->OnMeleeAttack(Owner, bHit, HitResults, SkillInfo);
	}
}
