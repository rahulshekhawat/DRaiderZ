// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_SphereCollision.h"
#include "CombatLibrary.h"
#include "EODPreprocessors.h"
#include "CombatZoneModeBase.h"
#include "CombatManager.h"
#include "EODCharacterBase.h"

#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_SphereCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UWorld* World = MeshComp ? MeshComp->GetWorld() : nullptr;
	if (World)
	{
		FTransform WorldTransform = MeshComp->GetComponentTransform();
		FVector TransformedCenter = WorldTransform.TransformPosition(Center);

#if EOD_DRAWING_DEBUG_SHAPES_ENABLED
		UKismetSystemLibrary::DrawDebugSphere(MeshComp, TransformedCenter, Radius, 12, FLinearColor::White, 5.f, 1.f);
#endif

		AActor* Owner = MeshComp->GetOwner();
		if (!IsValid(Owner) || Owner->GetNetMode() == NM_Client)
		{
			return;
		}

		// Only process this notify if the current game mode is ACombatZoneModeBase
		ACombatZoneModeBase* CombatZoneGameMode = Cast<ACombatZoneModeBase>(World->GetAuthGameMode());
		ACombatManager* CombatManager = CombatZoneGameMode ? CombatZoneGameMode->GetCombatManager() : nullptr;
		if (!CombatManager)
		{
			return;
		}

		FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
		FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(Owner);
		TArray<FHitResult> HitResults;

		bool bHit = World->SweepMultiByChannel(HitResults, TransformedCenter, TransformedCenter, FQuat::Identity, COLLISION_COMBAT, CollisionShape, Params);
		CombatManager->OnMeleeAttack(Owner, bHit, HitResults);
	}
}
