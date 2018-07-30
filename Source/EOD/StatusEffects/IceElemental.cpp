// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "IceElemental.h"
#include "Player/EODCharacterBase.h"

#include "Engine/World.h"
#include "TimerManager.h"

TArray<AEODCharacterBase*> UIceElemental::SlowedDownCharacters = TArray<AEODCharacterBase*>();

/*
UIceElemental::UIceElemental()
{
	bTriggersOnSuccessfulHit = true;
	SlowDownDuration = 2.f;
}

void UIceElemental::OnInitialize(ABaseCharacter * Owner, class AActor* Instigator)
{
	// Owner is needed to activate buffs on self or allies
	SetOwningCharacter(Owner);
	
}

void UIceElemental::OnDeinitialize()
{
	SetOwningCharacter(nullptr);



	// @todo remove any timers that this status effect has activated on enemies?
	// @todo maybe call ConditionalBeginDestroy() on this object?
	// this->ConditionalBeginDestroy();
}

void UIceElemental::OnActivation(TArray<TWeakObjectPtr<ABaseCharacter>> RecipientCharacters)
{
	for (TWeakObjectPtr<ABaseCharacter> RecipientCharacter : RecipientCharacters)
	{
		if (RecipientCharacter.IsValid())
		{
			ABaseCharacter* Character = RecipientCharacter.Get();
			int32 ElementalResistance = Character->StatsComp->ModifyElementalIceResistance(-GetOwningCharacter()->StatsComp->GetElementalIceDamage());
			if (ElementalResistance <= 0)
			{
				// @todo Deal special elemental ice damage
			}
			
			if (UIceElemental::SlowedDownCharacters.Contains(Character))
			{
				continue;
			}

			ApplySlowDown(Character);
		}
	}
}

void UIceElemental::OnDeactivation()
{
}

void UIceElemental::ApplySlowDown(ABaseCharacter * TargetCharacter)
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("RemoveSlowDown"), TargetCharacter);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, SlowDownDuration, false);

	TargetCharacter->StatsComp->ModifyActiveTimeDilation(-SlowDownModifier);
	SlowedDownCharacters.Add(TargetCharacter);

	// @todo pass status info to the affected character
}

void UIceElemental::RemoveSlowDown(ABaseCharacter * TargetCharacter)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *FString("Removed slowdown"));
	}

	TargetCharacter->StatsComp->ModifyActiveTimeDilation(SlowDownModifier);
	SlowedDownCharacters.Remove(TargetCharacter);
}

void UIceElemental::ApplySlowDown(TWeakObjectPtr<ABaseCharacter> TargetCharacter)
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("RemoveSlowDown"), TargetCharacter);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, SlowDownDuration, false);
	TargetCharacter.Get()->StatsComp->ModifyActiveTimeDilation(-SlowDownModifier);

	AffectedCharacters.Add(TargetCharacter);
}

void UIceElemental::RemoveSlowDown(TWeakObjectPtr<ABaseCharacter> TargetCharacter)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *FString("Removed slowdown"));
	}

	TargetCharacter.Get()->StatsComp->ModifyActiveTimeDilation(SlowDownModifier);
	AffectedCharacters.Remove(TargetCharacter);
}
*/
