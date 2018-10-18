// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillBarComponent.h"

USkillBarComponent::USkillBarComponent()
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

FORCEINLINE APlayerCharacter * USkillBarComponent::GetOwningPlayer() const
{
	return OwningPlayer;
}

FORCEINLINE void USkillBarComponent::SetOwningPlayer(APlayerCharacter * NewOwner)
{
	OwningPlayer = NewOwner;
}

