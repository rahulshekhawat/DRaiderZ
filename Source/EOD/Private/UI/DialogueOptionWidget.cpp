// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "DialogueOptionWidget.h"
#include "DialogueWindowWidget.h"
#include "PlayerCharacter.h"

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
	if (IsValid(ParentDialogueWidget))
	{
		ParentDialogueWidget->UpdateDialogueWindow(OptionEventID);
	}
}

void UDialogueOptionWidget::HandleExitEvent()
{
	/*
	if (!IsValid(ParentDialogueWidget) || !IsValid(ParentDialogueWidget->GetOwningPlayerPawn()))
	{
		return;
	}

	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(ParentDialogueWidget->GetOwningPlayerPawn());
	if (PlayerChar)
	{
		PlayerChar->ExitDialogue(ParentDialogueWidget);
	}
	*/
}

void UDialogueOptionWidget::HandleFinishEvent()
{
	/*
	if (!ParentDialogueWidget || !ParentDialogueWidget->GetOwningPlayerPawn())
	{
		return;
	}

	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(ParentDialogueWidget->GetOwningPlayerPawn());
	if (PlayerChar)
	{
		PlayerChar->FinishDialogue(ParentDialogueWidget);
	}
	*/
}
