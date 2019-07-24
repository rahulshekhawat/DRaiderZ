// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "SkillBarWidget.h"
#include "PlayerSkillBase.h"
#include "PlayerSkillsComponent.h"
#include "SkillBarContainerWidget.h"

USkillBarWidget::USkillBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool USkillBarWidget::Initialize()
{
	if (Super::Initialize() &&
		SkillContainer_1 &&
		SkillContainer_2 &&
		SkillContainer_3 &&
		SkillContainer_4 &&
		SkillContainer_5 &&
		SkillContainer_6 &&
		SkillContainer_7 &&
		SkillContainer_8 &&
		SkillContainer_9 &&
		SkillContainer_10 &&
		SkillContainer_11 &&
		SkillContainer_12 &&
		SkillContainer_13 &&
		SkillContainer_14 &&
		SkillContainer_15 &&
		SkillContainer_16 &&
		SkillContainer_17 &&
		SkillContainer_18 &&
		SkillContainer_19 &&
		SkillContainer_20)
	{
		for (int i = 1; i <= 20; i++)
		{
			USkillBarContainerWidget* Cont = GetContainerAtIndex(i);
			Cont->SkillBarIndex = i;
		}

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
	TArray<uint8> Keys;
	SkillBarMap.GetKeys(Keys);

	for (uint8 Key : Keys)
	{
		uint8 SkillKey = SkillBarMap[Key];
		if (SkillsMap.Contains(SkillKey))
		{
			UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(SkillsMap[SkillKey]);
			USkillBarContainerWidget* Cont = GetContainerAtIndex(Key);
			if (Skill && Cont)
			{
				Cont->SetDataObj(Skill);
			}
		}
	}
}

void USkillBarWidget::SetOwnerSkillsComponent(UPlayerSkillsComponent* SkillsComponent)
{
	OwnerSkillsComponent = SkillsComponent;
}

uint8 USkillBarWidget::GetIndexOfSkillBarContainer(USkillBarContainerWidget* Container) const
{
	return Container ? Container->SkillBarIndex : 0;
}
