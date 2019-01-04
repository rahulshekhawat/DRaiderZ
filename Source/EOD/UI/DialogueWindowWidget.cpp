// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "DialogueWindowWidget.h"
#include "EOD/Core/GameSingleton.h"
#include "EOD/Statics/DialogueLibrary.h"
#include "EOD/UI/DialogueOptionWidget.h"

#include "Engine/Engine.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

#include "Kismet/KismetSystemLibrary.h"

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
	UGameSingleton* GameSingleton = IsValid(GEngine) ? Cast<UGameSingleton>(GEngine->GameSingleton) : nullptr;
	if (IsValid(GameSingleton) && IsValid(GameSingleton->DialogueWindowsDataTable))
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

			if (DialogueOptions.Num() > 0 && IsValid(DialogueOptions[0]))
			{
				DialogueOptions[0]->SetOptionSelected(true);
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

	UGameSingleton* GameSingleton = IsValid(GEngine) ? Cast<UGameSingleton>(GEngine->GameSingleton) : nullptr;
	if (IsValid(GameSingleton) && IsValid(GameSingleton->DialogueOptionsDataTable))
	{
		FDialogueOption* DialogueOpt = GameSingleton->DialogueOptionsDataTable->FindRow<FDialogueOption>(OptionID, FString("UDialogueWindowWidget::AddOption() : Searching for dialogue option row"));
		if (DialogueOpt)
		{
			UDialogueOptionWidget* NewOptionWidget = CreateWidget<UDialogueOptionWidget>(GetGameInstance(), DialogueOptionWidgetClass);
			if (NewOptionWidget)
			{
				FText Text = FText::FromString(DialogueOpt->OptionText);
				NewOptionWidget->OptionText->SetText(Text);
				NewOptionWidget->SetOptionEventID(DialogueOpt->EventID);
				NewOptionWidget->SetOptionEventType(DialogueOpt->EventType);
				NewOptionWidget->SetOwningDialogueWidget(this);

				UVerticalBoxSlot* VBSlot = VertiBox ? VertiBox->AddChildToVerticalBox(NewOptionWidget) : nullptr;
				if (VBSlot)
				{
					FMargin SlotPadding = FMargin(0.f, 10.f, 0.f, 0.f);
					VBSlot->SetPadding(SlotPadding);
				}

				DialogueOptions.Add(NewOptionWidget);
			}
		}
	}
}

void UDialogueWindowWidget::CleanupOptions_Implementation()
{
	if (IsValid(VertiBox))
	{
		for (UDialogueOptionWidget* OptionWidget : DialogueOptions)
		{
			VertiBox->RemoveChild(OptionWidget);
		}
	}
	DialogueOptions.Empty();
}

void UDialogueWindowWidget::SimulateSelectedOptionClick()
{
	UDialogueOptionWidget* OptionWidget = nullptr;
	for (UDialogueOptionWidget* DialogueOption : DialogueOptions)
	{
		if (IsValid(DialogueOption) && DialogueOption->IsOptionSelected())
		{
			OptionWidget = DialogueOption;
			break;
		}
	}
	if (IsValid(OptionWidget) && IsValid(OptionWidget->OptionButton))
	{
		OptionWidget->OptionButton->OnClicked.Broadcast();
	}
}
