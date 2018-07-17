// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AnimNotify_RaidCollision.h"
#include "Core/EODPreprocessors.h"

#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNotify_RaidCollision::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	for (FCapsuleInfo& Capsule : CollisionCapsules)
	{
#if WITH_EDITOR

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

		UKismetSystemLibrary::DrawDebugCapsule(MeshComp, TransformedCenter, HalfHeightVector.Size(), Capsule.Radius, TransformedRotation, FLinearColor::White, 5.f, 1.f);
		
		FVector TransformedBottom = WorldTransform.TransformPosition(CorrectedBottom);
		FVector TransformedTop = WorldTransform.TransformPosition(CorrectedTop);
		UKismetSystemLibrary::DrawDebugArrow(MeshComp, TransformedBottom, TransformedTop, 100.f, FLinearColor::Red, 5.f, 2.f);

#endif // WITH_EDITOR

		//~ Begin collision handling code

		/*
		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(Capsule.Radius, HalfHeightVector.Size());
		TArray<FHitResult> HitResults;
		MeshComp->GetWorld()->SweepMultiByChannel(HitResults, TransformedCenter, TransformedCenter, TransformedRotation.Quaternion(), ECollisionChannel::ECC_Camera, CollisionShape);

		for (FHitResult& HitResult : HitResults)
		{
			FString HitActorName = HitResult.GetActor()->GetName();
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *HitActorName);
			}
		}
		*/
		//~ End collision handling code

	}

#if WITH_DEVSTAGE_CODE
	/*
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;
 
	//	Re-initialize hit info
	FHitResult RV_Hit(ForceInit);
     
	//	call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingle(
		RV_Hit,        //result
		Start,    //start
		End, //end
		ECC_Pawn, //collision channel
		RV_TraceParams
		);
	 */

	// TSharedPtr<FCapsuleInfo> MyPtr = 
#endif

}
