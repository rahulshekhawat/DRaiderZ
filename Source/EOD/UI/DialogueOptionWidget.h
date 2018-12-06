// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/DialogueLibrary.h"

#include "Blueprint/UserWidget.h"
#include "DialogueOptionWidget.generated.h"

class UButton;
class UTextBlock;
class UDialogueWindowWidget;

/**
 * 
 */
UCLASS()
class EOD_API UDialogueOptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UDialogueOptionWidget(const FObjectInitializer& ObjectInitializer);

	bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* OptionButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* OptionText;

 	FORCEINLINE void SetOptionEventID(FName NewOptionEventID);

	FORCEINLINE void SetOptionEventType(TEnumAsByte<EDialogueEventType> NewOptionEventType);

	FORCEINLINE void SetOwningDialogueWidget(UDialogueWindowWidget* NewOwner);

	FORCEINLINE void SetOptionSelected(bool bValue);

	FORCEINLINE bool IsOptionSelected() const;

protected:

	UPROPERTY(Transient, BlueprintReadWrite, Category = DialogueWidget)
	bool bOptionSelected;

	UPROPERTY(Transient, BlueprintReadWrite, Category = DialogueWidget)
	TEnumAsByte<EDialogueEventType> OptionEventType;

	UPROPERTY(Transient, BlueprintReadWrite, Category = DialogueWidget)
	FName OptionEventID;

	UPROPERTY(Transient, BlueprintReadWrite, Category = DialogueWidget)
	UDialogueWindowWidget* OwningDialogueWidget;

	UFUNCTION(BlueprintCallable, Category = DialogueWidget)
	void OnOptionButtonClicked();

	UFUNCTION(BlueprintCallable, Category = DialogueWidget)
	void HandleNewDialogueEvent();

	UFUNCTION(BlueprintCallable, Category = DialogueWidget)
	void HandleExitEvent();

	UFUNCTION(BlueprintCallable, Category = DialogueWidget)
	void HandleFinishEvent();

};

FORCEINLINE void UDialogueOptionWidget::SetOptionEventID(FName NewOptionEventID)
{
	OptionEventID = NewOptionEventID;
}

FORCEINLINE void UDialogueOptionWidget::SetOptionEventType(TEnumAsByte<EDialogueEventType> NewOptionEventType)
{
	OptionEventType = NewOptionEventType;
}

FORCEINLINE void UDialogueOptionWidget::SetOwningDialogueWidget(UDialogueWindowWidget * NewOwner)
{
	OwningDialogueWidget = NewOwner;
}

FORCEINLINE void UDialogueOptionWidget::SetOptionSelected(bool bValue)
{
	bOptionSelected = bValue;
}

FORCEINLINE bool UDialogueOptionWidget::IsOptionSelected() const
{
	return bOptionSelected;
}
