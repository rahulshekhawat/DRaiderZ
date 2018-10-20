// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillBarComponent.h"
#include "Player/PlayerCharacter.h"
#include "UI/SkillBarWidget.h"
#include "UI/HUDWidget.h"


USkillBarComponent::USkillBarComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(false);
}

void USkillBarComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeComponentWidget();
}

void USkillBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FORCEINLINE USkillBarWidget* USkillBarComponent::GetSkillBarWidget() const
{
	return SkillBarWidget;
}

USkillBarWidget* USkillBarComponent::BP_GetSkillBarWidget() const
{
	return GetSkillBarWidget();
}

FORCEINLINE void USkillBarComponent::InitializeComponentWidget()
{
	APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetOwner());
	if (!(OwningPlayer && OwningPlayer->IsLocallyControlled() && OwningPlayer->GetHUDWidget()))
	{
		return;
	}

	if (SkillBarWidgetClass.Get())
	{
		SkillBarWidget = CreateWidget<USkillBarWidget>(OwningPlayer->GetGameInstance(), SkillBarWidgetClass);
		OwningPlayer->GetHUDWidget()->AddSkillBarWidget(SkillBarWidget);
	}
}

FORCEINLINE bool USkillBarComponent::CanUseSkill(const int32 SkillSlotIndex)
{
	if (!SkillBarWidget || !SkillBarWidget->IsSkillInCooldown(SkillSlotIndex))
	{
		return false;
	}

	// FName SkillID = SkillBarWidget->GetSkillAtIndex(SkillSlotIndex);
	// FPlayerSkillTableRow* Skill = 

	return false;
}

FName USkillBarComponent::GetSkillIDFromSkillSlot(const int32 SkillSlotIndex)
{
	FName SkillID = NAME_None;



	return SkillID;
}

/*
void USkillBarComponent::OnSkillUsed(FName SkillID, const FPlayerSkillTableRow* Skill)
{
	// @todo put skill on cooldown
	// @todo set superseding skill group
}
*/
