// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/UI/SkillTreeWidget.h"
#include "EOD/UI/SkillBarWidget.h"
#include "EOD/Statics/CharacterLibrary.h"

#include "Engine/StreamableManager.h"
#include "Components/ActorComponent.h"
#include "SkillsComponent.generated.h"

class USkillBarWidget;
class USkillTreeWidget;
class UStatusEffectBase;
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

	/** Returns skill tree widget */
	FORCEINLINE USkillTreeWidget* GetSkillTreeWidget() const;

	/** Returns skill tree widget */
	UFUNCTION(BlueprintPure, Category = UI, meta = (DisplayName = "Get Skill Tree Widget"))
	USkillTreeWidget* BP_GetSkillTreeWidget() const;

	/** Toggle the display of skill tree UI in player viewport */
	void ToggleSkillTreeUI();

	/** Returns true if player can use skill placed at given skill slot index */
	bool CanUseSkill(const int32 SkillSlotIndex);

	/**
	 * Returns SkillID and skill struct pointer of skill at given skill slot index
	 * This will return <NAME_None, nullptr> if no skill is equipped in given skill slot or if the skill is in cooldown (skill unavailable for use)
	 * However it will return <SkillID, FSkillTableRow*> for chain skill if it available for use
	 */
	TPair<FName, FSkillTableRow*> GetSkillFromSkillSlot(const int32 SkillSlotIndex);

	/**
	 * Returns SkillID and skill struct pointer of chain skill at the give nSkillSlotIndex
	 */
	TPair<FName, FSkillTableRow*> GetChainSkillFromSkillSlot(const int32 SkillSlotIndex);

	TPair<FName, FSkillTableRow*> GetHitImmuneSkillFromSkillSlot(const int32 SkillSlotIndex);

	void OnSkillUsed(const int32 SkillSlotIndex, FName SkillID, const FSkillTableRow* Skill);

	void SetOffChainSkillReset();

	void OnSkillGroupAddedToSkillBar(const FString& SkillGroup);

	void OnSkillGroupRemovedFromSkillBar(const FString& SkillGroup);

	void OnPressingSkillKey(const int32 SkillKeyIndex);

	void OnReleasingSkillKey(const int32 SkillKeyIndex);

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

	FTimerHandle ChainSkillTimerHandle;

	TMap<FString, TSharedPtr<FStreamableHandle>> SkillGroupAnimationStreamableHandles;

	//~ Marked uproperty to prevent garbage collection
	UPROPERTY()
	TMap<FString, UStatusEffectBase*> SkillGroupToStatusEffectMap;

	void ResetChainSkill();

	/** Get the owning EOD player of this component */
	APlayerCharacter* GetOwningEODPlayer() const;

	FORCEINLINE FString GetGenderPrefix() const;

	void InitializeComponentWidgets();

	void InitializeSkillBar();

};

FORCEINLINE USkillBarWidget* USkillsComponent::GetSkillBarWidget() const
{
	return SkillBarWidget;
}

FORCEINLINE USkillTreeWidget* USkillsComponent::GetSkillTreeWidget() const
{
	return SkillTreeWidget;
}

FORCEINLINE FString USkillsComponent::GetGenderPrefix() const
{
	if (OwnerGender == ECharacterGender::Female)
	{
		return FString("F_");
	}
	else
	{
		return FString("M_");
	}
}