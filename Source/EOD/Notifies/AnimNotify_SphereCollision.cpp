// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_SphereCollision.h"
#include "Statics/CombatLibrary.h"
#include "Core/EODPreprocessors.h"
#include "Core/CombatZoneModeBase.h"
#include "Core/CombatManager.h"
#include "Player/EODCharacterBase.h"


#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_SphereCollision::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	// only process this notify if called from server (dedicated or listen)
	if (!MeshComp->GetOwner() || MeshComp->GetOwner()->GetNetMode() == NM_Client)
	{
		return;
	}

	FTransform WorldTransform = MeshComp->GetComponentTransform();
	FVector TransformedCenter = WorldTransform.TransformPosition(Center);
	
#if WITH_EDITOR
	UKismetSystemLibrary::DrawDebugSphere(MeshComp, TransformedCenter, Radius, 12, FLinearColor::White, 5.f, 1.f);
#endif // WITH_EDITOR

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
