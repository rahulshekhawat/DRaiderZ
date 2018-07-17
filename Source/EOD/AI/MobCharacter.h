// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/BaseCharacter.h"
#include "MobCharacter.generated.h"

/**
 * Base class for in-game AI controlled mob character
 */
UCLASS()
class EOD_API AMobCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	AMobCharacter(const FObjectInitializer& ObjectInitializer);
	
protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AIMovement)
	float MaxWalkSpeedInCombat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AIMovement)
	float MaxWalkSpeedOutsideCombat;
	
	virtual void SetInCombat(bool bValue) override;

	bool HasGreetingAnimation() const { bool bResult = GetGreetingMontageSectionName() == NAME_None ? false : true; return bResult; }

	FName GetGreetingMontageSectionName() const { return GreetingMontageSectionName; }
	
protected:


private:

	/** Changes maximum walk speed of character based on whether character is engaged in combat or not */
	void UpdateMaxWalkSpeed();
	
	UPROPERTY(EditDefaultsOnly, Category = Animations)
	FName GreetingMontageSectionName;
	


};
