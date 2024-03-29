// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterLibrary.h"
#include "GameplayEffect.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "Camera/CameraShake.h"
#include "Engine/StreamableManager.h"
#include "Engine/GameInstance.h"
#include "EODGameInstance.generated.h"

class UWorld;
class UDamageNumberWidget;
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable)
	UDataTable* PlayerStatsDataTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable)
	UDataTable* CharacterStatsDataTable;

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

	void LoadSaveGame();

	void OnPreLoadMap(const FString& MapName);
	void OnPostLoadMap(UWorld* WorldObj);

public:

	// --------------------------------------
	//  Utility
	// --------------------------------------

	/**
	 * Displays damage numbers on player screen
	 * @param DamageValue	The amount by which the character was damaged
	 * @param bCritHit		True if the damage was critical
	 * @param DamagedActor	The actor that was damaged
	 * @param HitLocation	The hit position (in world space)
	 */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void DisplayDamageNumbers(
		const float DamageValue,
		const bool bCritHit,
		const AActor* DamagedActor,
		const AActor* DamageInstigator,
		const FVector& HitLocation);

	UPROPERTY(EditAnywhere, Category = "Utility")
	TSubclassOf<UDamageNumberWidget> DamageWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Utility|Colors", BlueprintReadOnly)
	FLinearColor BuffTextColor;

	UPROPERTY(EditDefaultsOnly, Category = "Utility|Colors", BlueprintReadOnly)
	FLinearColor DebuffTextColor;

	UPROPERTY(EditDefaultsOnly, Category = "Utility|Colors", BlueprintReadOnly)
	FLinearColor DodgeTextColor;

	UPROPERTY(EditDefaultsOnly, Category = "Utility|Colors", BlueprintReadOnly)
	FLinearColor PlayerDamagedTextColor;

	UPROPERTY(EditDefaultsOnly, Category = "Utility|Colors", BlueprintReadOnly, DisplayName = "NPC Damaged Text Color")
	FLinearColor NPCNormalDamagedTextColor;

	UPROPERTY(EditDefaultsOnly, Category = "Utility|Colors", BlueprintReadOnly, DisplayName = "NPC Critically Damaged Text Color")
	FLinearColor NPCCritDamagedTextColor;

	// ----

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void PlayerCameraShakeOnHit(const AActor* DamagedActor, const AActor* DamageInstigator, ECameraShakeType CamShakeType, const FVector& EpiCenter);
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void PlayCameraShake(ECameraShakeType CameraShakeType, const FVector& EpiCenter);
	
	UPROPERTY(EditDefaultsOnly, Category = "Utility|Camera", BlueprintReadOnly)
	TSubclassOf<UMatineeCameraShake> LowCamShakeClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Utility|Camera", BlueprintReadOnly)
	TSubclassOf<UMatineeCameraShake> MediumCamShakeClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Utility|Camera", BlueprintReadOnly)
	TSubclassOf<UMatineeCameraShake> HighCamShakeClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Utility|Camera", BlueprintReadOnly)
	float CamShakeInnerRadius;
	
	UPROPERTY(EditDefaultsOnly, Category = "Utility|Camera", BlueprintReadOnly)
	float CamShakeOuterRadius;

	// ----
	
	/**
	 * Displays damage numbers on player screen
	 * @param DamageValue	The amount by which the character was damaged
	 * @param bCritHit		True if the damage was critical
	 * @param DamagedActor	The actor that was damaged
	 * @param HitLocation	The hit position (in world space)
	 */
	UFUNCTION(BlueprintCallable, Category = "Utility")
	void DisplayGameplayText(
		const FString& GameplayText,
		const AActor* DamagedActor,
		const AActor* DamageInstigator,
		const FVector& HitLocation);

	UFUNCTION(BlueprintPure, Category = "Utility")
	FString GetMapDisplayName(FName EditorMapName) const;

	UFUNCTION(BlueprintPure, Category = "Utility")
	FString GetPlayerName() const;

protected:

	/** TMap<EditorMapName, MapNameForPlayer> */
	UPROPERTY(EditAnywhere, Category = "Maps")
	TMap<FName, FString> MapNames;


public:

	// --------------------------------------
	//  Online
	// --------------------------------------
	
	UFUNCTION(BlueprintCallable, Category = "Online")
	virtual void HostOnlineGame(FString ServerName);

	UFUNCTION(BlueprintCallable, Category = "Online")
	virtual void JoinOnlineGame(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Online")
	virtual void EndOnlineGame();

	UFUNCTION(BlueprintCallable, Category = "Online")
	virtual void OpenSessionListMenu();

protected:

	FString DesiredServerName;

	virtual void OnCreateSessionComplete(FName SessionName, bool Success);

	virtual void OnDestroySessionComplete(FName SessionName, bool Success);

	virtual void OnFindSessionsComplete(bool Success);

	virtual void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// virtual void CreateSession();


private:

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

public:

	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = "Damage Classes")
	TSubclassOf<UGameplayEffect> PhysicalDamageClass;
	
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = "Damage Classes")
	TSubclassOf<UGameplayEffect> MagicDamageClass;
	

};
