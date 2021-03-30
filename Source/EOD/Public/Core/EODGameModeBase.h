// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EODGameModeBase.generated.h"

class AEODCharacterBase;
class AStatusEffectsManager;

/** A struct containing respawn info */
USTRUCT(BlueprintType)
struct EOD_API FRespawnInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Transient)
	FVector RespawnLocation;
	
	UPROPERTY(Transient)
	TSubclassOf<AEODCharacterBase> RespawnClass;

	FRespawnInfo() :
		RespawnLocation(FVector::ZeroVector),
		RespawnClass(NULL)
	{
	}

	FRespawnInfo(const FVector& Location, TSubclassOf<AEODCharacterBase> CharClass) :
		RespawnLocation(Location),
		RespawnClass(CharClass)
	{
	}
};

/**
 * 
 */
UCLASS()
class EOD_API AEODGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AEODGameModeBase(const FObjectInitializer& ObjectInitializer);

	/** Spawns actors of manager classes */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Updates game state every frame */
	virtual void Tick(float DeltaTime) override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	// --------------------------------------
	//  Respawn mechanics
	// --------------------------------------

	UFUNCTION(BlueprintCallable, Category = Respawn)
	void SetOffCharacterRespawn(const FVector& RespawnLocation, const TSubclassOf<AEODCharacterBase>& CharacterClass);

	UFUNCTION()
	void RespawnCharacter(FRespawnInfo RespawnInfo);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Respawn)
	float RespawnDelay;

	// --------------------------------------
	//  Manager Classes
	// --------------------------------------


	/** Blueprint class used for spawning female characters */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<AEODCharacterBase> FemalePawnClass;

	/** Blueprint class used for spawning male characters */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<AEODCharacterBase> MalePawnClass;
	
};
