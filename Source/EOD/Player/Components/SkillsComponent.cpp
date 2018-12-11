// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillsComponent.h"
#include "EOD/UI/HUDWidget.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/Core/EODSaveGame.h"
#include "EOD/Core/GameSingleton.h"
#include "EOD/Player/PlayerCharacter.h"
#include "EOD/StatusEffects/StatusEffectBase.h"

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

USkillBarWidget* USkillsComponent::BP_GetSkillBarWidget() const
{
	return GetSkillBarWidget();
}

USkillTreeWidget* USkillsComponent::BP_GetSkillTreeWidget() const
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
	if (!SkillBarWidget)
	{
		return;
	}

	// check(SkillBarWidget);

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
		SkillGroupToUse = ActiveSupersedingChainSkillGroup.Value;
	}
	else
	{
		SkillGroupToUse = SkillBarWidget->GetSkillGroupAtIndex(SkillSlotIndex);
	}

	if (SkillGroupToUse == FString("") || SkillBarWidget->IsSkillGroupInCooldown(SkillGroupToUse))
	{
		return TPair<FName, FSkillTableRow*>(NAME_None, nullptr);
	}

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

TPair<FName, FSkillTableRow*> USkillsComponent::GetChainSkillFromSkillSlot(const int32 SkillSlotIndex)
{
	if (!SkillBarWidget || !SkillTreeWidget || ActiveSupersedingChainSkillGroup.Key != SkillSlotIndex)
	{
		return TPair<FName, FSkillTableRow*>(NAME_None, nullptr);
	}

	FString SkillGroup = ActiveSupersedingChainSkillGroup.Value;
	if (SkillBarWidget->IsSkillGroupInCooldown(SkillGroup))
	{
		return TPair<FName, FSkillTableRow*>(NAME_None, nullptr);
	}

	FSkillState SkillState = SkillTreeWidget->GetSkillState(SkillGroup);
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

	FString SkillIDString = GenderPrefix + SkillGroup + FString("_") + FString::FromInt(SkillState.CurrentUpgradeLevel);
	FName SkillID = FName(*SkillIDString);
	FSkillTableRow* Skill = UCharacterLibrary::GetPlayerSkill(SkillID, FString("USkillsComponent::GetChainSkillFromSkillSlot"));
	if (!Skill)
	{
		return TPair<FName, FSkillTableRow*>(NAME_None, nullptr);
	}

	if (Skill->PrecedingSkillGroups.Num() == 0)
	{
		return TPair<FName, FSkillTableRow*>(SkillID, Skill);
	}

	bool bUsedPrecedingSkill = false;
	for (const FString& SGroup : Skill->PrecedingSkillGroups)
	{
		if (SGroup == ActivePrecedingChainSkillGroup)
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

TPair<FName, FSkillTableRow*> USkillsComponent::GetHitImmuneSkillFromSkillSlot(const int32 SkillSlotIndex)
{
	if (!SkillBarWidget || !SkillTreeWidget)
	{
		return TPair<FName, FSkillTableRow*>(NAME_None, nullptr);
	}

	FString SkillGroup = SkillBarWidget->GetSkillGroupAtIndex(SkillSlotIndex);
	FSkillState SkillState = SkillTreeWidget->GetSkillState(SkillGroup);
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

	FString SkillIDString = GenderPrefix + SkillGroup + FString("_") + FString::FromInt(SkillState.CurrentUpgradeLevel);
	FName SkillID = FName(*SkillIDString);
	FSkillTableRow* Skill = UCharacterLibrary::GetPlayerSkill(SkillID, FString("USkillsComponent::GetHitImmuneSkillFromSkillSlot"));

	if (Skill && Skill->bCanBeUsedWhileHit)
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
	if (Skill->SupersedingSkillGroup == FString("") || Skill->SupersedingSkillGroup == FString("None"))
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

	if (SkillGroupToStatusEffectMap.Contains(SkillGroup))
	{
		UStatusEffectBase* StatusEffect = SkillGroupToStatusEffectMap[SkillGroup];
		if (IsValid(StatusEffect))
		{
			StatusEffect->OnTriggerEvent(GetOwningEODPlayer());
		}
	}
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
			TSharedPtr<FStreamableHandle>& AnimHandle = SkillGroupAnimationStreamableHandles[SkillGroup];
			if (AnimHandle.IsValid())
			{
				AnimHandle.Get()->ReleaseHandle();
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
			TArray<FSoftObjectPath> AnimationsToLoad;
			if (!Skill->AnimMontage.IsNull())
			{
				AnimationsToLoad.Add(Skill->AnimMontage.ToSoftObjectPath());
			}
			if (!Skill->UpperBodyAnimMontage.IsNull())
			{
				AnimationsToLoad.Add(Skill->UpperBodyAnimMontage.ToSoftObjectPath());
			}

			TSharedPtr<FStreamableHandle> AnimHandle = GameSingleton->StreamableManager.RequestSyncLoad(AnimationsToLoad);
			// TSharedPtr<FStreamableHandle> AnimHandle = GameSingleton->StreamableManager.RequestSyncLoad(Skill->AnimMontage.ToSoftObjectPath());
			if (AnimHandle.IsValid())
			{
				SkillGroupAnimationStreamableHandles.Add(SkillGroup, AnimHandle);
			}
		}

		if (Skill->SupersedingSkillGroup != FString(""))
		{
			OnSkillGroupAddedToSkillBar(Skill->SupersedingSkillGroup);
		}

		if (SkillGroupToStatusEffectMap.Contains(SkillGroup))
		{
			SkillGroupToStatusEffectMap[SkillGroup]->MarkPendingKill();
			SkillGroupToStatusEffectMap.Remove(SkillGroup);
		}

		if (Skill->StatusEffect.Get())
		{
			// UStatusEffectBase* StatusEffect = NewObject<UStatusEffectBase>(GetTransientPackage(), Skill->StatusEffect.Get(), NAME_None, RF_Transient);
			UStatusEffectBase* StatusEffect = NewObject<UStatusEffectBase>(PlayerPawn, Skill->StatusEffect.Get(), NAME_None, RF_Transient);
			if (StatusEffect)
			{
				StatusEffect->Initialize(GetOwningEODPlayer(), nullptr);
				SkillGroupToStatusEffectMap.Add(SkillGroup, StatusEffect);
			}
		}
	}
}

void USkillsComponent::OnSkillGroupRemovedFromSkillBar(const FString& SkillGroup)
{
	if (SkillGroupAnimationStreamableHandles.Contains(SkillGroup))
	{
		TSharedPtr<FStreamableHandle>& AnimHandle = SkillGroupAnimationStreamableHandles[SkillGroup];
		if (AnimHandle.IsValid())
		{
			AnimHandle.Get()->ReleaseHandle();
			SkillGroupAnimationStreamableHandles.Remove(SkillGroup);
		}
	}

	if (SkillGroupToStatusEffectMap.Contains(SkillGroup))
	{
		SkillGroupToStatusEffectMap[SkillGroup]->MarkPendingKill();
		SkillGroupToStatusEffectMap.Remove(SkillGroup);
	}
}

void USkillsComponent::OnPressingSkillKey(const int32 SkillKeyIndex)
{
}

void USkillsComponent::OnReleasingSkillKey(const int32 SkillKeyIndex)
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

APlayerCharacter* USkillsComponent::GetOwningEODPlayer() const
{
	return Cast<APlayerCharacter>(GetOwner());
}