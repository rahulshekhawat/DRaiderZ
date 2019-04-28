// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UILibrary.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomWidgetEventMCDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSliderChangeMCDelegate, float, Value);

/**
 * 
 */
UCLASS()
class EOD_API UUILibrary : public UObject
{
	GENERATED_BODY()

public:

	UUILibrary(const FObjectInitializer& ObjectInitializer);
	
};
