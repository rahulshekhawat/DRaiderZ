// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_SphereCollision.h"
#include "Statics/CombatLibrary.h"
#include "Core/EODPreprocessors.h"
#include "Player/EODCharacterBase.h"


#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_SphereCollision::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	// only call from server
	if (!MeshComp->GetOwner() || MeshComp->GetOwner()->GetNetMode() == NM_Client)
	{
		return;
	}

	FTransform WorldTransform = MeshComp->GetComponentTransform();
	FVector TransformedCenter = WorldTransform.TransformPosition(Center);
	
#if WITH_EDITOR
	
	UKismetSystemLibrary::DrawDebugSphere(MeshComp, TransformedCenter, Radius, 12, FLinearColor::White, 5.f, 1.f);

#endif // WITH_EDITOR
	
	FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(MeshComp->GetOwner());

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
	TArray<FHitResult> HitResults;
	
	bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(HitResults, TransformedCenter, TransformedCenter, FQuat::Identity, COLLISION_COMBAT, CollisionShape, Params);
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
