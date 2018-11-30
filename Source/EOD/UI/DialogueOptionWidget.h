// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/DialogueLibrary.h"

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

protected:

	UPROPERTY(Transient, BlueprintReadWrite, Category = DialogueWidget)
	TEnumAsByte<EDialogueEventType> EventType;

	UPROPERTY(Transient, BlueprintReadWrite, Category = DialogueWidget)
	FName EventID;

	UPROPERTY(Transient, BlueprintReadWrite, Category = DialogueWidget)
	UDialogueWindowWidget* OwningDialogueWidget;


};
