// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "PlayerSkillsComponent.h"
#include "Skills/GameplaySkillBase.h"
#include "PlayerCharacter.h"

#include "UnrealNetwork.h"

UPlayerSkillsComponent::UPlayerSkillsComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerSkillsComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeSkills();
}

void UPlayerSkillsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerSkillsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UPlayerSkillsComponent::OnPressingSkillKey(const int32 SkillKeyIndex)
{
	uint8 SkillIndex = 0;
	UGameplaySkillBase* Skill = nullptr;
	if (SkillBarMap.Contains(SkillKeyIndex))
	{
		SkillIndex = SkillBarMap[SkillKeyIndex];
	}

	if (SkillsMap.Contains(SkillIndex))
	{
		Skill = SkillsMap[SkillIndex];
	}

	if (Skill)
	{
		TriggerSkill(SkillIndex, Skill);
	}
}

void UPlayerSkillsComponent::OnReleasingSkillKey(const int32 SkillKeyIndex)
{
}

void UPlayerSkillsComponent::InitializeSkills(AEODCharacterBase* CompOwner)
{
	// If skills have already been initialized
	if (SkillsMap.Num() > 0)
	{
		return;
	}

	if (!CompOwner)
	{
		CompOwner = GetCharacterOwner();
	}

	check(CompOwner);
	check(SkillsDataTable);

	FString ContextString = FString("UPlayerSkillsComponent::InitializeSkills()");
	TArray<FName> Keys = SkillsDataTable->GetRowNames();
	uint8 SkillIndex = 1;
	for (FName Key : Keys)
	{
		FGameplaySkillTableRow* Row = SkillsDataTable->FindRow<FGameplaySkillTableRow>(Key, ContextString);
		check(Row);
		UGameplaySkillBase* GameplaySkill = NewObject<UGameplaySkillBase>(this, Row->PlayerSkill, Key, RF_Transient);
		check(GameplaySkill)
		GameplaySkill->InitSkill(CompOwner, CompOwner->Controller);
		GameplaySkill->SetSkillGroup(Key);
		SkillsMap.Add(SkillIndex, GameplaySkill);

		SkillIndex++;
	}
}

void UPlayerSkillsComponent::OnSkillAddedToSkillBar(uint8 SkillBarIndex, FName SkillGroup)
{
	TArray<uint8> Keys;
	SkillsMap.GetKeys(Keys);

	for (uint8 Key : Keys)
	{
		UGameplaySkillBase* Skill = SkillsMap[Key];
		if (Skill && Skill->GetSkillGroup() == SkillGroup)
		{
			if (SkillBarMap.Contains(Key))
			{
				SkillBarMap[SkillBarIndex] = Key;
			}
			else
			{
				SkillBarMap.Add(SkillBarIndex, Key);
			}

			break;
		}
	}
}

void UPlayerSkillsComponent::TriggerSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
	if (!Skill)
	{
		Skill = SkillsMap[SkillIndex];
	}

	if (!Skill)
	{
		return;
	}

	if (Skill->CanActivateSkill())
	{
		Skill->ActivateSkill();
	}
}

void UPlayerSkillsComponent::ReleaseSkill(uint8 SkillIndex, UGameplaySkillBase* Skill, float ReleaseDelay)
{
}
