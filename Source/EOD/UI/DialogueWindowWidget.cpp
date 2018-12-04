// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "DialogueWindowWidget.h"
#include "Core/GameSingleton.h"
#include "Statics/DialogueLibrary.h"
#include "UI/DialogueOptionWidget.h"

#include "Button.h"
#include "Engine/Engine.h"
#include "RichTextBlock.h"
#include "TextBlock.h"
#include "VerticalBox.h"
#include "VerticalBoxSlot.h"

UDialogueWindowWidget::UDialogueWindowWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultDialogueText = NSLOCTEXT("eod-namespace", "eng-us", "DEFAULT TEXT\n\n<IF YOU ARE READING THIS THEN ACTUAL DIALOGUE TEXT IS MISSING>");

}

bool UDialogueWindowWidget::Initialize()
{
	if (Super::Initialize() && DialogueText && VertiBox)
	{
		return true;
	}

	return false;
}

void UDialogueWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDialogueWindowWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UDialogueWindowWidget::UpdateDialogueWindow_Implementation(FName DialogueWindowID)
{
	UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
	if (GameSingleton)
	{
		FDialogueWindow* DialogueWin = GameSingleton->DialogueWindowsDataTable->FindRow<FDialogueWindow>(DialogueWindowID, FString("UpdateDialogueWindow() : Looking for dialogue window row"));
		if (DialogueWin)
		{
			CleanupOptions();
			FText Text = FText::FromString(DialogueWin->DialogueText);
			DialogueText->SetText(Text);
			CurrentDialogueWindowID = DialogueWindowID;

			for (FName OptionID : DialogueWin->OptionIDs)
			{
				AddOption(OptionID);
			}
		}
	}
}

void UDialogueWindowWidget::MoveToNextDialogue_Implementation()
{
}

void UDialogueWindowWidget::AddOption_Implementation(FName OptionID)
{
	if (!DialogueOptionWidgetClass.Get())
	{
		return;
	}

	UGameSingleton* GameSingleton = Cast<UGameSingleton>(GEngine->GameSingleton);
	if (GameSingleton)
	{
		FDialogueOption* DialogueOpt = GameSingleton->DialogueOptionsDataTable->FindRow<FDialogueOption>(OptionID, FString("UDialogueWindowWidget::AddOption() : Searching for dialogue option row"));
		if (DialogueOpt)
		{
			// UDialogueOptionWidget* NewOptionWidget = CreateWidget<UDialogueOptionWidget>(GetGameInstance(), UDialogueOptionWidget::StaticClass());
			UDialogueOptionWidget* NewOptionWidget = CreateWidget<UDialogueOptionWidget>(GetGameInstance(), DialogueOptionWidgetClass);
			FText Text = FText::FromString(DialogueOpt->OptionText);
			NewOptionWidget->OptionText->SetText(Text);
			NewOptionWidget->SetOptionEventID(DialogueOpt->EventID);
			NewOptionWidget->SetOptionEventType(DialogueOpt->EventType);
			NewOptionWidget->SetOwningDialogueWidget(this);

			UVerticalBoxSlot* VBSlot = VertiBox->AddChildToVerticalBox(NewOptionWidget);
			FMargin SlotPadding = FMargin(0.f, 10.f, 0.f, 0.f);
			VBSlot->SetPadding(SlotPadding);

			DialogueOptions.Add(NewOptionWidget);
		}
	}
}

/*
void UDialogueWindowWidget::FocusOnFirstOption()
{
	if (DialogueOptions.Num() > 0)
	{
		DialogueOptions[0]->OptionButton->SetKeyboardFocus();
	}
}
*/

void UDialogueWindowWidget::CleanupOptions_Implementation()
{
	for (UDialogueOptionWidget* OptionWidget : DialogueOptions)
	{
		VertiBox->RemoveChild(OptionWidget);
	}
}
