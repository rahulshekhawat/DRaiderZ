// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SequentialMelee_AISkill.h"
#include "EODCharacterBase.h"
#include "EODCharacterMovementComponent.h"
#include "AILibrary.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USequentialMelee_AISkill::USequentialMelee_AISkill(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USequentialMelee_AISkill::TriggerSkill()
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	if (!Instigator)
	{
		return;
	}

	AAIController* AIController = Cast<AAIController>(Instigator->Controller);
	// Technically just the presence of AIController is proof enough that it's a local controller, but this is a fail-safe
	bool bIsLocalController = AIController && AIController->IsLocalController();
	if (bIsLocalController)
	{
		Instigator->SetCharacterStateAllowsMovement(false);
		Instigator->SetCharacterStateAllowsRotation(false);

		FCharacterStateInfo StateInfo(ECharacterState::UsingActiveSkill, SkillIndex);
		StateInfo.NewReplicationIndex = Instigator->CharacterStateInfo.NewReplicationIndex + 1;
		Instigator->CharacterStateInfo = StateInfo;

		UBlackboardComponent* BComp = AIController->GetBlackboardComponent();
		if (BComp)
		{
			UObject* EnemyObject = BComp->GetValueAsObject(UAILibrary::BBKey_TargetEnemy);
			AActor* EnemyActor = Cast<AActor>(EnemyObject);

			UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(Instigator->GetCharacterMovement());
			if (MoveComp)
			{
				const FVector& MyLocation = Instigator->GetActorLocation();
				const FVector& TargetLocation = EnemyActor->GetActorLocation();

				const FVector& OrientationVector = TargetLocation - MyLocation;
				FRotator OrientationRotator = OrientationVector.ToOrientationRotator();

				float DesiredRotationYaw = OrientationRotator.Yaw;
				MoveComp->SetDesiredCustomRotationYaw(DesiredRotationYaw);
			}
		}
	}

	if (SkillMontage)
	{
		SkillDuration = Instigator->PlayAnimMontage(SkillMontage, 1.f, AnimationStartSectionName);
		float ActualSkillDuration;

		if (SkillMontage->BlendOutTriggerTime >= 0.f)
		{
			ActualSkillDuration = SkillDuration;
		}
		else
		{
			ActualSkillDuration = SkillDuration - SkillMontage->BlendOut.GetBlendTime();
		}

		UWorld* World = Instigator->GetWorld();
		check(World);
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &USequentialMelee_AISkill::PlaySequentialAnimation, 0);
		World->GetTimerManager().SetTimer(SkillTimerHandle, TimerDelegate, ActualSkillDuration, false);

		Instigator->OnSkillActivated(SkillIndex, SkillGroup, this);
	}
	else
	{
		Instigator->ResetState();
	}
}

void USequentialMelee_AISkill::PlaySequentialAnimation(int32 AnimIndex)
{
	AEODCharacterBase* Instigator = SkillInstigator.Get();
	check(Instigator);

	if (SequentialAnimations.Num() <= AnimIndex)
	{
		FinishSkill();
		return;
	}

	AAIController* AIController = Cast<AAIController>(Instigator->Controller);
	bool bIsLocalController = AIController && AIController->IsLocalController();
	if (bIsLocalController)
	{
		UBlackboardComponent* BComp = AIController->GetBlackboardComponent();
		if (BComp)
		{
			UObject* EnemyObject = BComp->GetValueAsObject(UAILibrary::BBKey_TargetEnemy);
			AActor* EnemyActor = Cast<AActor>(EnemyObject);

			UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(Instigator->GetCharacterMovement());
			if (MoveComp)
			{
				const FVector& MyLocation = Instigator->GetActorLocation();
				const FVector& TargetLocation = EnemyActor->GetActorLocation();

				const FVector& OrientationVector = TargetLocation - MyLocation;
				FRotator OrientationRotator = OrientationVector.ToOrientationRotator();

				float DesiredRotationYaw = OrientationRotator.Yaw;
				MoveComp->SetDesiredCustomRotationYaw(DesiredRotationYaw);
			}
		}
	}

	const FSequentialAnimation& SeqAnim = SequentialAnimations[AnimIndex];
	if (SeqAnim.Animation)
	{
		SkillDuration = Instigator->PlayAnimMontage(SeqAnim.Animation, 1.f, SeqAnim.SectionName);
		float ActualSkillDuration;

		if (SeqAnim.Animation->BlendOutTriggerTime >= 0.f)
		{
			ActualSkillDuration = SkillDuration;
		}
		else
		{
			ActualSkillDuration = SkillDuration - SeqAnim.Animation->BlendOut.GetBlendTime();
		}

		UWorld* World = Instigator->GetWorld();
		check(World);
		if (SequentialAnimations.Num() <= AnimIndex + 1)
		{
			World->GetTimerManager().SetTimer(SkillTimerHandle, this, &USequentialMelee_AISkill::FinishSkill, ActualSkillDuration);
		}
		else
		{
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUObject(this, &USequentialMelee_AISkill::PlaySequentialAnimation, AnimIndex + 1);
			World->GetTimerManager().SetTimer(SkillTimerHandle, TimerDelegate, ActualSkillDuration, false);
		}
	}
	else
	{
		FinishSkill();
	}
}
