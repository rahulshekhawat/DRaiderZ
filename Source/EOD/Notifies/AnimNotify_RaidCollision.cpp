// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_RaidCollision.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNotify_RaidCollision::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	for (FCapsuleInfo& Capsule : CollisionCapsules)
	{
		FTransform WorldTransform = MeshComp->GetComponentTransform();

		FVector CorrectedBottom = Capsule.Bottom.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));
		FVector CorrectedTop = Capsule.Top.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));

		FVector HalfHeightVector = (CorrectedTop - CorrectedBottom) / 2;
		FVector Center = CorrectedBottom + HalfHeightVector;

		// Transform Center
		FVector TransformedCenter = WorldTransform.TransformPosition(Center);

		FRotator CapsuleRotation = UKismetMathLibrary::MakeRotFromZ(HalfHeightVector);

		// Transform rotation
		FRotator TransformedRotation = WorldTransform.TransformRotation(CapsuleRotation.Quaternion()).Rotator();

		UKismetSystemLibrary::DrawDebugCapsule(MeshComp, Center, HalfHeightVector.Size(), Capsule.Radius, TransformedRotation, FLinearColor::White, 5.f, 1.f);

	}
}
