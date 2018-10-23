// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/CharacterLibrary.h"
#include "Components/ActorComponent.h"
#include "SkillBarComponent.generated.h"

class USkillBarWidget;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EOD_API USkillBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillBarComponent(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Returns skill bar widget */
	FORCEINLINE USkillBarWidget* GetSkillBarWidget() const;

	/** Returns skill bar widget */
	UFUNCTION(BlueprintPure, Category = UI, meta = (DisplayName = "Get Skill Bar Widget"))
	USkillBarWidget* BP_GetSkillBarWidget() const;

	void InitializeComponentWidget();

	/** Returns true if player can use skill placed at given skill slot index */
	bool CanUseSkill(const int32 SkillSlotIndex);

	/**
	 * Returns SkillID of skill at given skill slot index
	 * This will return NAME_None if no skill is equipped in given skill slot or if the skill is in cooldown (skill unavailable for use)
	 * However it will return the SkillID for chain skill if it available for use
	 */
	FName GetSkillIDFromSkillSlot(const int32 SkillSlotIndex);

	void OnSkillUsed(const int32 SkillSlotIndex, FName SkillID, const FSkillTableRow* Skill);

private:

	UPROPERTY(Transient)
	USkillBarWidget* SkillBarWidget;

	UPROPERTY(EditAnywhere, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USkillBarWidget> SkillBarWidgetClass;

	// UPROPERTY(Transient)
	// TMap<int32, >

};

FORCEINLINE USkillBarWidget* USkillBarComponent::GetSkillBarWidget() const
{
	return SkillBarWidget;
}
