// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EODGameInstance.generated.h"

class UMetaSaveGame;

/**
 * 
 */
UCLASS()
class EOD_API UEODGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UEODGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

public:
	static const int32 UserIndex;

	static const FString DefaultMetaSaveSlotName;

	FORCEINLINE UMetaSaveGame* GetMetaSaveGameObject() const { return MetaSaveGameObject; }

	UFUNCTION(BlueprintCallable, Category = "Save/Load System", meta = (DisplayName = "Get Meta Save Game Object"))
	UMetaSaveGame* BP_GetMetaSaveGameObject() const { return GetMetaSaveGameObject(); }

private:
	UPROPERTY()
	UMetaSaveGame* MetaSaveGameObject;


};
