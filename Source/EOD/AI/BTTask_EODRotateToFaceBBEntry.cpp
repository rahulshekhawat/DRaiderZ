// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BTTask_EODRotateToFaceBBEntry.h"
#include "Player/EODCharacterBase.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"

UBTTask_EODRotateToFaceBBEntry::UBTTask_EODRotateToFaceBBEntry(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "EOD Rotate to face BB entry";
	bNotifyTick = true;

	// accept only actors and vectors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_EODRotateToFaceBBEntry, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_EODRotateToFaceBBEntry, BlackboardKey));
	BlackboardKey.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_EODRotateToFaceBBEntry, BlackboardKey));
}

EBTNodeResult::Type UBTTask_EODRotateToFaceBBEntry::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* OwningCharacter = Cast<AEODCharacterBase>(AIController->GetPawn());

	// @note if AIController is NULL, the previous line will lead to a crash. Kinda dumb to test for NULL after using it.
	if (AIController == nullptr || OwningCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	FVector OwnerLocation = OwningCharacter->GetActorLocation();
	// const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* ActorValue = Cast<AActor>(KeyValue);

		if (ActorValue != nullptr)
		{
			FVector OrientationVector = ActorValue->GetActorLocation() - OwnerLocation;
			FRotator OrientationRotator = OrientationVector.ToOrientationRotator();

			bool bResult = OwningCharacter->DeltaRotateCharacterToDesiredYaw(OrientationRotator.Yaw, 0.f, Precision);
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
		const FVector KeyValue = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

		FVector OrientationVector = KeyValue - OwnerLocation;
		FRotator OrientationRotator = OrientationVector.ToOrientationRotator();

		bool bResult = OwningCharacter->DeltaRotateCharacterToDesiredYaw(OrientationRotator.Yaw, 0.f, Precision);
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
		const FRotator KeyValue = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID());

		bool bResult = OwningCharacter->DeltaRotateCharacterToDesiredYaw(KeyValue.Yaw, 0.f, Precision);
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

void UBTTask_EODRotateToFaceBBEntry::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* OwningCharacter = Cast<AEODCharacterBase>(AIController->GetPawn());

	if (AIController == nullptr || OwningCharacter == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	FVector OwnerLocation = OwningCharacter->GetActorLocation();
	// const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* ActorValue = Cast<AActor>(KeyValue);

		if (ActorValue != nullptr)
		{
			FVector OrientationVector = ActorValue->GetActorLocation() - OwnerLocation;
			FRotator OrientationRotator = OrientationVector.ToOrientationRotator();

			bool bResult = OwningCharacter->DeltaRotateCharacterToDesiredYaw(OrientationRotator.Yaw, DeltaSeconds, Precision);
			if (bResult)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		const FVector KeyValue = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

		FVector OrientationVector = KeyValue - OwnerLocation;
		FRotator OrientationRotator = OrientationVector.ToOrientationRotator();

		bool bResult = OwningCharacter->DeltaRotateCharacterToDesiredYaw(OrientationRotator.Yaw, DeltaSeconds, Precision);
		if (bResult)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		const FRotator KeyValue = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID());

		bool bResult = OwningCharacter->DeltaRotateCharacterToDesiredYaw(KeyValue.Yaw, DeltaSeconds, Precision);
		if (bResult)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

EBTNodeResult::Type UBTTask_EODRotateToFaceBBEntry::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	return EBTNodeResult::Aborted;
}
