// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLibrary.h"

#include "Engine/DataTable.h"
#include "GameFramework/Info.h"
#include "PlayerSkillTreeManager.generated.h"

class UEODGameInstance;
class USkillTreeWidget;
class UGameplaySkillBase;

/**
 * Skill tree manager class for locally controlled player
 */
UCLASS(BlueprintType, Blueprintable)
class EOD_API APlayerSkillTreeManager : public AInfo
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	APlayerSkillTreeManager(const FObjectInitializer& ObjectInitializer);

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;

public:

	// --------------------------------------
	//	Skill Tree
	// --------------------------------------

	void CreateSkillTreeWidget(UEODGameInstance* GameInstance = nullptr);

	FORCEINLINE USkillTreeWidget* GetSkillTreeWidget() const { return SkillTreeWidget; }

protected:

	/** A map of skill group to it's skill tree slot info */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Layout")
	TMap<FName, FSkillTreeSlot> SkillTreeLayout;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<USkillTreeWidget> SkillTreeWidgetClass;

	UPROPERTY(Transient)
	USkillTreeWidget* SkillTreeWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Layout")
	UDataTable* SkillTreeLayoutTable;


};
