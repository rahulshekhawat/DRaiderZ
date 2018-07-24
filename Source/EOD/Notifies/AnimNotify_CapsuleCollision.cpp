// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_CapsuleCollision.h"
#include "Statics/CombatLibrary.h"
#include "Player/BaseCharacter.h"
#include "Core/EODPreprocessors.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotify_CapsuleCollision::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	FTransform WorldTransform = MeshComp->GetComponentTransform();

	FVector TransformedCenter = WorldTransform.TransformPosition(Center);
	FQuat TransformedQuat = WorldTransform.TransformRotation(Rotation.Quaternion());

#if WITH_EDITOR
	
	UKismetSystemLibrary::DrawDebugCapsule(MeshComp, TransformedCenter, HalfHeight, Radius, TransformedQuat.Rotator(), FLinearColor::White, 5.f, 1.f);

#endif

	FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(MeshComp->GetOwner());

	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(Radius, HalfHeight);
	TArray<FHitResult> CapsuleHitResults;
	
	bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(CapsuleHitResults, TransformedCenter, TransformedCenter, TransformedQuat, COLLISION_COMBAT, CollisionShape, Params);
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (BaseCharacter)
	{
		UCombatLibrary::HandleCombatCollision(BaseCharacter, CapsuleHitResults, bHit);
	}
	else
	{
		UCombatLibrary::HandleCombatCollision(MeshComp->GetOwner(), CapsuleHitResults, bHit);
	}

}
