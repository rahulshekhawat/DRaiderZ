// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/Characters/Components/GameplaySkillsComponent.h"
#include "EOD/Characters/EODCharacterBase.h"


UGameplaySkillsComponent::UGameplaySkillsComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);

	ChainSkillResetDelay = 2.f;
}


void UGameplaySkillsComponent::PostLoad()
{
	Super::PostLoad();
}

void UGameplaySkillsComponent::BeginPlay()
{
	Super::BeginPlay();

	// Casting GetOwner() in BeginPlay because:
	// * GetOwner() in constructor is CDO of created character (e.g., Default_BP_FemaleCharacter, etc.)
	// * GetOwner() in PostLoad() was correct for AI characters spawned along with map, but was incorrect (NULL)  for player character
	// * GetOwner() has been found to be setup correctly in BeginPlay
	CharacterOwner = Cast<AEODCharacterBase>(GetOwner());

}

void UGameplaySkillsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UGameplaySkillsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UGameplaySkillsComponent::SetCurrentActiveSkill(const FName SkillID)
{
	if (!IsValid(CharacterOwner))
	{
		return;
	}
	else if (IsValid(CharacterOwner) && CharacterOwner->Role < ROLE_Authority)
	{
		Server_SetCurrentActiveSkill(SkillID);
	}
	else if (IsValid(CharacterOwner) && CharacterOwner->Role == ROLE_Authority)
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

void UGameplaySkillsComponent::Server_SetCurrentActiveSkill_Implementation(const FName SkillID)
{
	SetCurrentActiveSkill(SkillID);
}

bool UGameplaySkillsComponent::Server_SetCurrentActiveSkill_Validate(const FName SkillID)
{
	return true;
}
