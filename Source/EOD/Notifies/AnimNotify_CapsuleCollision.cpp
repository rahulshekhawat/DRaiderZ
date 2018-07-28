// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_CapsuleCollision.h"
#include "Statics/CombatLibrary.h"
#include "Player/BaseCharacter.h"
#include "Core/EODPreprocessors.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotify_CapsuleCollision::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	// only call from server
	if (!MeshComp->GetOwner() || MeshComp->GetOwner()->GetNetMode() == NM_Client)
	{
		return;
	}

	FTransform WorldTransform = MeshComp->GetComponentTransform();

	FVector TransformedCenter = WorldTransform.TransformPosition(Center);
	FQuat TransformedQuat = WorldTransform.TransformRotation(Rotation.Quaternion());

#if WITH_EDITOR
	
	UKismetSystemLibrary::DrawDebugCapsule(MeshComp, TransformedCenter, HalfHeight, Radius, TransformedQuat.Rotator(), FLinearColor::White, 5.f, 1.f);

#endif // WITH_EDITOR

	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(Radius, HalfHeight);
	TArray<FHitResult> HitResults;
	
	FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(MeshComp->GetOwner());
	
	bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(HitResults, TransformedCenter, TransformedCenter, TransformedQuat, COLLISION_COMBAT, CollisionShape, Params);
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (BaseCharacter)
	{
		UCombatLibrary::HandleCombatCollision(BaseCharacter, Animation, HitResults, bHit);
	}
	else
	{
		UCombatLibrary::HandleCombatCollision(MeshComp->GetOwner(), Animation, HitResults, bHit);
	}

}
