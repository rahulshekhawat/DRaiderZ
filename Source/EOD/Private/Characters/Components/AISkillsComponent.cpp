// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "AISkillsComponent.h"
#include "AISkillBase.h"
#include "AICharacterBase.h"
#include "EODAIControllerBase.h"

UAISkillsComponent::UAISkillsComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//~ @todo Find an appropriate value for maximum melee range
	MaxMeleeRange = 50.f;
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
		UGameplaySkillBase* GameplaySkill = NewObject<UGameplaySkillBase>(this, Row->SkillClass, Key, RF_Transient);
		check(GameplaySkill);
		GameplaySkill->InitSkill(CompOwner, CompOwner->Controller);
		GameplaySkill->SetSkillIndex(SkillIndex);

		if (GameplaySkill->GetSkillGroup() == NAME_None)
		{
			GameplaySkill->SetSkillGroup(Key);
		}
		else
		{
			check(GameplaySkill->GetSkillGroup() == Key);
		}

		SkillIndexToSkillMap.Add(GameplaySkill->GetSkillIndex(), GameplaySkill);
		SkillGroupToSkillMap.Add(GameplaySkill->GetSkillGroup(), GameplaySkill);
		SkillGroupToSkillIndexMap.Add(GameplaySkill->GetSkillGroup(), GameplaySkill->GetSkillIndex());

		SkillIndex++;
	}

	GenerateSkillTypesList();
}

void UAISkillsComponent::TriggerSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
	AEODCharacterBase* CharOwner = GetCharacterOwner();

	check(SkillIndexToSkillMap.Contains(SkillIndex));
	UAISkillBase* AISkill = Skill ? Cast<UAISkillBase>(Skill) : Cast<UAISkillBase>(SkillIndexToSkillMap[SkillIndex]);

	if (!AISkill|| !CharOwner)
	{
		return;
	}

	bool bIsLocalCharacter = CharOwner->Controller && CharOwner->Controller->IsLocalController();
	if (bIsLocalCharacter)
	{
		if (AISkill->CanTriggerSkill())
		{
			// For AI characters, the controller only exists on server (i.e., where the characters are local)
			// So, we don't really need to check for CharOwner's role and call server RPC to trigger skill
			AISkill->TriggerSkill();

			if (AISkill->bSkillCanBeCharged)
			{
				StartChargingSkill();
			}

			LastUsedSkillGroup = AISkill->GetSkillGroup();
			LastUsedSkillIndex = SkillIndex;
			ActiveSkills.Add(AISkill);
		}
	}
	else
	{
		AISkill->TriggerSkill();
		ActiveSkills.Add(AISkill);
		OnSkillTriggered(SkillIndex, AISkill->GetSkillGroup(), AISkill);
	}
}

void UAISkillsComponent::ReleaseSkill(uint8 SkillIndex, UGameplaySkillBase* Skill, float ReleaseDelay)
{
}

void UAISkillsComponent::CancelSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
}

void UAISkillsComponent::CancelAllActiveSkills()
{
	Super::CancelAllActiveSkills();
}

bool UAISkillsComponent::CanUseAnySkill() const
{
	return false;
}

bool UAISkillsComponent::CanUseSkill(uint8 SkillIndex, UGameplaySkillBase* Skill)
{
	if (!SkillIndexToSkillMap.Contains(SkillIndex))
	{
		return false;
	}

	UAISkillBase* AISkill = Skill ? Cast<UAISkillBase>(Skill) : Cast<UAISkillBase>(SkillIndexToSkillMap[SkillIndex]);
	if (!AISkill)
	{
		return false;
	}

	bool bCanTriggerSkill = AISkill->CanTriggerSkill();
	return bCanTriggerSkill;
}

void UAISkillsComponent::OnSkillCancelled(uint8 SkillIndex, FName SkillGroup, UGameplaySkillBase* Skill)
{
	Super::OnSkillCancelled(SkillIndex, SkillGroup, Skill);

	AEODCharacterBase* CharOwner = GetCharacterOwner();
	if (CharOwner)
	{
		CharOwner->SetLastUsedSkill(FLastUsedSkillInfo(SkillGroup, true));
	}
}

void UAISkillsComponent::OnSkillFinished(uint8 SkillIndex, FName SkillGroup, UGameplaySkillBase* Skill)
{
	Super::OnSkillFinished(SkillIndex, SkillGroup, Skill);

	AEODCharacterBase* CharOwner = GetCharacterOwner();
	if (CharOwner)
	{
		CharOwner->SetLastUsedSkill(FLastUsedSkillInfo(SkillGroup, false));
	}
}

FName UAISkillsComponent::GetMostWeightedSkillID(const AEODCharacterBase* TargetCharacter) const
{
	if (!TargetCharacter)
	{
		return NAME_None;
	}

	AAICharacterBase* AIChar = Cast<AAICharacterBase>(GetOwner());
	const FVector& AILocation = AIChar->GetActorLocation();
	const FVector& EnemyLocation = TargetCharacter->GetActorLocation();
	float Distance = (EnemyLocation - AILocation).Size();

	//~ @todo If the character needs healing, prioritize healing
	//~ @todo If allies are nearby and need healing, prioritize healing them unless we're are under attack
	//~ @todo If allies are nearby and fighting, buff them unless we're under attack
	//~ @todo If low on health, run away and attempt to heal

	FName WeightedSkillID;
	if (Distance > MaxMeleeRange)
	{
		WeightedSkillID = GetMostWeightedRangedSkillID(TargetCharacter);
		//~ @todo in case we don't find an appropriate ranged skill to attack with, we should reposition AI character
	}
	else
	{
		WeightedSkillID = GetMostWeightedMeleeSkillID(TargetCharacter);
		//~ @todo	in case we don't find an appropriate melee skill to attack with
		//			we should try and pick a ranged skill
	}

	//~ @todo FailSafe in-case character has only one skill

	return WeightedSkillID;
}

FName UAISkillsComponent::GetMostWeightedMeleeSkillID(const AEODCharacterBase* TargetCharacter) const
{
	TArray<FName> SkillsToIgnore;
	SkillsToIgnore.Add(LastUsedSkillGroup);

	/**
	 * Logic:
	 *	If enemy is blocking, try and get an unblockable skill.
	 *	If enemy is under CCE, then try and get a normal skill.
	 *	If enemy is dodging, then try and get an undodgable skill.
	 *	If enemy is using a skill, then try and get an interrupt skill.
	 *	If enemy is doing nothing (Idle-Walk-Run), then try and get any CCE skill.
	 */

	FName WeightedSkillID = NAME_None;
	if (TargetCharacter->IsBlocking())
	{
		WeightedSkillID = GetRandomUnblockableSkill(MeleeSkills, SkillsToIgnore);
	}
	else if (TargetCharacter->IsDodging())
	{
		WeightedSkillID = GetRandomUndodgableSkill(MeleeSkills, SkillsToIgnore);
	}
	else if (TargetCharacter->IsUsingAnySkill())
	{
		WeightedSkillID = GetRandomInterruptSkill(MeleeSkills, SkillsToIgnore);
	}
	else if (TargetCharacter->HasBeenHit())
	{
		WeightedSkillID = GetRandomNormalSkill(MeleeSkills, SkillsToIgnore);
	}
	else
	{
		WeightedSkillID = GetRandomCCESkill(MeleeSkills, SkillsToIgnore);
	}

	if (WeightedSkillID == NAME_None)
	{
		WeightedSkillID = GetAnyRandomSkill(MeleeSkills, TArray<FName>());
	}

	return WeightedSkillID;
}

FName UAISkillsComponent::GetMostWeightedRangedSkillID(const AEODCharacterBase* TargetCharacter) const
{
	TArray<FName> SkillsToIgnore;
	SkillsToIgnore.Add(LastUsedSkillGroup);

	/**
	 * Logic:
	 *	If enemy is blocking, try and get an unblockable skill.
	 *	If enemy is under CCE, then try and get a normal skill.
	 *	If enemy is dodging, then try and get an undodgable skill.
	 *	If enemy is using a skill, then try and get an interrupt skill.
	 *	If enemy is doing nothing (Idle-Walk-Run), then try and get any CCE skill.
	 */

	FName WeightedSkillID = NAME_None;
	if (TargetCharacter->IsBlocking())
	{
		WeightedSkillID = GetRandomUnblockableSkill(RangedSkills, SkillsToIgnore);
	}
	else if (TargetCharacter->IsDodging())
	{
		WeightedSkillID = GetRandomUndodgableSkill(RangedSkills, SkillsToIgnore);
	}
	else if (TargetCharacter->IsUsingAnySkill())
	{
		WeightedSkillID = GetRandomInterruptSkill(RangedSkills, SkillsToIgnore);
	}
	else if (TargetCharacter->HasBeenHit())
	{
		WeightedSkillID = GetRandomNormalSkill(RangedSkills, SkillsToIgnore);
	}
	else
	{
		WeightedSkillID = GetRandomCCESkill(RangedSkills, SkillsToIgnore);
	}

	if (WeightedSkillID == NAME_None)
	{
		WeightedSkillID = GetAnyRandomSkill(RangedSkills, TArray<FName>());
	}

	return WeightedSkillID;
}

FName UAISkillsComponent::GetHealingSkillID(bool bPartyHeal) const
{
	//~ Prioritize party healing over solo healing.

	TArray<FName> SkillsToIgnore;
	SkillsToIgnore.Add(LastUsedSkillGroup);

	//~ @todo check if there's even a party before retreiving party healing skills
	FName WeightedSkillID = GetAnyRandomSkill(PartyHealingSkills, SkillsToIgnore);
	if (WeightedSkillID == NAME_None)
	{
		WeightedSkillID = GetAnyRandomSkill(SelfHealingSkills, SkillsToIgnore);
	}

	return WeightedSkillID;
}

FName UAISkillsComponent::GetWeightedBuffSkillID(bool bPartyBuff) const
{
	//~ Prioritize party buffs over solo buffs.

	TArray<FName> SkillsToIgnore;
	SkillsToIgnore.Add(LastUsedSkillGroup);

	//~ @todo check if there's even a party before retreiving party buff skills
	FName WeightedSkillID = GetAnyRandomSkill(PartyBuffSkills, SkillsToIgnore);
	if (WeightedSkillID == NAME_None)
	{
		WeightedSkillID = GetAnyRandomSkill(SelfBuffSkills, SkillsToIgnore);
	}

	return WeightedSkillID;
}

FName UAISkillsComponent::GetWeightedDebuffSkillID() const
{
	TArray<FName> SkillsToIgnore;
	SkillsToIgnore.Add(LastUsedSkillGroup);
	FName WeightedSkillID = GetAnyRandomSkill(DebuffSkills, SkillsToIgnore);
	return WeightedSkillID;
}

void UAISkillsComponent::GenerateSkillTypesList()
{
	TArray<FName> Keys;
	SkillGroupToSkillMap.GetKeys(Keys);

	for (FName Key : Keys)
	{
		UAISkillBase* AISkill = Cast<UAISkillBase>(SkillGroupToSkillMap[Key]);
		check(AISkill);

		ESkillEffect SkillEffect = AISkill->GetSkillEffect();
		switch (SkillEffect)
		{
		case ESkillEffect::DamageMelee:
			MeleeSkills.Add(Key);
			break;
		case ESkillEffect::DamageRanged:
			RangedSkills.Add(Key);
			break;
		case ESkillEffect::HealSelf:
			SelfHealingSkills.Add(Key);
			break;
		case ESkillEffect::HealParty:
			PartyHealingSkills.Add(Key);
			break;
		case ESkillEffect::BuffSelf:
			SelfBuffSkills.Add(Key);
			break;
		case ESkillEffect::BuffParty:
			PartyBuffSkills.Add(Key);
			break;
		case ESkillEffect::DebuffEnemy:
			DebuffSkills.Add(Key);
			break;
		default:
			break;
		}
	}
}

FName UAISkillsComponent::GetRandomUnblockableSkill(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const
{
	TArray<FName> AvailableSkills = GetAvailableSkills(SkillSource, SkillsToIgnore);
	TArray<FName> NormalAttackSkills;
	for (const FName& Key : AvailableSkills)
	{
		UAISkillBase* AISkill = SkillGroupToSkillMap.Contains(Key) ? Cast<UAISkillBase>(SkillGroupToSkillMap[Key]) : nullptr;
		if (AISkill)
		{
			if (AISkill->SkillInfo.bUnblockable)
			{
				NormalAttackSkills.Add(Key);
			}
		}
	}
	return GetRandomName(NormalAttackSkills);
}

FName UAISkillsComponent::GetRandomUndodgableSkill(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const
{
	TArray<FName> AvailableSkills = GetAvailableSkills(SkillSource, SkillsToIgnore);
	TArray<FName> NormalAttackSkills;
	for (const FName& Key : AvailableSkills)
	{
		UAISkillBase* AISkill = SkillGroupToSkillMap.Contains(Key) ? Cast<UAISkillBase>(SkillGroupToSkillMap[Key]) : nullptr;
		if (AISkill)
		{
			if (AISkill->SkillInfo.bUndodgable)
			{
				NormalAttackSkills.Add(Key);
			}
		}
	}
	return GetRandomName(NormalAttackSkills);
}

FName UAISkillsComponent::GetRandomInterruptSkill(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const
{
	TArray<FName> AvailableSkills = GetAvailableSkills(SkillSource, SkillsToIgnore);
	TArray<FName> NormalAttackSkills;
	for (const FName& Key : AvailableSkills)
	{
		UAISkillBase* AISkill = SkillGroupToSkillMap.Contains(Key) ? Cast<UAISkillBase>(SkillGroupToSkillMap[Key]) : nullptr;
		if (AISkill)
		{
			if (AISkill->SkillInfo.CCEffectInfo.CCEffect != ECrowdControlEffect::Interrupt)
			{
				NormalAttackSkills.Add(Key);
			}
		}
	}
	return GetRandomName(NormalAttackSkills);
}

FName UAISkillsComponent::GetRandomCCESkill(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const
{
	TArray<FName> AvailableSkills = GetAvailableSkills(SkillSource, SkillsToIgnore);
	TArray<FName> NormalAttackSkills;
	for (const FName& Key : AvailableSkills)
	{
		UAISkillBase* AISkill = SkillGroupToSkillMap.Contains(Key) ? Cast<UAISkillBase>(SkillGroupToSkillMap[Key]) : nullptr;
		if (AISkill)
		{
			if (AISkill->SkillInfo.CCEffectInfo.CCEffect != ECrowdControlEffect::Flinch)
			{
				NormalAttackSkills.Add(Key);
			}
		}
	}
	return GetRandomName(NormalAttackSkills);
}

FName UAISkillsComponent::GetRandomNormalSkill(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const
{
	TArray<FName> AvailableSkills = GetAvailableSkills(SkillSource, SkillsToIgnore);
	TArray<FName> NormalAttackSkills;
	for (const FName& Key : AvailableSkills)
	{
		UAISkillBase* AISkill = SkillGroupToSkillMap.Contains(Key) ? Cast<UAISkillBase>(SkillGroupToSkillMap[Key]) : nullptr;
		if (AISkill)
		{
			if (AISkill->SkillInfo.CCEffectInfo.CCEffect == ECrowdControlEffect::Flinch)
			{
				NormalAttackSkills.Add(Key);
			}
		}
	}
	return GetRandomName(NormalAttackSkills);
}

FName UAISkillsComponent::GetAnyRandomSkill(const TArray<FName>& SkillSource, const TArray<FName>& SkillsToIgnore) const
{
	TArray<FName> AvailableSkills = GetAvailableSkills(SkillSource, SkillsToIgnore);
	return GetRandomName(AvailableSkills);
}
