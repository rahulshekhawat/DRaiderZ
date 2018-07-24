// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Notifies/AnimNotify_RaidCollision.h"
#include "UObject/NoExportTypes.h"
#include "CollisionParser.generated.h"


USTRUCT(BlueprintType)
struct FRaidCollision
{
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Time;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FRaidCapsule> Capsules;


};

USTRUCT(BlueprintType)
struct FAnimationHitInfo
{
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString TALENT_HIT_id;
	
	/*
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString AnimationName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString WeaponAllowed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MeshPath;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FRaidCollision> RaidCollisions;
};


/**
 * 
 */
UCLASS()
class EDITORTOOLS_API UCollisionParser : public UObject
{
	GENERATED_BODY()
	
public:
	
	static const FName EditorDir_Models;

	static const FString CollisionImportSettingsFilePath;

	static const FString ProcessedAnimationsListFilePath;

	static const FString AnimationsNotFoundFilePath;

	static const FString AnimationNamesFilePath;

	UFUNCTION(BlueprintCallable, Category = EditorLibrary)
	static void ImportAndApplyCollisionInfo();
	// static void ImportAndApplyCollisionInfo(FName TalentHitInfoXmlPath, FName TalentXmlPath);
	
	UFUNCTION(BlueprintCallable, Category = EditorLibrary)
	static TArray<FString> GetAnimationNamesFromTalentID(FString TalentID);

	
	
	
};
