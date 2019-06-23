// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NotifyParser.generated.h"

class USkeletalMesh;

/**
 * 
 */
UCLASS()
class EDITORTOOLS_API UNotifyParser : public UObject
{
	GENERATED_BODY()

public:

	UNotifyParser(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = NotifyParser)
	static void DeleteAllNotifies(USkeletalMesh* SkeletalMesh);

};
