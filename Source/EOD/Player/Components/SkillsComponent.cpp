// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillsComponent.h"
#include "UI/HUDWidget.h"
#include "UI/SkillTreeWidget.h"
#include "Core/EODPreprocessors.h"
#include "Core/EODSaveGame.h"
#include "Core/GameSingleton.h"
#include "Player/PlayerCharacter.h"

#include "Engine/Engine.h"
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
	InitializeSkillBar();
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

void USkillsComponent::InitializeSkillBar()
{
	check(SkillBarWidget);

	for (int i = 1; i <= 20; i++)
	{
		FString SkillGroup = SkillBarWidget->GetSkillGroupAtIndex(i);
		OnSkillGroupAddedToSkillBar(SkillGroup);
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
#if MESSAGE_LOGGING_ENABLED
		FString LogMessage = FString("Chain key found. Skill group: ") + ActiveSupersedingChainSkillGroup.Value;
		UKismetSystemLibrary::PrintString(this, LogMessage);
#endif // MESSAGE_LOGGING_ENABLED
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

	SkillBarWidget->PutSkillOnCooldownTimer(SkillGroup, Skill->Cooldown, 0.5f);

	GetWorld()->GetTimerManager().ClearTimer(ChainSkillTimerHandle);
}

void USkillsComponent::SetOffChainSkillReset()
{
	GetWorld()->GetTimerManager().SetTimer(ChainSkillTimerHandle, this, &USkillsComponent::ResetChainSkill, ChainSkillResetDelay, false);
}

void USkillsComponent::OnSkillGroupAddedToSkillBar(const FString& SkillGroup)
{
	APlayerCharacter* PlayerPawn = GetOwningEODPlayer();
	if (!PlayerPawn || SkillGroup == FString(""))
	{
		return;
	}

	if (SkillTreeWidget)
	{
		FSkillState SkillState = SkillTreeWidget->GetSkillState(SkillGroup);

#if TEST_CODE_ENABLED
		if (SkillState.CurrentUpgradeLevel == 0)
		{
			SkillState.CurrentUpgradeLevel = 1;
		}
#endif // TEST_CODE_ENABLED

		FString SkillIDString = GetGenderPrefix() + SkillGroup + FString("_") + FString::FromInt(SkillState.CurrentUpgradeLevel);
		FSkillTableRow* Skill = PlayerPawn->GetSkill(FName(*SkillIDString));

		if (!Skill)
		{
			return;
		}

		if (SkillGroupAnimationStreamableHandles.Contains(SkillGroup))
		{
			TSharedPtr<FStreamableHandle>& Handle = SkillGroupAnimationStreamableHandles[SkillGroup];
			if (Handle.IsValid())
			{
				Handle.Get()->ReleaseHandle();
				SkillGroupAnimationStreamableHandles.Remove(SkillGroup);
			}
		}

		UGameSingleton* GameSingleton = nullptr;
		if (GEngine)
		{
			GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
		}
		
		if (GameSingleton)
		{
			TSharedPtr<FStreamableHandle> Handle = GameSingleton->StreamableManager.RequestSyncLoad(Skill->AnimMontage.ToSoftObjectPath());
			if (Handle.IsValid())
			{
				SkillGroupAnimationStreamableHandles.Add(SkillGroup, Handle);
			}
		}

		if (Skill->SupersedingSkillGroup != FString(""))
		{
			OnSkillGroupAddedToSkillBar(Skill->SupersedingSkillGroup);
		}
	}
}

void USkillsComponent::OnSkillGroupRemovedFromSkillBar(const FString& SkillGroup)
{
}

void USkillsComponent::ResetChainSkill()
{
	ActivePrecedingChainSkillGroup = FString("");
	ActiveSupersedingChainSkillGroup = TPair<int32, FString>(-1, FString(""));

#if MESSAGE_LOGGING_ENABLED
	UKismetSystemLibrary::PrintString(this, FString("Skill chain resetted"));
#endif // MESSAGE_LOGGING_ENABLED
}
