// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "CharAnimInstance.h"
#include "EODCharacterBase.h"


UCharAnimInstance::UCharAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	IdleWalkRun_AnimationsBlendTime = 0.2f;
	Default_AnimationBlendTime = 0.2f;
}

void UCharAnimInstance::NativeInitializeAnimation()
{
	EODCharacterOwner = TryGetPawnOwner() ? Cast<AEODCharacterBase>(TryGetPawnOwner()) : nullptr;

	OnMontageBlendingOut.AddUniqueDynamic(this, &UCharAnimInstance::HandleMontageBlendingOut);
	OnMontageEnded.AddUniqueDynamic(this, &UCharAnimInstance::HandleMontageEnded);
}

void UCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!IsValid(EODCharacterOwner))
	{
		if (!IsValid((EODCharacterOwner = TryGetPawnOwner() ? Cast<AEODCharacterBase>(TryGetPawnOwner()) : nullptr)))
		{
			return;
		}
	}

	MovementSpeed = EODCharacterOwner->GetVelocity().Size();
	CharacterMovementDirection = EODCharacterOwner->GetCharacterMovementDirection();
	bIsBlocking = EODCharacterOwner->IsBlocking();
}

void UCharAnimInstance::NativePostEvaluateAnimation()
{
}

void UCharAnimInstance::NativeUninitializeAnimation()
{
}

float UCharAnimInstance::GetMovementSpeed() const
{
	return EODCharacterOwner ? EODCharacterOwner->GetVelocity().Size() : 0.f;
}

bool UCharAnimInstance::IsBlocking() const
{
	return EODCharacterOwner ? EODCharacterOwner->IsBlocking() : false;
}

ECharMovementDirection UCharAnimInstance::GetIWRCharMovementDir() const
{
	return EODCharacterOwner ? EODCharacterOwner->GetCharacterMovementDirection() : ECharMovementDirection::None;
}

void UCharAnimInstance::TransitionBetweenMontages(
	UAnimMontage* TransitionFromMontage,
	UAnimMontage* TransitionToMontage,
	const FName Section,
	bool bZeroBlendOut)
{
	if (Montage_IsPlaying(TransitionFromMontage))
	{
		int32 MovingMontageSectionIndex = TransitionToMontage->GetSectionIndex(Section);
		int32 StandStillMontageSectionIndex = TransitionFromMontage->GetSectionIndex(Section);
		if (MovingMontageSectionIndex != INDEX_NONE && StandStillMontageSectionIndex != INDEX_NONE)
		{
			float SectionStartTime;
			float SectionEndtime;
			TransitionFromMontage->GetSectionStartAndEndTime(StandStillMontageSectionIndex, SectionStartTime, SectionEndtime);
			float CurrentPosition = Montage_GetPosition(TransitionFromMontage);
			float SectionOffset = CurrentPosition - SectionStartTime;
			if (bZeroBlendOut)
			{
				Montage_Stop(0.f, TransitionFromMontage);
			}
			else
			{
				Montage_Stop(TransitionFromMontage->BlendOut.GetBlendTime(), TransitionFromMontage);
			}

			TransitionToMontage->GetSectionStartAndEndTime(MovingMontageSectionIndex, SectionStartTime, SectionEndtime);
			float MontageStartPosition = SectionStartTime + SectionOffset;

			Montage_Play(TransitionToMontage, 1.f, EMontagePlayReturnType::MontageLength, MontageStartPosition);
		}
	}
}

float UCharAnimInstance::GetSectionLength(UAnimMontage* AnimMontage, FName SectionName)
{
	if (!AnimMontage || SectionName == NAME_None)
	{
		return -1.f;
	}

	int32 SectionIndex = AnimMontage->GetSectionIndex(SectionName);
	if (SectionIndex != INDEX_NONE)
	{
		float StartTime;
		float EndTime;
		AnimMontage->GetSectionStartAndEndTime(SectionIndex, StartTime, EndTime);
		return EndTime - StartTime;
	}

	return -1.f;
}

void UCharAnimInstance::HandleMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (IsValid(EODCharacterOwner))
	{
		EODCharacterOwner->OnMontageBlendingOut(AnimMontage, bInterrupted);
	}
}

void UCharAnimInstance::HandleMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (IsValid(EODCharacterOwner))
	{
		EODCharacterOwner->OnMontageEnded(AnimMontage, bInterrupted);
	}
}

bool UCharAnimInstance::IsValidActionTag(const FGameplayTag& ActionTag) const
{
	return WeaponActions.Contains(ActionTag) || GenericActions.Contains(ActionTag);
}

FEODAction UCharAnimInstance::GetAction(const FGameplayTag& ActionTag) const
{
	if (ActionTag.IsValid() == false)
	{
		UE_LOG(LogRaiderZ, Warning, TEXT("Invalid ActionTag passed in function: %s"), *FString(__FUNCTION__));
		return FEODAction();
	}

	if (EODCharacterOwner && EODCharacterOwner->GetEquippedWeaponTag().IsValid())
	{
		const FGameplayTag WeaponTag = EODCharacterOwner->GetEquippedWeaponTag();
		if (WeaponActions.Contains(ActionTag))
		{
			const FWeaponActionTableRow& WeaponActionRow = WeaponActions[ActionTag];
			if (WeaponActionRow.WeaponToActionMap.Contains(WeaponTag))
			{
				return WeaponActionRow.WeaponToActionMap[WeaponTag];
			}
		}
	}
	
	if (GenericActions.Contains(ActionTag))
	{
		return GenericActions[ActionTag];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find the action tag: %s"), *ActionTag.ToString());
		return FEODAction();
	}
}

void UCharAnimInstance::InitializeActions()
{
	if (CustomActions.Num() > 0)
	{
		GenericActions.Append(CustomActions);
	}

	for (UDataTable* ActionsDataTable : ActionsDataTables)
	{
		if (ActionsDataTable == nullptr)
		{
			continue;
		}

		TArray<FName> RowNames = ActionsDataTable->GetRowNames();
		for (FName RowName : RowNames)
		{
			FWeaponActionTableRow* Row = ActionsDataTable->FindRow<FWeaponActionTableRow>(RowName, FString(__FUNCTION__));
			FGameplayTag KeyTag = FGameplayTag::RequestGameplayTag(RowName);
			WeaponActions.Add(KeyTag, *Row);
		}
	}
}
