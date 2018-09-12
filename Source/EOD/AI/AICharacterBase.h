// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/EODCharacterBase.h"
#include "AICharacterBase.generated.h"

class UEODWidgetComponent;

/**
 * AICharacterBase is the base class for AI controlled characters
 */
UCLASS()
class EOD_API AAICharacterBase : public AEODCharacterBase
{
	GENERATED_BODY()
	
public:

	AAICharacterBase(const FObjectInitializer& ObjectInitializer);

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Initialize TArray<FSkill*> */
	virtual void PostInitializeComponents() override;

	/** Called once this actor has been deleted */
	virtual void Destroyed() override;

	FORCEINLINE UEODWidgetComponent* GetAggroWidgetComp() const { return AggroWidgetComp; }

	FORCEINLINE UEODWidgetComponent* GetHealthWidgetComp() const { return HealthWidgetComp; }

	virtual void SetInCombat(bool bValue) override;

	/** [server] Handle melee collision */
	virtual void OnMeleeCollision(UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit) override;

	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

private:

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UEODWidgetComponent* AggroWidgetComp;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UEODWidgetComponent* HealthWidgetComp;

	UPROPERTY(Category = AIMovement, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxWalkSpeedInCombat;

	UPROPERTY(Category = AIMovement, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxWalkSpeedOutsideCombat;

	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	FString InGameName;

	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	FString InGameDescription;

	UPROPERTY(EditDefaultsOnly, Category = Animations)
	UAnimMontage* AnimationMontage_HitEffects;

	UPROPERTY(EditDefaultsOnly, Category = Animations)
	UAnimMontage* AnimationMontage_Flinch;

	UPROPERTY(EditDefaultsOnly, Category = Animations)
	UAnimMontage* AnimationMontage_Skills;

	UPROPERTY(EditDefaultsOnly, Category = Skills)
	UDataTable* DataTable_Skills;

	/** Changes maximum walk speed of character based on whether character is engaged in combat or not */
	void UpdateMaxWalkSpeed();

};
