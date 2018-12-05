// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_BoxCollision.h"
#include "EOD/Statics/CombatLibrary.h"
#include "EOD/Player/EODCharacterBase.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/Core/CombatZoneModeBase.h"
#include "EOD/Core/CombatManager.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_BoxCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// only call from server
	if (!MeshComp->GetOwner() || MeshComp->GetOwner()->GetNetMode() == NM_Client)
	{
		return;
	}
	
	FTransform WorldTransform = MeshComp->GetComponentTransform();
	FVector TransformedCenter = WorldTransform.TransformPosition(Center);
	FQuat TransformedQuat = WorldTransform.TransformRotation(Rotation.Quaternion());
	
#if DEBUG_SHAPES_ENABLED
	UKismetSystemLibrary::DrawDebugBox(MeshComp, TransformedCenter, BoxHalfExtent, FLinearColor::White, TransformedQuat.Rotator(), 5.f, 1.f);
#endif // DEBUG_SHAPES_ENABLED

	// Only process this notify if the current game mode is ACombatZoneModeBase
	ACombatZoneModeBase* CombatZoneGameMode = Cast<ACombatZoneModeBase>(MeshComp->GetWorld()->GetAuthGameMode());
	if (!CombatZoneGameMode)
	{
		return;
	}
	
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(BoxHalfExtent);
	FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(MeshComp->GetOwner());
	TArray<FHitResult> HitResults;
		
	bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(HitResults, TransformedCenter, TransformedCenter, TransformedQuat, COLLISION_COMBAT, CollisionShape, Params);
	CombatZoneGameMode->GetCombatManager()->OnMeleeAttack(MeshComp->GetOwner(), bHit, HitResults);

}
