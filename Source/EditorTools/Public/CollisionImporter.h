// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "XmlFile.h"
#include "UObject/NoExportTypes.h"
#include "CollisionImporter.generated.h"

class USkeletalMesh;

/**
 * 
 */
UCLASS()
class EDITORTOOLS_API UCollisionImporter : public UObject
{
	GENERATED_BODY()
	
public:

	UCollisionImporter(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = EditorLibrary)
	static void ImportCollisionForSkeletalMesh(USkeletalMesh* Mesh);

	static const FString NPCFilePath;
	static const FString TalentFilePath;

	static bool GetNPCID(FXmlNode* NPCRootNode, const FString& MeshName, FString& OutNPCID);

};
