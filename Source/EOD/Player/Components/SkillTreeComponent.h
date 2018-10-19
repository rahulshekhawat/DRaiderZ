// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillTreeComponent.generated.h"

class USkillTreeWidget;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EOD_API USkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillTreeComponent(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Returns skill bar widget */
	FORCEINLINE USkillTreeWidget* GetSkillTreeWidget() const;

	/** Returns skill bar widget */
	UFUNCTION(BlueprintPure, Category = UI, meta = (DisplayName = "Get Skill Tree Widget"))
	USkillTreeWidget* BP_GetSkillTreeWidget() const;

	FORCEINLINE void InitializeComponentWidget();



private:

	UPROPERTY(Transient)
	USkillTreeWidget* SkillTreeWidget;

	UPROPERTY(EditAnywhere, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USkillTreeWidget> SkillTreeWidgetClass;
	
};
