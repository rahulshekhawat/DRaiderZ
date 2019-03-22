// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/Characters/HumanCharacter.h"
#include "EOD/Characters/Components/EODCharacterMovementComponent.h"
#include "EOD/Statics/EODGlobalNames.h"

#include "GameFramework/PlayerController.h"

FName AHumanCharacter::HairComponentName(TEXT("Hair"));
FName AHumanCharacter::HatItemComponentName(TEXT("Hat Item"));
FName AHumanCharacter::FaceItemComponentName(TEXT("Face Item"));
FName AHumanCharacter::ChestComponentName(TEXT("Chest"));
FName AHumanCharacter::HandsComponentName(TEXT("Hands"));
FName AHumanCharacter::LegsComponentName(TEXT("Legs"));
FName AHumanCharacter::FeetComponentName(TEXT("Feet"));

AHumanCharacter::AHumanCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// @note Defaul skeletal mesh component inherited from ACharacter class will contain face mesh
	if (GetMesh())
	{
		GetMesh()->AddLocalOffset(FVector(0.f, 0.f, -90.f));
		GetMesh()->bUseAttachParentBound = true;
	}

	// @note : SetMasterPoseComponent() from constructor doesn't work in packaged game (for some weird reason?!)

	Hair				= CreateNewArmorComponent(AHumanCharacter::HairComponentName, ObjectInitializer);
	HatItem				= CreateNewArmorComponent(AHumanCharacter::HatItemComponentName, ObjectInitializer);
	FaceItem			= CreateNewArmorComponent(AHumanCharacter::FaceItemComponentName, ObjectInitializer);
	Chest				= CreateNewArmorComponent(AHumanCharacter::ChestComponentName, ObjectInitializer);
	Hands				= CreateNewArmorComponent(AHumanCharacter::HandsComponentName, ObjectInitializer);
	Legs				= CreateNewArmorComponent(AHumanCharacter::LegsComponentName, ObjectInitializer);
	Feet				= CreateNewArmorComponent(AHumanCharacter::FeetComponentName, ObjectInitializer);

	bUseControllerRotationYaw = false;

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

	// Since setting master pose component from constructor doesn't work in packaged game
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

void AHumanCharacter::StartDodge()
{
}

void AHumanCharacter::FinishDodge()
{
}

void AHumanCharacter::EnableCharacterGuard()
{
}

void AHumanCharacter::DisableCharacterGuard()
{
}

void AHumanCharacter::OnPressedForward()
{
	UWorld* World = GetWorld();
	if (World)
	{
		bBackwardPressed = false;
		bForwardPressed = true;
		World->GetTimerManager().SetTimer(SPAttackTimerHandle, this, &AHumanCharacter::DisableForwardPressed, 0.1f, false);
	}
}

void AHumanCharacter::OnPressedBackward()
{
	UWorld* World = GetWorld();
	if (World)
	{
		bForwardPressed = false;
		bBackwardPressed = true;
		World->GetTimerManager().SetTimer(SPAttackTimerHandle, this, &AHumanCharacter::DisableBackwardPressed, 0.1f, false);
	}
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
