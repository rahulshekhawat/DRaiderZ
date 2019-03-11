// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Statics/CharacterLibrary.h"

#include "Engine/StreamableManager.h"
#include "Engine/GameInstance.h"
#include "EODGameInstance.generated.h"

class UWorld;
class UUserWidget;
class UMetaSaveGame;
class UPlayerSaveGame;
class APlayerSkillTreeManager;

/**
 * 
 */
UCLASS()
class EOD_API UEODGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	UEODGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	// --------------------------------------
	//	Global Variables
	// --------------------------------------

	/** The title of the game that will be displayed to player on title screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EOD Global Variables")
	FText GameTitle;

	/** Name of map where the player starts a new campaign */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EOD Global Variables")
	FName StartupMapName;

	// --------------------------------------
	//	Campaign
	// --------------------------------------

	/** Start a new campaign with currently selected character */
	UFUNCTION(BlueprintCallable, Category = "Campaign")
	void StartNewCampaign();

	/** Continue a previously saved campaign */
	UFUNCTION(BlueprintCallable, Category = "Campaign")
	void ContinuePreviousCampaign();
 
	/** Returns true if player has a previously saved campaign and can continue it */
	UFUNCTION(BlueprintCallable, Category = "Campaign")
	bool CanContinuePreviousCampaign() const;

	// --------------------------------------
	//	Player Save Game System
	// --------------------------------------

	static const int32 PlayerIndex;
	static const FString MetaSaveSlotName;

	/** A global instance of FStreamableManager to handle asset loading */
	FStreamableManager StreamableManager;

	/** Create and save a new save game profile */
	void CreateNewProfile(const FString& ProfileName);

	/** Load a save game profile and replace current save game profile with it */
	UPlayerSaveGame* LoadProfileAsCurrent(const FString& ProfileName);

	/** Get current save game object containing player data */
	FORCEINLINE UPlayerSaveGame* GetCurrentPlayerSaveGameObject() const { return CurrentProfileSaveGame; }

	/** Get name of currently loaded save game profile */
	FORCEINLINE FString GetCurrentPlayerSaveGameName() const { return CurrentProfileName; }

	/** Get current save game object containing player data */
	UFUNCTION(BlueprintCallable, Category = "Save/Load System", meta = (DisplayName = "Get Current Player Save Game"))
	UPlayerSaveGame* BP_GetCurrentPlayerSaveGame() const { return GetCurrentPlayerSaveGameObject(); }

protected:

	FORCEINLINE UMetaSaveGame* GetMetaSaveGameObject() const { return MetaSaveGame; }

private:

	/** Name of currently loaded save game profile */
	FString CurrentProfileName;

	/** Save game object containing player save profiles */
	UPROPERTY(Transient)
	UMetaSaveGame* MetaSaveGame;

	/** Player save game object containing player data */
	UPROPERTY(Transient)
	UPlayerSaveGame* CurrentProfileSaveGame;

protected:

	// --------------------------------------
	//	Loading Screen
	// --------------------------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass;

	UPROPERTY(Transient)
	UUserWidget* LoadingScreenWidget;

private:

	void OnPreLoadMap(const FString& MapName);

	void OnPostLoadMap(UWorld* WorldObj);

public:

	// --------------------------------------
	//	Player Skill Tree
	// --------------------------------------

	UPROPERTY(Transient)
	TMap<FString, FSkillState> SGToSSMap;

	TMap<FString, TSharedRef<FSkillState>> SkillGroupToSkillStateMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Tree")
	TSubclassOf<APlayerSkillTreeManager> PlayerSkillTreeManagerClass;

	UPROPERTY(Transient)
	APlayerSkillTreeManager* SkillTreeManager;

};
