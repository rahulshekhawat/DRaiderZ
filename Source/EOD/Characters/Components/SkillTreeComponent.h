// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/CharacterLibrary.h"

#include "Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "SkillTreeComponent.generated.h"

class UGameplaySkillBase;
class USkillTreeWidget;
class USkillPointsInfoWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EOD_API USkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	USkillTreeComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

public:

	// --------------------------------------
	//	User Interface
	// --------------------------------------

	FORCEINLINE USkillTreeWidget* GetSkillTreeWidget() const { return SkillTreeWidget; }

	FORCEINLINE USkillPointsInfoWidget* GetSkillPointsInfoWidget() const { return SkillPointsInfoWidget; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<USkillPointsInfoWidget> SkillPointsInfoWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<USkillTreeWidget> SkillTreeWidgetClass;

private:

	UPROPERTY(Transient)
	USkillTreeWidget* SkillTreeWidget;

	UPROPERTY(Transient)
	USkillPointsInfoWidget* SkillPointsInfoWidget;

public:

	// --------------------------------------
	//	Skill Tree Layout
	// --------------------------------------

	FORCEINLINE TMap<FName, UGameplaySkillBase*> GetActivePlayerSkillsMap() const { return ActivePlayerSkillsMap; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Layout")
	UDataTable* SkillTreeLayoutTable;

	UPROPERTY(Transient)
	TMap<FName, UGameplaySkillBase*> ActivePlayerSkillsMap;

};
