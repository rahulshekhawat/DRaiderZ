// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/GameplaySkillsComponent.h"
#include "AISkillsComponent.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UAISkillsComponent : public UGameplaySkillsComponent
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UAISkillsComponent(const FObjectInitializer& ObjectInitializer);

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// --------------------------------------
	//  Gameplay
	// --------------------------------------

	virtual void InitializeSkills(AEODCharacterBase* CompOwner = nullptr) override;




};
