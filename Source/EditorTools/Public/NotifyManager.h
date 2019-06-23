// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NotifyManager.generated.h"

class USkeletalMesh;
class UAnimNotify;
class UAnimNotifyState;

/**
 * 
 */
UCLASS()
class EDITORTOOLS_API UNotifyManager : public UObject
{
	GENERATED_BODY()
	
public:

	UNotifyManager(const FObjectInitializer& ObjectInitializer);

	/**
	 * Removes all anim notifies from every single animation that exists in editor
	 * @param NotifyClass The anim notify class whose objects are to be removed
	 * @warning Cannot be undone, use with caution
	*/
	UFUNCTION(BlueprintCallable, Category = AnimNotifyLibrary)
	static void CleanUpNotifiesFromAllAnimations(TSubclassOf<class UAnimNotify> NotifyClass);
	
	/**
	 * Removes all anim notify states from every single animation that exists in editor
	 * @param NotifyStateClass The anim notify state class whose objects are to be removes
	 * @warning Cannot be undone, use with caution
	*/
	UFUNCTION(BlueprintCallable, Category = AnimNotifyLibrary)
	static void CleanUpNotifyStatesFromAllAnimations(TSubclassOf<class UAnimNotifyState> NotifyStateClass);
	
	UFUNCTION(BlueprintCallable, Category = NotifyParser)
	static void DeleteAllNotifies(USkeletalMesh* SkeletalMesh);
	
	UFUNCTION(BlueprintCallable, Category = NotifyParser)
	static void DeleteAllNotifiesOfClass(USkeletalMesh* SkeletalMesh, TSubclassOf<UAnimNotify> NotifyClass);

	UFUNCTION(BlueprintCallable, Category = NotifyParser)
	static void DeleteAllNotifyStatesOfClass(USkeletalMesh* SkeletalMesh, TSubclassOf<UAnimNotifyState> NotifyClass);
	
	
};
