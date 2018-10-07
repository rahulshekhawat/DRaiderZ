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

	/** Get aggro widget component */
	FORCEINLINE UEODWidgetComponent* GetAggroWidgetComp() const { return AggroWidgetComp; }

	/** Get health widget component */
	FORCEINLINE UEODWidgetComponent* GetHealthWidgetComp() const { return HealthWidgetComp; }

	/** Get aggro widget component */
	UFUNCTION(BlueprintPure, Category = WidgetComponent, meta = (DisplayName = "Get Aggro Widget Component"))
	UEODWidgetComponent* BP_GetAggroWidgetComp() const;

	/** Get health widget component */
	UFUNCTION(BlueprintPure, Category = WidgetComponent, meta = (DisplayName = "Get Health Widget Component"))
	UEODWidgetComponent* BP_GetHealthWidgetComp() const;

	/** Set whether character is in combat or not */
	virtual void SetInCombat(bool bValue) override;

	/** [server] Handle melee collision */
	virtual void OnMeleeCollision(UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit) override;

	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

	/** [server + client] Interrupt this character's current action */
	virtual void Interrupt(const EHitDirection InterruptDirection) override;

	/** [server + client] Flinch this character. This is nothing more than a visual feedback to getting attacked */
	virtual void Flinch(const EHitDirection FlinchDirection) override;

	/** [server + client] Applies stun to this character */
	virtual void Stun(const float Duration) override;

	/** [client] Removes 'stun' crowd control effect from this character */
	virtual void EndStun() override;

	/** [server + client] Freeze this character */
	virtual void Freeze(const float Duration) override;

	/** [client] Removes 'freeze' crowd control effect from this character */
	virtual void EndFreeze() override;

	/** [server + client] Knockdown this character */
	virtual void Knockdown(const float Duration) override;

	/** [client] Removes 'knock-down' crowd control effect from this character */
	virtual void EndKnockdown() override;

	/** [server + client] Knockback this character */
	virtual void Knockback(const float Duration, const FVector& Impulse) override;

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
