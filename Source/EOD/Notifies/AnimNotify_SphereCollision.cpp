// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_SphereCollision.h"
#include "EOD/Statics/CombatLibrary.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/Core/CombatZoneModeBase.h"
#include "EOD/Core/CombatManager.h"
#include "EOD/Player/EODCharacterBase.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_SphereCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// only process this notify if called from server (dedicated or listen)
	if (!MeshComp->GetOwner() || MeshComp->GetOwner()->GetNetMode() == NM_Client)
	{
		return;
	}

	FTransform WorldTransform = MeshComp->GetComponentTransform();
	FVector TransformedCenter = WorldTransform.TransformPosition(Center);
	
#if DEBUG_SHAPES_ENABLED
	UKismetSystemLibrary::DrawDebugSphere(MeshComp, TransformedCenter, Radius, 12, FLinearColor::White, 5.f, 1.f);
#endif // DEBUG_SHAPES_ENABLED

	// Only process this notify if the current game mode is ACombatZoneModeBase
	ACombatZoneModeBase* CombatZoneGameMode = Cast<ACombatZoneModeBase>(MeshComp->GetWorld()->GetAuthGameMode());
	if (!CombatZoneGameMode)
	{
		return;
	}

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
	FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(MeshComp->GetOwner());
	TArray<FHitResult> HitResults;
	
	bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(HitResults, TransformedCenter, TransformedCenter, FQuat::Identity, COLLISION_COMBAT, CollisionShape, Params);
	CombatZoneGameMode->GetCombatManager()->OnMeleeAttack(MeshComp->GetOwner(), bHit, HitResults);
}
