// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "DynamicSkillBarWidget.h"
#include "ContainerWidget.h"
#include "PlayerSkillBase.h"
#include "PlayerSkillsComponent.h"

#include "Components/Image.h"

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

	if (OwnerSkillsComponent.IsValid())
	{
		UPlayerSkillsComponent* SkillsComp = OwnerSkillsComponent.Get();
		uint8 SkillBarIndex = GetIndexOfSkillContainer(ToChildContainer);
		bool bResult = SkillsComp->AddSkillToSkillBar(SkillBarIndex, FromContainer->GetContainerData().ItemGroup);
		if (bResult)
		{
			ToChildContainer->SetContainerData(FromContainer->GetContainerData());
			
			uint8 SkillIndex = SkillsComp->GetSkillBarMap()[SkillBarIndex];
			UPlayerSkillBase* Skill = Cast<UPlayerSkillBase>(SkillsComp->GetSkillsMap()[SkillIndex]);
			check(Skill);
			bool bCanActivate = Skill->CanPlayerActivateThisSkill();
			if (bCanActivate)
			{
				ToChildContainer->ItemImage->SetIsEnabled(true);
				ToChildContainer->SetCanBeClicked(true);
			}
			else
			{
				ToChildContainer->ItemImage->SetIsEnabled(false);
				ToChildContainer->SetCanBeClicked(false);
			}
			
			return true;
		}
	}

	return false;
}

bool UDynamicSkillBarWidget::OnContainersSwapped(UContainerWidget* Container1, UContainerWidget* Container2)
{
	if (!Container1 || !Container2)
	{
		return false;
	}

	if (OwnerSkillsComponent.IsValid())
	{
		UPlayerSkillsComponent* SkillsComp = OwnerSkillsComponent.Get();

		uint8 SBI1 = GetIndexOfSkillContainer(Container1);
		uint8 SBI2 = GetIndexOfSkillContainer(Container2);

		FName SG1 = Container1->GetContainerData().ItemGroup;
		FName SG2 = Container2->GetContainerData().ItemGroup;

		bool bResult = SkillsComp->SwapSkillsOnSkillBar(SBI1, SG1, SBI2, SG2);
		if (bResult)
		{
			FContainerData ContData = Container1->GetContainerData();
			Container1->SetContainerData(Container2->GetContainerData());
			Container2->SetContainerData(ContData);
			return true;
		}
	}

	return false;
}

bool UDynamicSkillBarWidget::OnContainerRemoved(UContainerWidget* Container)
{
	if (!Container)
	{
		return false;
	}

	if (OwnerSkillsComponent.IsValid())
	{
		UPlayerSkillsComponent* SkillsComp = OwnerSkillsComponent.Get();
		uint8 SkillBarIndex = GetIndexOfSkillContainer(Container);
		bool bResult = SkillsComp->RemoveSkillFromSkillBar(SkillBarIndex, Container->GetContainerData().ItemGroup);
		if (bResult)
		{
			Container->SetContainerData(FContainerData());
			return true;
		}
	}

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

				if (Skill->CanPlayerActivateThisSkill())
				{
					Cont->ItemImage->SetIsEnabled(true);
					Cont->SetCanBeClicked(true);
				}
				else
				{
					Cont->ItemImage->SetIsEnabled(false);
					Cont->SetCanBeClicked(false);
				}
			}
		}
	}
}
