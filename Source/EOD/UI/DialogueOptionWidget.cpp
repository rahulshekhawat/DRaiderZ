// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "DialogueOptionWidget.h"
#include "EOD/UI/DialogueWindowWidget.h"
#include "EOD/Characters/PlayerCharacter.h"

#include "Components/Button.h"

UDialogueOptionWidget::UDialogueOptionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UDialogueOptionWidget::Initialize()
{
	if (Super::Initialize() && OptionButton && OptionText)
	{
		OptionButton->OnClicked.AddDynamic(this, &UDialogueOptionWidget::OnOptionButtonClicked);
		return true;
	}

	return false;
}

void UDialogueOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDialogueOptionWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UDialogueOptionWidget::OnOptionButtonClicked()
{
	switch (OptionEventType)
	{
	case Dialogue:
		HandleNewDialogueEvent();
		break;
	case ArmorMenu:
		break;
	case WeaponMenu:
		break;
	case Exit:
		HandleExitEvent();
		break;
	case Finish:
		HandleFinishEvent();
		break;
	default:
		break;
	}
}

void UDialogueOptionWidget::HandleNewDialogueEvent()
{
	if (OwningDialogueWidget)
	{
		OwningDialogueWidget->UpdateDialogueWindow(OptionEventID);
	}
}

void UDialogueOptionWidget::HandleExitEvent()
{
	if (!OwningDialogueWidget || !OwningDialogueWidget->GetOwningPlayerPawn())
	{
		return;
	}

	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OwningDialogueWidget->GetOwningPlayerPawn());
	if (PlayerChar)
	{
		PlayerChar->ExitDialogue(OwningDialogueWidget);
	}
}

void UDialogueOptionWidget::HandleFinishEvent()
{
	if (!OwningDialogueWidget || !OwningDialogueWidget->GetOwningPlayerPawn())
	{
		return;
	}

	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OwningDialogueWidget->GetOwningPlayerPawn());
	if (PlayerChar)
	{
		PlayerChar->FinishDialogue(OwningDialogueWidget);
	}
}
