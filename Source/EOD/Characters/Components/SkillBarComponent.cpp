// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/Characters/Components/SkillBarComponent.h"
#include "EOD/Characters/EODCharacterBase.h"

#include "UnrealNetwork.h"

USkillBarComponent::USkillBarComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
	
	ChainSkillResetDelay = 2.f;
	CharacterOwner = Cast<AEODCharacterBase>(GetOwner());
}

void USkillBarComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USkillBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void USkillBarComponent::SetCurrentActiveSkill(const FName SkillID)
{
	if (IsValid(CharacterOwner) && CharacterOwner->Role < ROLE_Authority)
	{
		Server_SetCurrentActiveSkill(SkillID);
	}
	else
	{
		if (SkillID == NAME_None)
		{
			ActiveSkill = nullptr;
		}
		else
		{
			ActiveSkill = GetSkill(SkillID);
			ActiveSkillID = ActiveSkill ? SkillID : NAME_None;
		}
	}
}

void USkillBarComponent::Server_SetCurrentActiveSkill_Implementation(const FName SkillID)
{
	SetCurrentActiveSkill(SkillID);
}

bool USkillBarComponent::Server_SetCurrentActiveSkill_Validate(const FName SkillID)
{
	return true;
}
