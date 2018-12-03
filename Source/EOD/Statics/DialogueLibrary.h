// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "DialogueLibrary.generated.h"


UENUM(BlueprintType)
enum EDialogueEventType
{
	Dialogue,	// Triggers a new dialogue
	// Option,		// Replaces current dialogue options with new options
	ArmorMenu,	// Opens armor craft menu
	WeaponMenu,	// Opens weapon craft menu
	Exit,		// Exits dialogue. Implies the interacting player wants to finish dialogue.
	Finish,		// Same as Exit event but it implies that the dialogue sequence is over.
};

USTRUCT(BlueprintType)
struct EOD_API FDialogueOption : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Option")
	FString OptionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Option")
	TEnumAsByte<EDialogueEventType> EventType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue Option")
	FName EventID;

};

/** Struct for in-game skills */
USTRUCT(BlueprintType)
struct EOD_API FDialogueWindow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FString DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (DisplayName = "Option IDs"))
	TArray<FName> OptionIDs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FName NextDialogueID;

};


USTRUCT(BlueprintType)
struct EOD_API FNPCDialogueTriggers : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:



};


/**
 * 
 */
UCLASS()
class EOD_API UDialogueLibrary : public UObject
{
	GENERATED_BODY()

public:


	
};
