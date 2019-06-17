// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AIStatsComponent.h"
#include "AICharacterBase.h"
#include "FloatingHealthBarWidget.h"
#include "EODWidgetComponent.h"
#include "EODGameInstance.h"
#include "EODAIControllerBase.h"

#include "UnrealNetwork.h"
#include "Engine/DataTable.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetSystemLibrary.h"

UAIStatsComponent::UAIStatsComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UAIStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadAIStats();

}

void UAIStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UAIStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAIStatsComponent::LoadAIStats()
{
	UWorld* World = GetWorld();
	UEODGameInstance* EODGI = World ? Cast<UEODGameInstance>(World->GetGameInstance()) : nullptr;
	UDataTable* AISDataTable = EODGI ? EODGI->CharacterStatsDataTable : nullptr;

	FCharacterStatsTableRow* TableRow = nullptr;
	if (AISDataTable)
	{
		TableRow = AISDataTable->FindRow<FCharacterStatsTableRow>(StatsRowName, FString("UAIStatsComponent::LoadAIStats()"));
	}

	if (!TableRow)
	{
		//~ @todo maybe load some default stat values?
		return;
	}

	AEODAIControllerBase* AIC = Cast<AEODAIControllerBase>(GetOwner());
	AEODCharacterBase* AIChar = AIC ? Cast<AEODCharacterBase>(AIC->GetPawn()) : nullptr;
	if (AIChar)
	{
		AIChar->SetCharacterLevel(TableRow->Level);
	}

	Health.SetMaxValue(TableRow->Health);
	Health.RefillCurrentValue();

	Mana.SetMaxValue(TableRow->Mana);
	Mana.RefillCurrentValue();

	Stamina.SetMaxValue(TableRow->Stamina);
	Stamina.RefillCurrentValue();

	HealthRegenRate.SetValue(TableRow->HealthRegenRate);
	HealthRegenRate.ForceBroadcastDelegate();

	ManaRegenRate.SetValue(TableRow->ManaRegenRate);
	ManaRegenRate.ForceBroadcastDelegate();

	StaminaRegenRate.SetValue(TableRow->StaminaRegenRate);
	StaminaRegenRate.ForceBroadcastDelegate();

	PhysicalAttack.SetValue(TableRow->PhysicalAttack);
	PhysicalAttack.ForceBroadcastDelegate();

	PhysicalCritRate.SetValue(TableRow->PhysicalCritRate);
	PhysicalCritRate.ForceBroadcastDelegate();

	PhysicalResistance.SetValue(TableRow->PhysicalResistance);
	PhysicalResistance.ForceBroadcastDelegate();

	MagickalAttack.SetValue(TableRow->MagickalAttack);
	MagickalAttack.ForceBroadcastDelegate();

	MagickalCritRate.SetValue(TableRow->MagickalCritRate);
	MagickalCritRate.ForceBroadcastDelegate();

	MagickalResistance.SetValue(TableRow->MagickalResistance);
	MagickalResistance.ForceBroadcastDelegate();

	BleedResistance.SetValue(TableRow->BleedResistance);
	BleedResistance.ForceBroadcastDelegate();

	PhysicalDamageReductionOnBlock.SetValue(TableRow->PhysicalDamageReductionOnBlock);
	PhysicalDamageReductionOnBlock.ForceBroadcastDelegate();

	MagickalDamageReductionOnBlock.SetValue(TableRow->MagickalDamageReductionOnBlock);
	MagickalDamageReductionOnBlock.ForceBroadcastDelegate();

	CCImmunities.SetValue(TableRow->CrowdControlImmunities);

}
