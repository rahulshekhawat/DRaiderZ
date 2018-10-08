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

	/** Initialize TArray<FSkill*> */
	virtual void PostInitializeComponents() override;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

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

	/** Set whether character is in combat or not */
	virtual void SetInCombat(bool bValue) override;

	/** [server] Handle melee collision */
	virtual void OnMeleeCollision(UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit) override;

	virtual void OnMontageBlendingOut(UAnimMontage* AnimMontage, bool bInterrupted);

	virtual void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

private:

	/** Used to display floating aggro widget above AI character */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UEODWidgetComponent* AggroWidgetComp;

	/** Used to display floating health widget above AI character */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UEODWidgetComponent* HealthWidgetComp;

	/** In game name of this AI character. This may or may not differ for each instance of character */
	UPROPERTY(EditAnywhere, Category = BaseInfo, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString InGameName;

	/** In game description of this AI character. This may or may not differ for each instance of character */
	UPROPERTY(EditAnywhere, Category = BaseInfo, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString InGameDescription;

	/** Maximum speed an AI character can walk with inside combat */
	UPROPERTY(EditDefaultsOnly, Category = AIMovement, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxWalkSpeedInCombat;

	/** Maximum speed an AI character can walk with outside combat */
	UPROPERTY(EditDefaultsOnly, Category = AIMovement, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxWalkSpeedOutsideCombat;

	/** Animation montage containing animations for hit effects */
	UPROPERTY(EditDefaultsOnly, Category = Animations, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AnimMontage_HitEffects;

	/** Animation montage containing animations for character flinch */
	UPROPERTY(EditDefaultsOnly, Category = Animations, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AnimMontage_Flinch;

	/** Data table for character skills */
	UPROPERTY(EditDefaultsOnly, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* DataTable_Skills;

	/** Changes maximum walk speed of character based on whether character is engaged in combat or not */
	void UpdateMaxWalkSpeed();

	UPROPERTY(Transient, Category = Skills, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bHasFlinchAnimations;


};
