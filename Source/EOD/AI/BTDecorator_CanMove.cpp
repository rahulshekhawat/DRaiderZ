// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "BTDecorator_CanMove.h"
#include "EOD/Characters/EODCharacterBase.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CanMove::UBTDecorator_CanMove(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UBTDecorator_CanMove::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//~ @note The owner of 'OwnerComp' is a controller (not pawn)
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
	AEODCharacterBase* CharacterOwner = IsValid(AIController) ? Cast<AEODCharacterBase>(AIController->GetPawn()) : nullptr;
	return CharacterOwner ? CharacterOwner->CanMove() : false;
}
