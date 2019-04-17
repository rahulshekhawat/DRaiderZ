// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "AISkillsComponent.h"
#include "AISkillBase.h"
#include "AICharacterBase.h"
#include "EODAIControllerBase.h"

UAISkillsComponent::UAISkillsComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UAISkillsComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeSkills();
}

void UAISkillsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAISkillsComponent::InitializeSkills(AEODCharacterBase* CompOwner)
{
	// If skills have already been initialized
	if (SkillIndexToSkillMap.Num() > 0)
	{
		return;
	}

	if (!CompOwner)
	{
		// If CompOwner is invalid, try to initalize it from locally cached component owner
		CompOwner = GetCharacterOwner();
	}

	check(CompOwner);
	if (!SkillsDataTable)
	{
		return;
	}

	FString ContextString = FString("UAISkillsComponent::InitializeSkills()");
	TArray<FName> Keys = SkillsDataTable->GetRowNames();
	uint8 SkillIndex = 1;
	for (FName Key : Keys)
	{
		FGameplaySkillTableRow* Row = SkillsDataTable->FindRow<FGameplaySkillTableRow>(Key, ContextString);
		check(Row);
		UGameplaySkillBase* GameplaySkill = NewObject<UGameplaySkillBase>(this, Row->PlayerSkill, Key, RF_Transient);
		check(GameplaySkill);
		GameplaySkill->InitSkill(CompOwner, CompOwner->Controller);
		GameplaySkill->SetSkillIndex(SkillIndex);

		SkillIndexToSkillMap.Add(GameplaySkill->GetSkillIndex(), GameplaySkill);
		SkillGroupToSkillMap.Add(GameplaySkill->GetSkillGroup(), GameplaySkill);
		SkillGroupToSkillIndexMap.Add(GameplaySkill->GetSkillGroup(), GameplaySkill->GetSkillIndex());

		SkillIndex++;
	}
}
