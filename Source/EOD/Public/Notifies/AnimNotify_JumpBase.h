// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_JumpBase.generated.h"

class USoundBase;
class USoundAttenuation;

/**
 * 
 */
UCLASS(Abstract, Blueprintable, hidecategories = Object, collapsecategories)
class EOD_API UAnimNotify_JumpBase : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	/** Determines if the character is starting the jump or ending the jump */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = JumpNotify)
	bool bJumpStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* DefaultJumpStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* DefaultJumpEndSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* StoneJumpStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* StoneJumpEndSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* ConcreteJumpStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* ConcreteJumpEndSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* DirtJumpStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* DirtJumpEndSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* MetalJumpStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* MetalJumpEndSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* FoliageJumpStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* FoliageJumpEndSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* SandJumpStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* SandJumpEndSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* SnowJumpStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* SnowJumpEndSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* WaterJumpStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* WaterJumpEndSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* WoodJumpStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* WoodJumpEndSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundAttenuation* SoundAttenuation;

	UFUNCTION(BlueprintCallable, Category = FootNotify)
	void GetJumpHitResults(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, FHitResult& OutHitResults);

	UFUNCTION(BlueprintCallable, Category = FootNotify)
	void PlayJumpSound(const FHitResult& HitResult);


};
