// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Skills/InstantMeleeSkill.h"
#include "InstantMeleeSkill_NoSmoothRot.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UInstantMeleeSkill_NoSmoothRot : public UInstantMeleeSkill
{
	GENERATED_BODY()
	
public:

	UInstantMeleeSkill_NoSmoothRot(const FObjectInitializer& ObjectInitializer);

	virtual void ApplyRotation() override;


};
