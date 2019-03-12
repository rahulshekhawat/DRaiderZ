// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillPointsInfoWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class EOD_API USkillPointsInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USkillPointsInfoWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//	Necessary Child Widget
	// --------------------------------------

	/** A text block displaying the number of available skill points */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* AvailableSkillPointsNumText;

	/** A text block displaying the number of used skill points */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* UsedSkillPointsNumText;

	/** A text block displaying the number of used skill points in assassin tree */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* AssassinPointsNumText;

	/** A text block displaying the number of used skill points in berserker tree */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* BerserkerPointsNumText;

	/** A text block displaying the number of used skill points in cleric tree */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ClericPointsNumText;

	/** A text block displaying the number of used skill points in defender tree */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DefenderPointsNumText;

	/** A text block displaying the number of used skill points in sorcerer tree */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* SorcererPointsNumText;

};
