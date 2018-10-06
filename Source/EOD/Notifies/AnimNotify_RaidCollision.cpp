// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_RaidCollision.h"
#include "Core/EODPreprocessors.h"
#include "Statics/CombatLibrary.h"
#include "Player/EODCharacterBase.h"
#include "Core/CombatZoneModeBase.h"
#include "Core/CombatManager.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
// #include "Kismet/KismetMathLibrary.h"

void UAnimNotify_RaidCollision::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	// Only process from server
	if (!MeshComp->GetOwner() || MeshComp->GetOwner()->GetNetMode() == NM_Client)
	{
		return;
	}

	// Only process if the game mode is ACombatZoneModeBase
	ACombatZoneModeBase* CombatZoneGameMode = Cast<ACombatZoneModeBase>(GetWorld()->GetAuthGameMode());
	if (!CombatZoneGameMode)
	{
		return;
	}

	for (FRaidCapsule& Capsule : CollisionCapsules)
	{
		FTransform WorldTransform = MeshComp->GetComponentTransform();

		FVector CorrectedBottom = Capsule.Bottom.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));
		FVector CorrectedTop = Capsule.Top.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));
		FVector HalfHeightVector = (CorrectedTop - CorrectedBottom) / 2;
		FVector NormalizedHeightVector = HalfHeightVector.GetSafeNormal();
		FVector Center = CorrectedBottom + HalfHeightVector;
		FRotator CapsuleRotation = FRotationMatrix::MakeFromZ(HalfHeightVector).Rotator();

		// Transformation from object space to world space
		FVector TransformedCenter = WorldTransform.TransformPosition(Center);
		FRotator TransformedRotation = WorldTransform.TransformRotation(CapsuleRotation.Quaternion()).Rotator();

		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(Capsule.Radius, HalfHeightVector.Size());
		FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(MeshComp->GetOwner());
		TArray<FHitResult> HitResults;

		bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(HitResults, TransformedCenter, TransformedCenter, TransformedRotation.Quaternion(), COLLISION_COMBAT, CollisionShape, Params);
		CombatZoneGameMode->GetCombatManager()->OnMeleeHit(MeshComp->GetOwner(), bHit, HitResults);		

#if WITH_EDITOR // draw debug shapes only if inside editor
		UKismetSystemLibrary::DrawDebugCapsule(MeshComp, TransformedCenter, HalfHeightVector.Size(), Capsule.Radius, TransformedRotation, FLinearColor::White, 5.f, 1.f);
#endif // WITH_EDITOR
	}

	/*
	for (FRaidCapsule& Capsule : CollisionCapsules)
	{
		FTransform WorldTransform = MeshComp->GetComponentTransform();

		FVector CorrectedBottom = Capsule.Bottom.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));
		FVector CorrectedTop = Capsule.Top.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));

		FVector HalfHeightVector = (CorrectedTop - CorrectedBottom) / 2;
		FVector NormalizedHeightVector = HalfHeightVector.GetSafeNormal();
		FVector Center = CorrectedBottom + HalfHeightVector;
		FRotator CapsuleRotation = UKismetMathLibrary::MakeRotFromZ(HalfHeightVector);

		// Transform Center
		FVector TransformedCenter = WorldTransform.TransformPosition(Center);
		// Transform rotation
		FRotator TransformedRotation = WorldTransform.TransformRotation(CapsuleRotation.Quaternion()).Rotator();

#if WITH_EDITOR // draw debug shapes only if inside editor

		UKismetSystemLibrary::DrawDebugCapsule(MeshComp, TransformedCenter, HalfHeightVector.Size(), Capsule.Radius, TransformedRotation, FLinearColor::White, 5.f, 1.f);
		
		FVector TransformedBottom;
		FVector TransformedTop;
		FVector RotationFixedNormalizedHeightVector = TransformedRotation.RotateVector(NormalizedHeightVector);
		if (HalfHeightVector.Size() > Capsule.Radius)
		{
			// TransformedBottom = TransformedCenter - NormalizedHeightVector * HalfHeightVector.Size();
			TransformedBottom = TransformedCenter - RotationFixedNormalizedHeightVector * HalfHeightVector.Size();
			// TransformedTop = TransformedCenter + NormalizedHeightVector * HalfHeightVector.Size();
			TransformedTop = TransformedCenter + RotationFixedNormalizedHeightVector * HalfHeightVector.Size();
		}
		else
		{
			// TransformedBottom = TransformedCenter - NormalizedHeightVector * Capsule.Radius;
			TransformedBottom = TransformedCenter - RotationFixedNormalizedHeightVector * Capsule.Radius;
			// TransformedTop = TransformedCenter + NormalizedHeightVector * Capsule.Radius;
			TransformedTop = TransformedCenter + RotationFixedNormalizedHeightVector * Capsule.Radius;
		}

		UKismetSystemLibrary::DrawDebugArrow(MeshComp, TransformedBottom, TransformedTop, 100.f, FLinearColor::Red, 5.f, 2.f);
		
#endif // WITH_EDITOR

#if DEVSTAGE_CODE_ENABLED

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("CALLED FROM SERVER"));

#endif // DEVSTAGE_CODE_ENABLED

		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(Capsule.Radius, HalfHeightVector.Size());
		TArray<FHitResult> HitResults;
		
		FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(MeshComp->GetOwner());

		bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(HitResults, TransformedCenter, TransformedCenter, TransformedRotation.Quaternion(), COLLISION_COMBAT, CollisionShape, Params);
		AEODCharacterBase* BaseCharacter = Cast<AEODCharacterBase>(MeshComp->GetOwner());
		if (BaseCharacter)
		{
			// UCombatLibrary::HandleCombatCollision(BaseCharacter, Animation, HitResults, bHit);
			BaseCharacter->OnMeleeCollision(Animation, HitResults, bHit);
		}
		else
		{
			UCombatLibrary::HandleCombatCollision(MeshComp->GetOwner(), Animation, HitResults, bHit);
		}
		
#if DEVSTAGE_CODE_ENABLED

		TArray<FHitResult> LineHitResults;
		MeshComp->GetWorld()->LineTraceMultiByChannel(LineHitResults, TransformedTop, TransformedBottom, COLLISION_COMBAT, Params);

		for (FHitResult& HitResult : LineHitResults)
		{
			FVector Start = HitResult.ImpactPoint;
			FVector End = HitResult.ImpactPoint + HitResult.ImpactNormal * 50;
			UKismetSystemLibrary::DrawDebugArrow(MeshComp, Start, End, 100, FLinearColor::Blue, 5.f, 2.f);
		}
#endif
	}
	*/
}
