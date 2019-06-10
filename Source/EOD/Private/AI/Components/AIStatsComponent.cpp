// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "AIStatsComponent.h"
#include "AICharacterBase.h"
#include "FloatingHealthBarWidget.h"
#include "EODWidgetComponent.h"

#include "UnrealNetwork.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetSystemLibrary.h"

UAIStatsComponent::UAIStatsComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	OwningAIChar = Cast<AAICharacterBase>(GetOwner());
}

void UAIStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAIStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UAIStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
