// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWindowWidget.generated.h"

class URichTextBlock;
class UDialogueOptionWidget;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class EOD_API UDialogueWindowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDialogueWindowWidget(const FObjectInitializer& ObjectInitializer);

	bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	URichTextBlock* DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VertiBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DialogueWidget)
	FText DefaultDialogueText;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = DialogueWidget)
	void UpdateDialogueWindow(FName DialogueWindowID);

	void UpdateDialogueWindow_Implementation(FName DialogueWindowID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = DialogueWidget)
	void MoveToNextDialogue();

	void MoveToNextDialogue_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = DialogueWidget)
	void AddOption(FName OptionID);

	void AddOption_Implementation(FName OptionID);

	FORCEINLINE FName GetDialogueWindowID() const;

	void SimulateSelectedOptionClick();

	UPROPERTY(Transient, BlueprintReadWrite, Category = DialogueWidget)
	TArray<UDialogueOptionWidget*> DialogueOptions;

protected:
	UPROPERTY(Transient, BlueprintReadWrite, Category = DialogueWidget)
	FName CurrentDialogueWindowID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = DialogueWidget)
	TSubclassOf<UDialogueOptionWidget> DialogueOptionWidgetClass;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = DialogueWidget)
	void CleanupOptions();

	void CleanupOptions_Implementation();

};

FName UDialogueWindowWidget::GetDialogueWindowID() const
{
	return CurrentDialogueWindowID;
}
