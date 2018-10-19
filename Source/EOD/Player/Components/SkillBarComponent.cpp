// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillBarComponent.h"
#include "Player/PlayerCharacter.h"
#include "UI/SkillBarWidget.h"
#include "UI/HUDWidget.h"

#include "Components/CanvasPanel.h"
#include "Kismet/KismetSystemLibrary.h"

USkillBarComponent::USkillBarComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(false);
}

void USkillBarComponent::BeginPlay()
{
	Super::BeginPlay();	
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
		OwningPlayer->GetHUDWidget()->BP_AddSkillBarWidgetToCanvas(SkillBarWidget);
	}
}

bool USkillBarComponent::CanUseSkill(const int32 SkillSlotIndex)
{
	return false;
}
