// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "CombatLibrary.h"
#include "CombatInterface.generated.h"

class UActiveSkillBase;

/** An interface that must be implemented by all in-game actors that can engage in combat */
UINTERFACE(BlueprintType)
class EOD_API UCombatInterface : public UInterface
{
	GENERATED_BODY()

public:

    UCombatInterface(const FObjectInitializer& ObjectInitializer);

};

/** An interface that must be implemented by all in-game actors that can engage in combat */
class EOD_API ICombatInterface
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  Combat Interface
	// --------------------------------------
	
	virtual TSharedPtr<FAttackInfo> GetAttackInfoPtr() const;

	virtual void SetAttackInfoFromActiveSkill(UActiveSkillBase* ActiveSkill);

	virtual void ResetAttackInfo();

	virtual bool IsEnemyOf(ICombatInterface* TargetCI) const;

	virtual TSharedPtr<FAttackResponse> ReceiveAttack(
		AActor* HitInstigator,
		ICombatInterface* InstigatorCI,
		const TSharedPtr<FAttackInfo>& AttackInfoPtr,
		const FHitResult& DirectHitResult,
		const bool bLineHitResultFound,
		const FHitResult& LineHitResult);

	virtual float GetActualDamage(
		AActor* HitInstigator,
		ICombatInterface* InstigatorCI,
		const TSharedPtr<FAttackInfo>& AttackInfoPtr,
		const bool bCritHit,
		const bool bAttackBlocked);

};