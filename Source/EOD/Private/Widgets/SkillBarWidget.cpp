// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillBarWidget.h"
#include "PlayerSkillsComponent.h"

USkillBarWidget::USkillBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USkillBarWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}

	return false;
}

void USkillBarWidget::NativeConstruct()
{
	Super::NativeDestruct();
}

void USkillBarWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void USkillBarWidget::InitializeSkillBarLayout(const TMap<uint8, uint8>& SkillBarMap, const TMap<uint8, UGameplaySkillBase*>& SkillsMap)
{
	//~ @todo
}

void USkillBarWidget::SetOwnerSkillsComponent(UPlayerSkillsComponent* SkillsComponent)
{
	OwnerSkillsComponent = SkillsComponent;
}
