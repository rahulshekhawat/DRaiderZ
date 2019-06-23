// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NotifyParser.generated.h"

class USkeletalMesh;
class UAnimNotify;
class UAnimNotifyState;

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
	
	UFUNCTION(BlueprintCallable, Category = NotifyParser)
	static void DeleteAllNotifiesOfClass(USkeletalMesh* SkeletalMesh, TSubclassOf<UAnimNotify> NotifyClass);

	UFUNCTION(BlueprintCallable, Category = NotifyParser)
	static void DeleteAllNotifyStatesOfClass(USkeletalMesh* SkeletalMesh, TSubclassOf<UAnimNotifyState> NotifyClass);


};
