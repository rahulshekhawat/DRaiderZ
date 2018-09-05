// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/EODCharacterBase.h"
#include "AICharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API AAICharacterBase : public AEODCharacterBase
{
	GENERATED_BODY()
	
public:

	AAICharacterBase(const FObjectInitializer& ObjectInitializer);

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AIMovement)
	float MaxWalkSpeedInCombat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AIMovement)
	float MaxWalkSpeedOutsideCombat;

	virtual void SetInCombat(bool bValue) override;

	bool HasGreetingAnimation() const { bool bResult = GetGreetingMontageSectionName() == NAME_None ? false : true; return bResult; }

	FName GetGreetingMontageSectionName() const { return GreetingMontageSectionName; }

	/** Initialize TArray<FSkill*> */
	virtual void PostInitializeComponents() override;

	/** [server] Handle melee collision */
	virtual void OnMeleeCollision(UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit) override;

	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

private:

	/** Changes maximum walk speed of character based on whether character is engaged in combat or not */
	void UpdateMaxWalkSpeed();

	UPROPERTY(EditDefaultsOnly, Category = Animations)
	FName GreetingMontageSectionName;

	UPROPERTY(EditDefaultsOnly, Category = Animations)
	UAnimMontage* AnimationMontage_HitEffects;

	UPROPERTY(EditDefaultsOnly, Category = Animations)
	UAnimMontage* AnimationMontage_Flinch;

	UPROPERTY(EditDefaultsOnly, Category = Animations)
	UAnimMontage* AnimationMontage_Skills;

	UPROPERTY(EditDefaultsOnly, Category = Skills)
	UDataTable* DataTable_Skills;

	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	FString InGameName;

	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	FString InGameDescription;

};
