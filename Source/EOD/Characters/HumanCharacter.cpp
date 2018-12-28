// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/Characters/HumanCharacter.h"

#include "GameFramework/PlayerController.h"

AHumanCharacter::AHumanCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Hair				= CreateNewArmorComponent(FName("Hair"), ObjectInitializer);
	HatItem				= CreateNewArmorComponent(FName("Hat Item"), ObjectInitializer);
	FaceItem			= CreateNewArmorComponent(FName("Face Item"), ObjectInitializer);
	Chest				= CreateNewArmorComponent(FName("Chest"), ObjectInitializer);
	Hands				= CreateNewArmorComponent(FName("Hands"), ObjectInitializer);
	Legs				= CreateNewArmorComponent(FName("Legs"), ObjectInitializer);
	Feet				= CreateNewArmorComponent(FName("Feet"), ObjectInitializer);
}

void AHumanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Forward", IE_Pressed, this, &AHumanCharacter::OnPressedForward);
	PlayerInputComponent->BindAction("Forward", IE_Released, this, &AHumanCharacter::OnReleasedForward);
	PlayerInputComponent->BindAction("Backward", IE_Pressed, this, &AHumanCharacter::OnPressedBackward);
	PlayerInputComponent->BindAction("Backward", IE_Released, this, &AHumanCharacter::OnReleasedBackward);
}

void AHumanCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AHumanCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AHumanCharacter::PostInitProperties()
{
	Super::PostInitProperties();
}

void AHumanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHumanCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetMasterPoseComponentForMeshes();
}

void AHumanCharacter::Destroyed()
{
	Super::Destroyed();
}

void AHumanCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AHumanCharacter::UnPossessed()
{
	Super::UnPossessed();

}

USkeletalMeshComponent* AHumanCharacter::CreateNewArmorComponent(const FName Name, const FObjectInitializer& ObjectInitializer)
{
	USkeletalMeshComponent* Sk = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, Name);
	if (Sk)
	{
		Sk->SetupAttachment(GetMesh());
		Sk->bUseAttachParentBound = true;
	}
	return Sk;
}

bool AHumanCharacter::StartDodging()
{
	return false;
}

bool AHumanCharacter::StopDodging()
{
	return false;
}

void AHumanCharacter::EnableCharacterGuard()
{
}

void AHumanCharacter::DisableCharacterGuard()
{
}

void AHumanCharacter::OnPressedForward()
{
	bBackwardPressed = false;
	bForwardPressed = true;
	GetWorld()->GetTimerManager().SetTimer(SPAttackTimerHandle, this, &AHumanCharacter::DisableForwardPressed, 0.1f, false);
}

void AHumanCharacter::OnPressedBackward()
{
	bForwardPressed = false;
	bBackwardPressed = true;
	GetWorld()->GetTimerManager().SetTimer(SPAttackTimerHandle, this, &AHumanCharacter::DisableBackwardPressed, 0.1f, false);
}

void AHumanCharacter::OnReleasedForward()
{
}

void AHumanCharacter::OnReleasedBackward()
{
}

void AHumanCharacter::DisableForwardPressed()
{
	bForwardPressed = false;
}

void AHumanCharacter::DisableBackwardPressed()
{
	bBackwardPressed = false;
}

void AHumanCharacter::TurnOnTargetSwitch()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	// Change material parameter only if character is NOT controlled by player
	if (!PC)
	{
		Super::TurnOnTargetSwitch();

		FName ParamName = FName("Target_Switch_On");
		Hair->SetScalarParameterValueOnMaterials(ParamName, 1.f);
		HatItem->SetScalarParameterValueOnMaterials(ParamName, 1.f);
		FaceItem->SetScalarParameterValueOnMaterials(ParamName, 1.f);
		Chest->SetScalarParameterValueOnMaterials(ParamName, 1.f);
		Hands->SetScalarParameterValueOnMaterials(ParamName, 1.f);
		Legs->SetScalarParameterValueOnMaterials(ParamName, 1.f);
		Feet->SetScalarParameterValueOnMaterials(ParamName, 1.f);
	}
}

void AHumanCharacter::TurnOffTargetSwitch()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	// Change material parameter only if character is NOT controlled by player
	if (!PC)
	{
		Super::TurnOnTargetSwitch();

		FName ParamName = FName("Target_Switch_On");
		Hair->SetScalarParameterValueOnMaterials(ParamName, 0.f);
		HatItem->SetScalarParameterValueOnMaterials(ParamName, 0.f);
		FaceItem->SetScalarParameterValueOnMaterials(ParamName, 0.f);
		Chest->SetScalarParameterValueOnMaterials(ParamName, 0.f);
		Hands->SetScalarParameterValueOnMaterials(ParamName, 0.f);
		Legs->SetScalarParameterValueOnMaterials(ParamName, 0.f);
		Feet->SetScalarParameterValueOnMaterials(ParamName, 0.f);
	}
}
