// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/Characters/Components/GameplaySkillsComponent.h"
#include "EOD/Characters/EODCharacterBase.h"
#include "EOD/Characters/PlayerCharacter.h"
#include "EOD/SaveSystem/PlayerSaveGame.h"
#include "EOD/Core/EODGameInstance.h"
#include "EOD/Characters/Components/EODCharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"


/**
 * EOD Character stats
 */
DECLARE_CYCLE_STAT(TEXT("EOD GameplaySkillsTick"), STAT_EODGameplaySkillsTick, STATGROUP_EOD);

UGameplaySkillsComponent::UGameplaySkillsComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);

	ChainSkillResetDelay = 2.f;
	MaxNumSkills = 20;

	for (int i = 1; i <= MaxNumSkills; i++)
	{
		SBIndexToSGMap.Add(i, FString(""));
	}
}

void UGameplaySkillsComponent::PostLoad()
{
	Super::PostLoad();
	// LoadSkillBarLayout();
}

void UGameplaySkillsComponent::BeginPlay()
{
	Super::BeginPlay();

	// Casting GetOwner() in BeginPlay because:
	// * GetOwner() in constructor is CDO of created character (e.g., Default_BP_FemaleCharacter, etc.)
	// * GetOwner() in PostLoad() was correct for AI characters spawned along with map, but was incorrect (NULL)  for player character
	// * GetOwner() has been found to be setup correctly in BeginPlay
	EODCharacterOwner = Cast<AEODCharacterBase>(GetOwner());

	if (IsValid(EODCharacterOwner) && EODCharacterOwner->IsLocallyControlled() && EODCharacterOwner->IsPlayerControlled())
	{
		// LoadSkillBarLayout();
	}
}

void UGameplaySkillsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	SCOPE_CYCLE_COUNTER(STAT_EODGameplaySkillsTick);

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (EODCharacterOwner && EODCharacterOwner->IsLocallyControlled())
	{



	}
}

void UGameplaySkillsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void UGameplaySkillsComponent::OnPressingSkillKey(const int32 SkillKeyIndex)
{
	// return if the character is incapable of using any skill right now
	if (!CanUseAnySkill())
	{
		return;
	}

	FString SGToUse = GetSkillGroupFromSkillKeyIndex(SkillKeyIndex);
	if (SGToUse == FString("") || SGToCooldownMap.Contains(SGToUse))
	{
		// Either no skill equipped in given slot or it's in cooldown
		return;
	}

	FName SkillID = GetPlayerSkillIDFromSG(SGToUse);
	FSkillTableRow* Skill = GetSkill(SkillID);

	if (!Skill)
	{
		// Invalid Skill ID
		return;
	}

	// If the player hasn't used the required preceding group 
	if (Skill->PrecedingSkillGroups.Num() > 0 && !Skill->PrecedingSkillGroups.Contains(LastUsedSkillGroup))
	{
		return;
	}

	TriggerSkill(SkillID, Skill);

}

void UGameplaySkillsComponent::OnReleasingSkillKey(const int32 SkillKeyIndex)
{
}

void UGameplaySkillsComponent::TriggerSkill(FName SkillID, FSkillTableRow* Skill)
{
	if (!IsValid(EODCharacterOwner))
	{
		return;
	}

	if (!Skill)
	{
		Skill = GetSkill(SkillID);
	}

	if (Skill && Skill->AnimMontage.Get())
	{
		EODCharacterOwner->PlayAnimationMontage(Skill->AnimMontage.Get(), Skill->SkillStartMontageSectionName, ECharacterState::UsingActiveSkill);
	}

	EODCharacterOwner->SetCharacterStateAllowsRotation(false);
	float ControlRotationYaw = EODCharacterOwner->GetControllerRotationYaw();
	UEODCharacterMovementComponent* MoveComp = Cast<UEODCharacterMovementComponent>(EODCharacterOwner->GetCharacterMovement());
	if (MoveComp)
	{
		MoveComp->SetDesiredCustomRotation(FRotator(0.f, ControlRotationYaw, 0.f));
	}
}

void UGameplaySkillsComponent::ReleaseSkill(FName SkillID, FSkillTableRow* Skill)
{
}

void UGameplaySkillsComponent::CancelCurrentSkill()
{
}

void UGameplaySkillsComponent::CancelSkill(FName SkillID)
{
}

bool UGameplaySkillsComponent::CanUseAnySkill() const
{
	return IsValid(EODCharacterOwner) && EODCharacterOwner->CanUseAnySkill();
}

bool UGameplaySkillsComponent::CanUseSkill(FName SkillID) const
{
	return false;
}

bool UGameplaySkillsComponent::CanUseSkillAtIndex(const int32 SkillKeyIndex) const
{
	return false;
}

FName UGameplaySkillsComponent::GetPlayerSkillIDFromSG(FString& SkillGroup) const
{
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(EODCharacterOwner);
	FString GenderPrefix = PlayerChar ? PlayerChar->GetGenderPrefix() : FString("");
	FString SkillIDString = GenderPrefix + SkillGroup + FString("_") + FString::FromInt(1);
	return FName(*SkillIDString);
}

FString UGameplaySkillsComponent::GetSkillGroupFromSkillKeyIndex(const int32 SkillKeyIndex) const
{
	if (IsSkillKeyIndexInvalid(SkillKeyIndex))
	{
		// Invalid skill key index
		return FString("");
	}



	if (ActiveSupersedingChainSkillGroup.Key == SkillKeyIndex)
	{
		return ActiveSupersedingChainSkillGroup.Value;
	}

	return SBIndexToSGMap[SkillKeyIndex];
}

void UGameplaySkillsComponent::UseSkill(FName SkillID)
{
}

void UGameplaySkillsComponent::SetCurrentActiveSkill(const FName SkillID)
{
	// Although setting ActiveSkillID and ActiveSkill on local client is un-necessary from combat perspective (all combat events fire on server only),
	// these variables have been set up in local client as well for query purposes.

	if (SkillID == NAME_None)
	{
		ActiveSkillID = NAME_None;
		ActiveSkill = nullptr;
	}
	else
	{
		ActiveSkill = GetSkill(SkillID);
		ActiveSkillID = ActiveSkill ? SkillID : NAME_None;
	}

	if (IsValid(EODCharacterOwner) && EODCharacterOwner->Role < ROLE_Authority)
	{
		Server_SetCurrentActiveSkill(SkillID);
	}
}

void UGameplaySkillsComponent::LoadSkillBarLayout()
{
	UEODGameInstance* GameInstance = EODCharacterOwner ? Cast<UEODGameInstance>(EODCharacterOwner->GetGameInstance()) : nullptr;
	if (!IsValid(GameInstance) || !IsValid(GameInstance->GetCurrentPlayerSaveGameObject()))
	{
		return;
	}
	
	UPlayerSaveGame* PlayerSaveGame = GameInstance->GetCurrentPlayerSaveGameObject();

	TMap<int32, FString>& SBLayout = PlayerSaveGame->SkillBarLayout;
	TArray<int32> LayoutKeys;
	SBLayout.GetKeys(LayoutKeys);
	for (int32 Key : LayoutKeys)
	{
		if (SBLayout[Key] == FString(""))
		{
			continue;
		}

		if (SBIndexToSGMap.Contains(Key))
		{
			SBIndexToSGMap[Key] = SBLayout[Key];
		}
	}
}

void UGameplaySkillsComponent::SaveSkillBarLayout()
{
	UEODGameInstance* GameInstance = EODCharacterOwner ? Cast<UEODGameInstance>(EODCharacterOwner->GetGameInstance()) : nullptr;
	UPlayerSaveGame* PlayerSaveGame = GameInstance ? GameInstance->GetCurrentPlayerSaveGameObject() : nullptr;
	if (IsValid(PlayerSaveGame))
	{
		PlayerSaveGame->SkillBarLayout = SBIndexToSGMap;
		UGameplayStatics::SaveGameToSlot(PlayerSaveGame, GameInstance->GetCurrentPlayerSaveGameName(), GameInstance->PlayerIndex);
	}
}

void UGameplaySkillsComponent::AddNewSkill(int32 SkillIndex, FString SkillGroup)
{
	SBIndexToSGMap.Add(SkillIndex, SkillGroup);

	//~ @todo load animations for the skill
}

/*
void UGameplaySkillsComponent::OnRep_ActiveSkillID()
{
}
*/

void UGameplaySkillsComponent::Server_TriggerSkill_Implementation(FName SkillID)
{
	// TriggerSkill(SkillID);
}

bool UGameplaySkillsComponent::Server_TriggerSkill_Validate(FName SkillID)
{
	return true;
}

void UGameplaySkillsComponent::Server_SetCurrentActiveSkill_Implementation(const FName SkillID)
{
	SetCurrentActiveSkill(SkillID);
}

bool UGameplaySkillsComponent::Server_SetCurrentActiveSkill_Validate(const FName SkillID)
{
	return true;
}
