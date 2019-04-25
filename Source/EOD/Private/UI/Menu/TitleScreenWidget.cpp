// Copyright 2018 Moikkai Games. All Rights Reserved.


#include "TitleScreenWidget.h"
#include "EODGameInstance.h"

#include "Components/TextBlock.h"

UTitleScreenWidget::UTitleScreenWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UTitleScreenWidget::Initialize()
{
	if (Super::Initialize() &&
		TitleText &&
		CreditText)
	{
		UEODGameInstance* GI = Cast<UEODGameInstance>(GetGameInstance());
		if (GI)
		{
			TitleText->SetText(GI->GameTitle.ToUpper());
		}

		SetRenderOpacity(0.f);
		return true;
	}
	
	return false;
}

void UTitleScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTitleScreenWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
