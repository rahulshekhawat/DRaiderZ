// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/Characters/HumanCharacter.h"
#include "EOD/Characters/Components/EODCharacterMovementComponent.h"
#include "EOD/Statics/EODGlobalNames.h"

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

void AHumanCharacter::SaveCharacterState()
{
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

bool AHumanCharacter::CanDodge() const
{
	return IsIdleOrMoving() || IsBlocking() || IsCastingSpell() || IsNormalAttacking();
}

bool AHumanCharacter::StartDodge()
{
	return false;
}

bool AHumanCharacter::StopDodge()
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
	// Change material parameter only if character is NOT controlled by local player
	if (GetController() && GetController()->IsLocalPlayerController())
	{
		Super::TurnOnTargetSwitch();

		Hair->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 1.f);
		HatItem->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 1.f);
		FaceItem->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 1.f);
		Chest->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 1.f);
		Hands->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 1.f);
		Legs->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 1.f);
		Feet->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 1.f);
	}
}

void AHumanCharacter::TurnOffTargetSwitch()
{
	// Change material parameter only if character is NOT controlled by local player
	if (GetController() && GetController()->IsLocalPlayerController())
	{
		Super::TurnOnTargetSwitch();

		Hair->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 0.f);
		HatItem->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 0.f);
		FaceItem->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 0.f);
		Chest->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 0.f);
		Hands->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 0.f);
		Legs->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 0.f);
		Feet->SetScalarParameterValueOnMaterials(MaterialParameterNames::TargetSwitchOn, 0.f);
	}


	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
	}
}

void AHumanCharacter::Server_StartDodge_Implementation()
{
}

bool AHumanCharacter::Server_StartDodge_Validate()
{
	return true;
}

void AHumanCharacter::Multicast_StartDodge_Implementation()
{
}
