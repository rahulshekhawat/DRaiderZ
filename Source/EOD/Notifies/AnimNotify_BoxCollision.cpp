// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_BoxCollision.h"
#include "Statics/CombatLibrary.h"
#include "Player/EODCharacterBase.h"
#include "Core/EODPreprocessors.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotify_BoxCollision::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
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
	
	UKismetSystemLibrary::DrawDebugBox(MeshComp, TransformedCenter, BoxHalfExtent, FLinearColor::White, TransformedQuat.Rotator(), 5.f, 1.f);

#endif // WITH_EDITOR
	
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(BoxHalfExtent);
	TArray<FHitResult> HitResults;
	
	FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(MeshComp->GetOwner());
	
	bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(HitResults, TransformedCenter, TransformedCenter, TransformedQuat, COLLISION_COMBAT, CollisionShape, Params);
	AEODCharacterBase* BaseCharacter = Cast<AEODCharacterBase>(MeshComp->GetOwner());
	if (BaseCharacter)
	{
		UCombatLibrary::HandleCombatCollision(BaseCharacter, Animation, HitResults, bHit);
	}
	else
	{
		UCombatLibrary::HandleCombatCollision(MeshComp->GetOwner(), Animation, HitResults, bHit);
	}


}
