// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

	FORCEINLINE void InitializeComponentWidget();

	/** Returns true if player can use skill placed at given skill slot index */
	bool CanUseSkill(const int32 SkillSlotIndex);

private:

	UPROPERTY(Transient)
	USkillBarWidget* SkillBarWidget;

	UPROPERTY(EditAnywhere, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USkillBarWidget> SkillBarWidgetClass;

	
};
