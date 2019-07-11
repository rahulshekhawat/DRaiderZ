// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AssetProcessor.generated.h"

class UStaticMesh;


/**
 * 
 */
UCLASS()
class EDITORTOOLS_API UAssetProcessor : public UObject
{
	GENERATED_BODY()
	
public:

	UAssetProcessor(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = AssetProcessor)
	static bool GenerateUniqueUVForStaticMesh(UStaticMesh* StaticMesh);

};
