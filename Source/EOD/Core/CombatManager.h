// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/CharacterLibrary.h"
#include "GameFramework/Info.h"
#include "CombatManager.generated.h"

class AActor;
class AEODCharacterBase;
class APlayerCharacter;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class EOD_API ACombatManager : public AInfo
{
	GENERATED_BODY()
	
public:

	ACombatManager(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void OnMeleeHit(AActor* Instigator, const bool bHit, const TArray<FHitResult>& HitResults);

	void ProcessActorMeleeAttack(AActor* Instigator, const bool bHit, const TArray<FHitResult>& HitResults);

	void ProcessNPCMeleeAttack(AEODCharacterBase* Instigator, const bool bHit, const TArray<FHitResult>& HitResults);

	void ProcessPlayerMeleeAttack(APlayerCharacter* Instigator, const bool bHit, const TArray<FHitResult>& HitResults);

	void PlayerToNPCMeleeAttack(APlayerCharacter* Instigator,
								AEODCharacterBase* HitCharacter,
								const FPlayerSkillTableRow* HitSkill,
								const FHitResult& HitResult);

	void PlayerToActorMeleeAttack(APlayerCharacter* Instigator,
								  AActor* HitActor,
								  const FPlayerSkillTableRow* HitSkill,
								  const FHitResult& HitResult);

	//~ @todo OnRangedHit

	UFUNCTION(BlueprintImplementableEvent, Category = WidgetText)
	void DisplayDamageText(const FString& Message);

	UFUNCTION(BlueprintImplementableEvent, Category = WidgetText)
	void DisplayStatusEffectText(const FString& Message);

protected:

	UPROPERTY(EditDefaultsOnly, Category = Colors, BlueprintReadOnly)
	FLinearColor BuffTextColor;

	UPROPERTY(EditDefaultsOnly, Category = Colors, BlueprintReadOnly)
	FLinearColor DebuffTextColor;

	UPROPERTY(EditDefaultsOnly, Category = Colors, BlueprintReadOnly)
	FLinearColor DodgeTextColor;

	UPROPERTY(EditDefaultsOnly, Category = Colors, BlueprintReadOnly)
	FLinearColor PlayerDamagedTextColor;

	UPROPERTY(EditDefaultsOnly, Category = Colors, BlueprintReadOnly, meta = (DisplayName = "NPC Damaged Text Color"))
	FLinearColor NPCNormalDamagedTextColor;

	UPROPERTY(EditDefaultsOnly, Category = Colors, BlueprintReadOnly, meta = (DisplayName = "NPC Critically Damaged Text Color"))
	FLinearColor NPCCritDamagedTextColor;

};
