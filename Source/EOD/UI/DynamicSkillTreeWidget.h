// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DynamicSkillTreeWidget.generated.h"

class UCanvasPanel;
class USkillTreeItemContainer;

/**
 * 
 */
UCLASS()
class EOD_API UDynamicSkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	UDynamicSkillTreeWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//	Necessary Child Widget
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* AssassinCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* BerserkerCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* ClericCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* DefenderCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* SorcererCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* AssassinInfoCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* BerserkerInfoCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* ClericInfoCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* DefenderInfoCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* SorcererInfoCanvas;

private:

	/** A list of all the assassin skills available in the skill tree */
	UPROPERTY(Transient)
	TArray<USkillTreeItemContainer*> AssassinSkills;

	/** A list of all the berserker skills available in the skill tree */
	UPROPERTY(Transient)
	TArray<USkillTreeItemContainer*> BerserkerSkills;

	/** A list of all the cleric skills available in the skill tree */
	UPROPERTY(Transient)
	TArray<USkillTreeItemContainer*> ClericSkills;

	/** A list of all the defender skills available in the skill tree */
	UPROPERTY(Transient)
	TArray<USkillTreeItemContainer*> DefenderSkills;

	/** A list of all the sorcerer skills available in the skill tree */
	UPROPERTY(Transient)
	TArray<USkillTreeItemContainer*> SorcererSkills;




};
