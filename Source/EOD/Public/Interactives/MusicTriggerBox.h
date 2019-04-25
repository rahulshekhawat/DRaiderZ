// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MusicTriggerBox.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS()
class EOD_API AMusicTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AMusicTriggerBox(const FObjectInitializer& ObjectInitializer);

	// --------------------------------------
	//  Sound
	// --------------------------------------

	/** Returns a random music from MusicCollection */
	UFUNCTION(BlueprintCallable, Category = Sound, DisplayName = "Get Random Music")
	USoundBase* BP_GetRandomMusic() const;

	/** Returns a random music from MusicCollection */
	inline USoundBase* GetRandomMusic() const;

protected:

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> MusicCollection;

};

inline USoundBase* AMusicTriggerBox::GetRandomMusic() const
{
	USoundBase* Sound = nullptr;
	int32 SoundsNum = MusicCollection.Num();
	if (SoundsNum == 1)
	{
		Sound = MusicCollection[0];
	}
	else if (SoundsNum > 1)
	{
		int32 RandSoundIndex = FMath::RandRange(0, SoundsNum - 1);
		Sound = MusicCollection[RandSoundIndex];
	}
	return Sound;
}
