// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplaySkillBase.generated.h"

class AEODCharacterBase;
class AController;

/**
 * 
 */
UCLASS()
class EOD_API UGameplaySkillBase : public UObject
{
	GENERATED_BODY()
	
public:
	UGameplaySkillBase(const FObjectInitializer& ObjectInitializer);

	void Initialize(AEODCharacterBase* Instigator, AController* Owner);




};
