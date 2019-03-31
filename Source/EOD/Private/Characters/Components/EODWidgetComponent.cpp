// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODWidgetComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"

void UEODWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UUserWidget* WidgetObj = GetUserWidgetObject();
	if (WidgetObj && WidgetObj->IsVisible())
	{
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (CameraManager)
		{
			FVector CameraManagerLocation = CameraManager->GetCameraLocation();
			FVector WidgetLocation = this->GetComponentLocation();

			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraManagerLocation);
			this->SetWorldRotation(LookAtRotation);
		}
	}
}
