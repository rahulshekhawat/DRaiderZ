// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "EODLevelScriptActor.generated.h"

class UUserWidget;
class USoundBase;
class UAudioComponent;
class AEODCharacterBase;
class AMusicTriggerBox;

/**
 * 
 */
UCLASS()
class EOD_API AEODLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AEODLevelScriptActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// --------------------------------------
	//  Combat
	// --------------------------------------

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void OnCombatStarted(AEODCharacterBase* PlayerCharacter);
	virtual void OnCombatStarted_Implementation(AEODCharacterBase* PlayerCharacter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void OnCombatEnded(AEODCharacterBase* PlayerCharacter);
	virtual void OnCombatEnded_Implementation(AEODCharacterBase* PlayerCharacter);
	
	// --------------------------------------
	//  Components
	// --------------------------------------

	static const FName BGMAudioComponentName;

protected:

	/** Audio component for playing sound effects on getting hit */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	UAudioComponent* BGMAudioComponent;

	// --------------------------------------
	//  Pseudo Constants
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TArray<USoundBase*> CombatMusicCollection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TArray<USoundBase*> NonCombatMusicCollection;

	/** Time before which the level BGM starts playing after level is loaded */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	float InitialBGMTriggerDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	float QueuedBGMTriggerDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	float BGMFadeInDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	float BGMFadeOutDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> LevelTitleWidgetClass;

public:

	// --------------------------------------
	//  Events
	// --------------------------------------

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void FadeInViewport();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void FadeOutViewport();

protected:

	void BindCombatDelegates();

	void DisplayLevelTitle();

public:

	// --------------------------------------
	//  Sound
	// --------------------------------------
	
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void SwitchToBGM(USoundBase* NewBGM);

protected:

	void SetupBindingsForMusicTriggers();

	void InitiateBackgroundMusic();

	UFUNCTION()
	void PlayMusic(USoundBase* Music);

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void StopMusic(USoundBase* Music);

	UFUNCTION()
	void StopCurrentMusic();

	UFUNCTION()
	void PlayCombatMusic();

	UFUNCTION()
	void PlayNonCombatMusic();

	void QueueNextNonCombatMusic();

	FTimerHandle MusicTimerHandle;

	inline USoundBase* GetRandomSound(const TArray<USoundBase*> Sounds) const;
	
	/** Event called when an actor begins overlap with a music trigger box */
	UFUNCTION()
	void OnMusicTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	/** Event called when an actor ends overlap with a music trigger box */
	UFUNCTION()
	void OnMusicTriggerEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UPROPERTY(Transient)
	AMusicTriggerBox* ActiveMusicTriggerBox;

	UPROPERTY(Transient)
	USoundBase* ActiveTriggerMusic;

};

inline USoundBase* AEODLevelScriptActor::GetRandomSound(const TArray<USoundBase*> Sounds) const
{
	USoundBase* Sound = nullptr;
	int32 SoundsNum = Sounds.Num();
	if (SoundsNum == 1)
	{
		Sound = Sounds[0];
	}
	else if (SoundsNum > 1)
	{
		int32 RandSoundIndex = FMath::RandRange(0, SoundsNum - 1);
		Sound = Sounds[RandSoundIndex];
	}
	return Sound;
}
