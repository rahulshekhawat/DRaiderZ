// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "DynamicSkillBarWidget.h"
#include "ContainerWidget.h"
#include "PlayerSkillBase.h"
#include "PlayerSkillsComponent.h"

UDynamicSkillBarWidget::UDynamicSkillBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

bool UDynamicSkillBarWidget::Initialize()
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
		InitializeContainersParent();
		return true;
	}

	return false;
}

void UDynamicSkillBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDynamicSkillBarWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UDynamicSkillBarWidget::OnContainerDropped(UContainerWidget* FromContainer, UContainerWidget* ToChildContainer)
{
	if (!FromContainer || !ToChildContainer)
	{
		return false;
	}
	
	ToChildContainer->SetContainerData(FromContainer->GetContainerData());
	if (OwnerSkillsComponent.IsValid())
	{
		UPlayerSkillsComponent* SkillsComp = OwnerSkillsComponent.Get();
		uint8 SkillBarIndex = GetIndexOfSkillContainer(ToChildContainer);
		SkillsComp->OnSkillAddedToSkillBar(SkillBarIndex, FromContainer->GetContainerData().ItemGroup);
	}

	return true;
}

bool UDynamicSkillBarWidget::OnContainersSwapped(UContainerWidget* Container1, UContainerWidget* Container2)
{
	if (!Container1 || !Container2)
	{
		return false;
	}

	FContainerData ContData = Container1->GetContainerData();
	Container1->SetContainerData(Container2->GetContainerData());
	Container2->SetContainerData(ContData);

	return true;
}

bool UDynamicSkillBarWidget::OnContainerRemoved(UContainerWidget* Container)
{
	return false;
}

void UDynamicSkillBarWidget::InitializeContainersParent()
{
	for (int i = 1; i <= 20; i++)
	{
		UContainerWidget* TempWidget = GetContainerAtIndex(i);
		if (TempWidget)
		{
			TempWidget->InitializeWithParent(this);
		}
	}
}

void UDynamicSkillBarWidget::ResetSkillBar()
{
	for (int i = 1; i <= 20; i++)
	{
		UContainerWidget* TempWidget = GetContainerAtIndex(i);
		if (TempWidget)
		{
			TempWidget->ResetContainer();
		}
	}
}

void UDynamicSkillBarWidget::SetSkillOwnerComponent(UPlayerSkillsComponent* SkillsComponent)
{
	OwnerSkillsComponent = SkillsComponent;
}

void UDynamicSkillBarWidget::InitializeSkillBarLayout(const TMap<uint8, uint8>& SkillBarMap, const TMap<uint8, UGameplaySkillBase*>& SkillsMap)
{
	TArray<uint8> Keys;
	SkillBarMap.GetKeys(Keys);

	for (uint8 Key : Keys)
	{
		uint8 SkillKey = SkillBarMap[Key];
		if (SkillsMap.Contains(SkillKey))
		{
			UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(SkillsMap[SkillKey]);
			UContainerWidget* Cont = GetContainerAtIndex(Key);
			if (Skill && Cont)
			{
				Cont->SetIcon(Skill->GetSkillIcon());
				Cont->SetInGameName(Skill->GetInGameSkillName());
				Cont->SetDescription(Skill->GetInGameDescription());
				Cont->SetMaxValue(Skill->GetMaxUpgradeLevel());
				Cont->SetItemID(Skill->GetSkillGroup());
				Cont->SetItemGroup(Skill->GetSkillGroup());
			}
		}
	}
}
