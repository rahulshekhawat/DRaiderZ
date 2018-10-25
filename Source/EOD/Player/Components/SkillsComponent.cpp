// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillsComponent.h"
#include "UI/HUDWidget.h"
#include "UI/SkillTreeWidget.h"
#include "Core/EODPreprocessors.h"
#include "Core/EODSaveGame.h"
#include "Core/GameSingleton.h"
#include "Player/PlayerCharacter.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


USkillsComponent::USkillsComponent(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	ChainSkillResetDelay = 2.f;

	APlayerCharacter* PC = Cast<APlayerCharacter>(GetOwner());
	if (PC)
	{
		OwnerGender = PC->GetCharacterGender();
	}
}

void USkillsComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeComponentWidgets();
}

void USkillsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

inline USkillBarWidget * USkillsComponent::BP_GetSkillBarWidget() const
{
	return GetSkillBarWidget();
}

inline USkillTreeWidget * USkillsComponent::BP_GetSkillTreeWidget() const
{
	return GetSkillTreeWidget();
}

void USkillsComponent::ToggleSkillTreeUI()
{
	if (SkillTreeWidget)
	{
		if (SkillTreeWidget->IsVisible())
		{
			SkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			SkillTreeWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void USkillsComponent::InitializeComponentWidgets()
{
	APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetOwner());
	if (!(OwningPlayer && OwningPlayer->Controller && OwningPlayer->Controller->IsLocalPlayerController() && OwningPlayer->GetHUDWidget()))
	{
		return;
	}

	if (SkillTreeWidgetClass.Get())
	{
		SkillTreeWidget = CreateWidget<USkillTreeWidget>(OwningPlayer->GetGameInstance(), SkillTreeWidgetClass);
		OwningPlayer->GetHUDWidget()->AddSkillTreeWidget(SkillTreeWidget);
		SkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (SkillBarWidgetClass.Get())
	{
		SkillBarWidget = CreateWidget<USkillBarWidget>(OwningPlayer->GetGameInstance(), SkillBarWidgetClass);
		OwningPlayer->GetHUDWidget()->AddSkillBarWidget(SkillBarWidget);
	}
}

bool USkillsComponent::CanUseSkill(const int32 SkillSlotIndex)
{
	return false;
}

TPair<FName, FSkillTableRow*> USkillsComponent::GetSkillFromSkillSlot(const int32 SkillSlotIndex)
{
	if (!SkillBarWidget || !SkillTreeWidget)
	{
		return TPair<FName, FSkillTableRow*>(NAME_None, nullptr);
	}

	FString SkillGroupToUse;
	if (ActiveSupersedingChainSkillGroup.Key == SkillSlotIndex)
	{
		SkillGroupToUse = ActiveSupersedingChainSkillGroup.Value;
	}
	else
	{
		FString SlotSkillGroup = SkillBarWidget->GetSkillGroupAtIndex(SkillSlotIndex);
		// If no skill is equipped in current slot
		if (SlotSkillGroup == "" || SkillBarWidget->IsSkillInCooldown(SkillSlotIndex))
		{
			return TPair<FName, FSkillTableRow*>(NAME_None, nullptr);
		}

		SkillGroupToUse = SlotSkillGroup;
	}

#if MESSAGE_LOGGING_ENABLED
	FString LogMessage = FString("Skill group is: ") + SkillGroupToUse;
	UKismetSystemLibrary::PrintString(this, LogMessage);
#endif // MESSAGE_LOGGING_ENABLED

	FSkillState SkillState = SkillTreeWidget->GetSkillState(SkillGroupToUse);
	if (SkillState.CurrentUpgradeLevel == 0)
	{
		SkillState.CurrentUpgradeLevel = 1;
	}

	FString GenderPrefix;
	if (OwnerGender == ECharacterGender::Female)
	{
		GenderPrefix = FString("F_");
	}
	else
	{
		GenderPrefix = FString("M_");
	}

	FString SkillIDString = GenderPrefix + SkillGroupToUse + FString("_") + FString::FromInt(SkillState.CurrentUpgradeLevel);
	FName SkillID = FName(*SkillIDString);
	FSkillTableRow* Skill = UCharacterLibrary::GetPlayerSkill(SkillID, FString("USkillsComponent::GetSkillIDFromSkillSlot"));
	if (!Skill)
	{
		return TPair<FName, FSkillTableRow*>(NAME_None, nullptr);
	}

	if (Skill->PrecedingSkillGroups.Num() == 0)
	{
		return TPair<FName, FSkillTableRow*>(SkillID, Skill);
	}

	bool bUsedPrecedingSkill = false;
	for (const FString& SkillGroup : Skill->PrecedingSkillGroups)
	{
		if (SkillGroup == ActivePrecedingChainSkillGroup)
		{
			bUsedPrecedingSkill = true;
			break;
		}
	}

	if (bUsedPrecedingSkill)
	{
		return TPair<FName, FSkillTableRow*>(SkillID, Skill);
	}
	else
	{
		return TPair<FName, FSkillTableRow*>(NAME_None, nullptr);
	}
}

void USkillsComponent::OnSkillUsed(const int32 SkillSlotIndex, FName SkillID, const FSkillTableRow* Skill)
{
	if (SkillID == NAME_None || Skill == nullptr)
	{
		return;
	}

	FString SkillIDString = SkillID.ToString();
	FString SkillGroup = SkillIDString.Mid(2, 4);
	if (SkillGroup.EndsWith(FString("_")))
	{
		SkillGroup = SkillIDString.Mid(2, 3);
	}

	ActivePrecedingChainSkillGroup = SkillGroup;
	if (Skill->SupersedingSkillGroup == FString(""))
	{
		ActiveSupersedingChainSkillGroup.Key = -1;
		ActiveSupersedingChainSkillGroup.Value = FString("");
	}
	else
	{
		ActiveSupersedingChainSkillGroup.Key = SkillSlotIndex;
		ActiveSupersedingChainSkillGroup.Value = Skill->SupersedingSkillGroup;
	}


	GetWorld()->GetTimerManager().SetTimer(ChainSkillTimerHandle, this, &USkillsComponent::ResetChainSkill, ChainSkillResetDelay, false);

	SkillBarWidget->PutSkillOnCooldownTimer(SkillGroup, Skill->Cooldown, 0.5f);
}

void USkillsComponent::ResetChainSkill()
{
	ActivePrecedingChainSkillGroup = FString("");
	ActiveSupersedingChainSkillGroup = TPair<int32, FString>(-1, FString(""));
}
