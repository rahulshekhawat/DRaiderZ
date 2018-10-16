// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_CapsuleCollision.h"
#include "Statics/CombatLibrary.h"
#include "Player/EODCharacterBase.h"
#include "Core/EODPreprocessors.h"
#include "Core/CombatZoneModeBase.h"
#include "Core/CombatManager.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotify_CapsuleCollision::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	// only process this notify if called from server (dedicated or listen)
	if (!MeshComp->GetOwner() || MeshComp->GetOwner()->GetNetMode() == NM_Client)
	{
		return;
	}

	FTransform WorldTransform 	= MeshComp->GetComponentTransform();
	FVector TransformedCenter 	= WorldTransform.TransformPosition(Center);
	FQuat TransformedQuat 		= WorldTransform.TransformRotation(Rotation.Quaternion());

#if WITH_EDITOR // draw debug shapes only if inside editor
	UKismetSystemLibrary::DrawDebugCapsule(MeshComp, TransformedCenter, HalfHeight, Radius, TransformedQuat.Rotator(), FLinearColor::White, 5.f, 1.f);
#endif // WITH_EDITOR

	// Only process this notify if the current game mode is ACombatZoneModeBase
	ACombatZoneModeBase* CombatZoneGameMode = Cast<ACombatZoneModeBase>(MeshComp->GetWorld()->GetAuthGameMode());
	if (!CombatZoneGameMode)
	{
		return;
	}

	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(Radius, HalfHeight);
	FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(MeshComp->GetOwner());
	TArray<FHitResult> HitResults;
	
	bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(HitResults, TransformedCenter, TransformedCenter, TransformedQuat, COLLISION_COMBAT, CollisionShape, Params);
	CombatZoneGameMode->GetCombatManager()->OnMeleeAttack(MeshComp->GetOwner(), bHit, HitResults);

}
