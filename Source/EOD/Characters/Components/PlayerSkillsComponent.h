// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/GameplaySkillsComponent.h"
#include "PlayerSkillsComponent.generated.h"

class UGameplaySkillBase;

/**
 * 
 */
UCLASS()
class EOD_API UPlayerSkillsComponent : public UGameplaySkillsComponent
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UPlayerSkillsComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Sets up property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// --------------------------------------
	//  Input Handling
	// --------------------------------------

	void OnPressingSkillKey(const int32 SkillKeyIndex);

	void OnReleasingSkillKey(const int32 SkillKeyIndex);

	// --------------------------------------
	//  Skill System
	// --------------------------------------

	virtual void InitializeSkills(AEODCharacterBase* CompOwner = nullptr) override;

	void OnSkillAddedToSkillBar(uint8 SkillBarIndex, FName SkillGroup);

protected:

	/** Skill index to skil map. Skill index will be used during replication */
	UPROPERTY(Transient)
	TMap<uint8, UGameplaySkillBase*> SkillsMap;

	/** A map of all the skills placed on skill bar to their Skill Index */
	UPROPERTY(Transient)
	TMap<uint8, uint8> SkillBarMap;



};
