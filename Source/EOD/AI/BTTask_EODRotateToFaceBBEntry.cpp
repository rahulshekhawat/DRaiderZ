// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BTTask_EODRotateToFaceBBEntry.h"
#include "EOD/Characters/EODCharacterBase.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"

UBTTask_EODRotateToFaceBBEntry::UBTTask_EODRotateToFaceBBEntry(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "EOD Rotate to face BB entry";
	bNotifyTick = true;

	// accept only actors and vectors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_EODRotateToFaceBBEntry, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_EODRotateToFaceBBEntry, BlackboardKey));
	BlackboardKey.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_EODRotateToFaceBBEntry, BlackboardKey));
}

EBTNodeResult::Type UBTTask_EODRotateToFaceBBEntry::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//~ @note The owner of 'OwnerComp' is a controller (not pawn)
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* CharacterOwner = IsValid(AIController) ? Cast<AEODCharacterBase>(AIController->GetPawn()) : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!IsValid(CharacterOwner) || !IsValid(BlackboardComp))
	{
		return EBTNodeResult::Failed;
	}

	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	FVector OwnerLocation = CharacterOwner->GetActorLocation();
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		AActor* ActorValue = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID()));
		if (IsValid( ActorValue))
		{
			FVector OrientationVector = ActorValue->GetActorLocation() - OwnerLocation;
			FRotator OrientationRotator = OrientationVector.ToOrientationRotator();
			bool bResult = CharacterOwner->DeltaRotateCharacterToDesiredYaw(OrientationRotator.Yaw, 0.f, Precision);
			if (bResult)
			{
				return EBTNodeResult::Succeeded;
			}
			else
			{
				return EBTNodeResult::InProgress;
			}
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		const FVector KeyValue = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

		FVector OrientationVector = KeyValue - OwnerLocation;
		FRotator OrientationRotator = OrientationVector.ToOrientationRotator();
		bool bResult = CharacterOwner->DeltaRotateCharacterToDesiredYaw(OrientationRotator.Yaw, 0.f, Precision);
		if (bResult)
		{
			return EBTNodeResult::Succeeded;
		}
		else
		{
			return EBTNodeResult::InProgress;
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		const FRotator KeyValue = BlackboardComp->GetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID());

		bool bResult = CharacterOwner->DeltaRotateCharacterToDesiredYaw(KeyValue.Yaw, 0.f, Precision);
		if (bResult)
		{
			return EBTNodeResult::Succeeded;
		}
		else
		{
			return EBTNodeResult::InProgress;
		}
	}
	return Result;
}

void UBTTask_EODRotateToFaceBBEntry::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//~ @note The owner of 'OwnerComp' is a controller (not pawn)
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* CharacterOwner = IsValid(AIController) ? Cast<AEODCharacterBase>(AIController->GetPawn()) : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!IsValid(CharacterOwner) || !IsValid(BlackboardComp))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	FVector OwnerLocation = CharacterOwner->GetActorLocation();
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		AActor* ActorValue = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID()));

		if (IsValid(ActorValue))
		{
			FVector OrientationVector = ActorValue->GetActorLocation() - OwnerLocation;
			FRotator OrientationRotator = OrientationVector.ToOrientationRotator();
			bool bResult = CharacterOwner->DeltaRotateCharacterToDesiredYaw(OrientationRotator.Yaw, DeltaSeconds, Precision);
			if (bResult)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		const FVector KeyValue = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

		FVector OrientationVector = KeyValue - OwnerLocation;
		FRotator OrientationRotator = OrientationVector.ToOrientationRotator();
		bool bResult = CharacterOwner->DeltaRotateCharacterToDesiredYaw(OrientationRotator.Yaw, DeltaSeconds, Precision);
		if (bResult)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		const FRotator KeyValue = BlackboardComp->GetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID());
		bool bResult = CharacterOwner->DeltaRotateCharacterToDesiredYaw(KeyValue.Yaw, DeltaSeconds, Precision);
		if (bResult)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

EBTNodeResult::Type UBTTask_EODRotateToFaceBBEntry::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}
