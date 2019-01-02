// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EODGameInstance.generated.h"

class UMetaSaveGame;
class UPlayerSaveGame;

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
	static const int32 PlayerIndex;

	static const FString MetaSaveSlotName;

	FORCEINLINE UMetaSaveGame* GetMetaSaveGameObject() const { return MetaSaveGame; }

	UFUNCTION(BlueprintCallable, Category = "Save/Load System", meta = (DisplayName = "Get Meta Save Game Object"))
	UMetaSaveGame* BP_GetMetaSaveGameObject() const { return GetMetaSaveGameObject(); }

	FORCEINLINE UPlayerSaveGame* GetCurrentPlayerSaveGameObject() const { return CurrentProfileSaveGame; }

	UFUNCTION(BlueprintCallable, Category = "Save/Load System", meta = (DisplayName = "Get Current Player Save Game"))
	UPlayerSaveGame* BP_GetCurrentPlayerSaveGame() const { return GetCurrentPlayerSaveGameObject(); }

private:
	UPROPERTY()
	UMetaSaveGame* MetaSaveGame;

	UPROPERTY()
	UPlayerSaveGame* CurrentProfileSaveGame;


};
