// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "SkillTreeComponent.h"

USkillTreeComponent::USkillTreeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(false);
}

void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void USkillTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FORCEINLINE USkillTreeWidget * USkillTreeComponent::GetSkillTreeWidget() const
{
	return SkillTreeWidget;
}

USkillTreeWidget * USkillTreeComponent::BP_GetSkillTreeWidget() const
{
	return GetSkillTreeWidget();
}

FORCEINLINE APlayerCharacter * USkillTreeComponent::GetOwningPlayer() const
{
	return OwningPlayer;
}

FORCEINLINE void USkillTreeComponent::SetOwningPlayer(APlayerCharacter * NewOwner)
{
	OwningPlayer = NewOwner;
}

