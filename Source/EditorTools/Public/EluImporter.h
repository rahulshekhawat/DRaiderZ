// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EluImporter.generated.h"

/**
 * 
 */
UCLASS()
class EDITORTOOLS_API UEluImporter : public UObject
{
	GENERATED_BODY()
	
public:

	UEluImporter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Elu Importer")
	static void ImportEluFile();

private:

	static bool ImportEluFile_Internal(const FString& EluFilePath);

};
