// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class EOD_API USkillBase : public UDataAsset
{
	GENERATED_BODY()
	
public:

	USkillBase(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnUse();

	UFUNCTION(BlueprintImplementableEvent, Category = Skills)
	void OnUse();

	
	
	
};
