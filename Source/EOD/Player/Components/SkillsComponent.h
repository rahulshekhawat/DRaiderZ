// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/CharacterLibrary.h"
#include "Components/ActorComponent.h"
#include "SkillsComponent.generated.h"

class USkillBarWidget;
class USkillTreeWidget;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EOD_API USkillsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// Sets default values for this component's properties
	USkillsComponent(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Returns skill bar widget */
	FORCEINLINE USkillBarWidget* GetSkillBarWidget() const;

	/** Returns skill bar widget */
	UFUNCTION(BlueprintPure, Category = UI, meta = (DisplayName = "Get Skill Bar Widget"))
	USkillBarWidget* BP_GetSkillBarWidget() const;

	/** Returns skill bar widget */
	FORCEINLINE USkillTreeWidget* GetSkillTreeWidget() const;

	/** Returns skill bar widget */
	UFUNCTION(BlueprintPure, Category = UI, meta = (DisplayName = "Get Skill Tree Widget"))
	USkillTreeWidget* BP_GetSkillTreeWidget() const;

	/** Toggle the display of skill tree UI in player viewport */
	void ToggleSkillTreeUI();

	void InitializeComponentWidgets();

	/** Returns true if player can use skill placed at given skill slot index */
	bool CanUseSkill(const int32 SkillSlotIndex);

	/**
	 * Returns SkillID and skill struct pointer of skill at given skill slot index
	 * This will return <NAME_None, nullptr> if no skill is equipped in given skill slot or if the skill is in cooldown (skill unavailable for use)
	 * However it will return <SkillID, nullptr> for chain skill if it available for use
	 */
	TPair<FName, FSkillTableRow*> GetSkillFromSkillSlot(const int32 SkillSlotIndex);

	void OnSkillUsed(const int32 SkillSlotIndex, FName SkillID, const FSkillTableRow* Skill);

protected:

	UPROPERTY(BlueprintReadOnly, Category = Skills)
	float ChainSkillResetDelay;

private:

	UPROPERTY(Transient)
	USkillBarWidget* SkillBarWidget;

	UPROPERTY(EditAnywhere, Category = Widgets)
	TSubclassOf<USkillBarWidget> SkillBarWidgetClass;

	UPROPERTY(Transient)
	USkillTreeWidget* SkillTreeWidget;

	UPROPERTY(EditAnywhere, Category = Widgets)
	TSubclassOf<USkillTreeWidget> SkillTreeWidgetClass;

	UPROPERTY(Transient)
	ECharacterGender OwnerGender;

	TPair<int32, FString> ActiveSupersedingChainSkillGroup;

	FString ActivePrecedingChainSkillGroup;

	void ResetChainSkill();

	FTimerHandle ChainSkillTimerHandle;

};

FORCEINLINE USkillBarWidget* USkillsComponent::GetSkillBarWidget() const
{
	return SkillBarWidget;
}

FORCEINLINE USkillTreeWidget* USkillsComponent::GetSkillTreeWidget() const
{
	return SkillTreeWidget;
}
