// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "HUDWidget.h"
#include "GameFramework/PlayerController.h"
#include "RaiderzPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API ARaiderzPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ARaiderzPlayerController();
	
	virtual void SetupInputComponent() override;

	virtual void SetPawn(APawn* InPawn) override;
	
	inline class UHUDWidget* GetHUDWidget() const { return HUDWidget; }
	
protected:

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Raiderz Widgets")
	class UHUDWidget* HUDWidget;
	
	/** Move controlled pawn forward/backward */
	void MovePawnForward(const float Value);

	/** Move controlled pawn left/right */
	void MovePawnRight(const float Value);

	UPROPERTY(Transient)
	class AEODCharacterBase* EODCharacter;
	
};
