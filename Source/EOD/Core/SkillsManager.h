// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// #include ""
#include "GameFramework/Info.h"
#include "SkillsManager.generated.h"

class AEODCharacterBase;

/**
 * 
 */
UCLASS()
class EOD_API ASkillsManager : public AInfo
{
	GENERATED_BODY()
	
public:
	ASkillsManager(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	// Map of character class to IDToSkillMap
	// TMap<TSubclassOf<AEODCharacterBase>, TMap<FName, FSkill*>*>
	
};
