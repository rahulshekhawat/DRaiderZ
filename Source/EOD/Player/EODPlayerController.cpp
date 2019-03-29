// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EOD/Player/EODPlayerController.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/Core/EODGameInstance.h"
#include "EOD/Characters/PlayerCharacter.h"
#include "EOD/Characters/Components/SkillTreeComponent.h"
#include "EOD/Characters/Components/GameplaySkillsComponent.h"
#include "EOD/Player/Components/InventoryComponent.h"
#include "EOD/Player/Components/PlayerStatsComponent.h"
#include "Components/PlayerSkillsComponent.h"

#include "DynamicHUDWidget.h"
#include "EODGameModeBase.h"
#include "StatusIndicatorWidget.h"
#include "InventoryWidget.h"
#include "PlayerStatsWidget.h"
#include "SkillBarWidget.h"
#include "DynamicSkillBarWidget.h"
#include "DynamicSkillTreeWidget.h"

#include "EOD/Statics/EODLibrary.h"
#include "EOD/SaveSystem/PlayerSaveGame.h"

#include "Components/SkillTreeComponent.h"
#include "PlayerSkillTreeManager.h"
#include "Components/PlayerStatsComponent.h"

#include "Engine/World.h"
#include "UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/SpringArmComponent.h"


const FName AEODPlayerController::InventoryComponentName(TEXT("Player Inventory"));
const FName AEODPlayerController::SkillTreeComponentName(TEXT("Player Skill Tree"));
const FName AEODPlayerController::StatsComponentName(TEXT("Player Stats"));

AEODPlayerController::AEODPlayerController(const FObjectInitializer & ObjectInitializer): Super(ObjectInitializer)
{
	InventoryComponent 	= ObjectInitializer.CreateDefaultSubobject<UInventoryComponent>(this, AEODPlayerController::InventoryComponentName);
	SkillTreeComponent = ObjectInitializer.CreateDefaultSubobject<USkillTreeComponent>(this, AEODPlayerController::SkillTreeComponentName);
	StatsComponent = ObjectInitializer.CreateDefaultSubobject<UPlayerStatsComponent>(this, AEODPlayerController::StatsComponentName);

	DodgeStaminaCost = 30;
	bEnableTouchEvents = false;
	bForceFeedbackEnabled = false;
}

void AEODPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent)
	{
		return;
	}

	InputComponent->BindAction("Forward", IE_Pressed, this, &AEODPlayerController::OnPressedForward);
	InputComponent->BindAction("Forward", IE_Released, this, &AEODPlayerController::OnReleasedForward);
	InputComponent->BindAction("Backward", IE_Pressed, this, &AEODPlayerController::OnPressedBackward);
	InputComponent->BindAction("Backward", IE_Released, this, &AEODPlayerController::OnReleasedBackward);

	//~ Mouse Input
	InputComponent->BindAxis("Turn", this, &AEODPlayerController::AddYawInput);
	InputComponent->BindAxis("LookUp", this, &AEODPlayerController::AddPitchInput);

	//~ Movement Input
	InputComponent->BindAxis("MoveForward",	this, &AEODPlayerController::MovePawnForward);
	InputComponent->BindAxis("MoveRight", this,	&AEODPlayerController::MovePawnRight);

	//~ Begin Action Input Bindings
	InputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &AEODPlayerController::ZoomInCamera);
	InputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &AEODPlayerController::ZoomOutCamera);

	InputComponent->BindAction("Guard", IE_Pressed, this, &AEODPlayerController::OnPressingGuardKey);
	InputComponent->BindAction("Guard", IE_Released, this, &AEODPlayerController::OnReleasingGuardKey);

	InputComponent->BindAction("NormalAttack", IE_Pressed, this, &AEODPlayerController::OnPressingNormalAttackKey);
	InputComponent->BindAction("NormalAttack", IE_Released, this, &AEODPlayerController::OnReleasingNormalAttackKey);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AEODPlayerController::MakePawnJump);
	InputComponent->BindAction("Dodge", IE_Pressed, this, &AEODPlayerController::AttemptDodge);
	InputComponent->BindAction("Interact", IE_Pressed, this, &AEODPlayerController::TriggerInteraction);

	FInputActionBinding& EscapeBinding = InputComponent->BindAction("Escape", IE_Pressed, this, &AEODPlayerController::OnPressingEscapeKey);
	EscapeBinding.bExecuteWhenPaused = true;

	InputComponent->BindAction("ToggleSheathe", IE_Pressed, this, &AEODPlayerController::ToggleSheathe);
	InputComponent->BindAction("ToggleAutoRun", IE_Pressed, this, &AEODPlayerController::ToggleAutoMove);
	InputComponent->BindAction("ToggleMouseCursor", IE_Pressed, this, &AEODPlayerController::ToggleMouseCursor);

	InputComponent->BindAction("ToggleStats", IE_Pressed, this, &AEODPlayerController::TogglePlayerStatsUI);
	InputComponent->BindAction("ToggleSkillTree", IE_Pressed, this, &AEODPlayerController::TogglePlayerSkillTreeUI);
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AEODPlayerController::TogglePlayerInventoryUI);

	InputComponent->BindAction("Skill_1", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<1>);
	InputComponent->BindAction("Skill_2", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<2>);
	InputComponent->BindAction("Skill_3", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<3>);
	InputComponent->BindAction("Skill_4", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<4>);
	InputComponent->BindAction("Skill_5", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<5>);
	InputComponent->BindAction("Skill_6", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<6>);
	InputComponent->BindAction("Skill_7", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<7>);
	InputComponent->BindAction("Skill_8", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<8>);
	InputComponent->BindAction("Skill_9", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<9>);
	InputComponent->BindAction("Skill_10", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<10>);
	InputComponent->BindAction("Skill_11", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<11>);
	InputComponent->BindAction("Skill_12", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<12>);
	InputComponent->BindAction("Skill_13", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<13>);
	InputComponent->BindAction("Skill_14", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<14>);
	InputComponent->BindAction("Skill_15", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<15>);
	InputComponent->BindAction("Skill_16", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<16>);
	InputComponent->BindAction("Skill_17", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<17>);
	InputComponent->BindAction("Skill_18", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<18>);
	InputComponent->BindAction("Skill_19", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<19>);
	InputComponent->BindAction("Skill_20", IE_Pressed, this, &AEODPlayerController::PressedSkillKey<20>);

	InputComponent->BindAction("Skill_1", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<1>);
	InputComponent->BindAction("Skill_2", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<2>);
	InputComponent->BindAction("Skill_3", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<3>);
	InputComponent->BindAction("Skill_4", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<4>);
	InputComponent->BindAction("Skill_5", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<5>);
	InputComponent->BindAction("Skill_6", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<6>);
	InputComponent->BindAction("Skill_7", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<7>);
	InputComponent->BindAction("Skill_8", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<8>);
	InputComponent->BindAction("Skill_9", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<9>);
	InputComponent->BindAction("Skill_10", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<10>);
	InputComponent->BindAction("Skill_11", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<11>);
	InputComponent->BindAction("Skill_12", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<12>);
	InputComponent->BindAction("Skill_13", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<13>);
	InputComponent->BindAction("Skill_14", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<14>);
	InputComponent->BindAction("Skill_15", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<15>);
	InputComponent->BindAction("Skill_16", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<16>);
	InputComponent->BindAction("Skill_17", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<17>);
	InputComponent->BindAction("Skill_18", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<18>);
	InputComponent->BindAction("Skill_19", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<19>);
	InputComponent->BindAction("Skill_20", IE_Released, this, &AEODPlayerController::ReleasedSkillKey<20>);

}

void AEODPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Load save game files?

}

void AEODPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void AEODPlayerController::BeginPlay()
{
	Super::BeginPlay();

	LoadPlayerState();
	InitializeWidgets();
}

void AEODPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAutoMoveEnabled() && IsValid(EODCharacter))
	{
		EODCharacter->MoveForward(1.f);
	}
}

void AEODPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	EODCharacter = InPawn ? Cast<AEODCharacterBase>(InPawn) : nullptr;
}

void AEODPlayerController::LoadPlayerState()
{
	if (IsLocalController())
	{
		UEODGameInstance* GameInstance = Cast<UEODGameInstance>(GetGameInstance());
		UPlayerSaveGame* PlayerSaveGame = GameInstance ? GameInstance->GetCurrentPlayerSaveGameObject() : nullptr;

		if (IsValid(PlayerSaveGame))
		{
			SetGender(PlayerSaveGame->CharacterGender);
		}
	}

	/*
	UEODGameInstance* GameInstance = Cast<UEODGameInstance>(GetGameInstance());
	UPlayerSaveGame* PlayerSaveGame = GameInstance ? GameInstance->GetCurrentPlayerSaveGameObject() : nullptr;
	if (IsValid(PlayerSaveGame) && IsValid(SkillTreeComponent))
	{
		
		// SkillTreeComponent->
	}
	*/
}

void AEODPlayerController::SetGender(ECharacterGender NewGender)
{
	Gender = NewGender;
	if (Role < ROLE_Authority)
	{
		Server_SetGender(NewGender);
	}
}

void AEODPlayerController::InitializeWidgets()
{
	if (!IsLocalPlayerController())
	{
		return;
	}
	
	InitializeHUDWidget();

	check(SkillTreeComponent);
	SkillTreeComponent->InitializeSkillTreeWidget();

	// If HUD widget created successfully
	if (HUDWidget)
	{
		InitStatusIndicatorWidget();
		InitSkillBarWidget();
		// InitInventoryWidget();
		// InitSkillTreeWidget();
	}
}

void AEODPlayerController::InitializeHUDWidget()
{
	if (!HUDWidget && IsLocalPlayerController() && HUDWidgetClass.Get())
	{
		HUDWidget = CreateWidget<UDynamicHUDWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}

void AEODPlayerController::InitStatusIndicatorWidget()
{
	if (HUDWidget && StatsComponent)
	{
		UStatusIndicatorWidget* StatusIndicatorWidget = HUDWidget->GetStatusIndicatorWidget();
		if (StatusIndicatorWidget)
		{
			StatsComponent->OnHealthChanged.AddUniqueDynamic(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateHealthBar);
			StatsComponent->OnManaChanged.AddUniqueDynamic(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateManaBar);
			StatsComponent->OnStaminaChanged.AddUniqueDynamic(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateStaminaBar);
		}
	}
}

void AEODPlayerController::InitInventoryWidget()
{
	if (IsValid(HUDWidget) && IsValid(HUDWidget->GetInventoryWidget()))
	{
		UInventoryWidget* InvWidget = HUDWidget->GetInventoryWidget();
		TArray<FInventoryItem> Items = InventoryComponent->GetInventoryItems();
		for (const FInventoryItem& Item : Items)
		{
			InvWidget->AddItem(Item);
		}

		InventoryComponent->OnInventoryItemAdded.AddDynamic(InvWidget, &UInventoryWidget::AddItem);
		InventoryComponent->OnInventoryItemRemoved.AddDynamic(InvWidget, &UInventoryWidget::RemoveItem);
	}
}

void AEODPlayerController::InitSkillTreeWidget()
{
	if (IsValid(HUDWidget) && IsValid(HUDWidget->GetSkillTreeWidget()))
	{



	}
}

void AEODPlayerController::InitSkillBarWidget()
{
	UPlayerSkillsComponent* SkillsComp = EODCharacter ? Cast<UPlayerSkillsComponent>(EODCharacter->GetGameplaySkillsComponent()) : nullptr;
	if (HUDWidget && SkillsComp)
	{
		UDynamicSkillBarWidget* SkillBarWidget = HUDWidget->GetSkillBarWidget();
		if (SkillBarWidget)
		{
			SkillBarWidget->SetSkillOwnerComponent(SkillsComp);
		}
	}	
}

void AEODPlayerController::TogglePlayerHUD()
{
	if (IsValid(HUDWidget) && HUDWidget->IsVisible())
	{
		HUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (IsValid(HUDWidget) && !HUDWidget->IsVisible())
	{
		HUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AEODPlayerController::TogglePlayerSkillTreeUI()
{
	UDynamicSkillTreeWidget* STWidget = HUDWidget ? HUDWidget->GetSkillTreeWidget() : nullptr;
	if (STWidget && STWidget->IsVisible())
	{
		STWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (STWidget && !STWidget->IsVisible())
	{
		STWidget->SetVisibility(ESlateVisibility::Visible);
		// STWidget->RefreshVisuals();
	}
}

void AEODPlayerController::TogglePlayerInventoryUI()
{
	UInventoryWidget* InventoryWidget = HUDWidget ? HUDWidget->GetInventoryWidget() : nullptr;
	if (InventoryWidget && InventoryWidget->IsVisible())
	{
		HUDWidget->GetInventoryWidget()->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (InventoryWidget && !InventoryWidget->IsVisible())
	{
		HUDWidget->GetInventoryWidget()->SetVisibility(ESlateVisibility::Visible);
	}
}

void AEODPlayerController::OnPressedForward()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->OnPressedForward();
	}
}

void AEODPlayerController::OnReleasedForward()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->OnReleasedForward();
	}
}

void AEODPlayerController::OnPressedBackward()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->OnPressedBackward();
	}
}

void AEODPlayerController::OnReleasedBackward()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->OnReleasedBackward();
	}
}

void AEODPlayerController::TogglePlayerStatsUI()
{
	if (IsValid(HUDWidget) && IsValid(HUDWidget->GetPlayerStatsWidget()) && HUDWidget->GetPlayerStatsWidget()->IsVisible())
	{
		HUDWidget->GetPlayerStatsWidget()->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (IsValid(HUDWidget) && IsValid(HUDWidget->GetPlayerStatsWidget()) && !HUDWidget->GetPlayerStatsWidget()->IsVisible())
	{
		HUDWidget->GetPlayerStatsWidget()->SetVisibility(ESlateVisibility::Visible);
	}
}

void AEODPlayerController::MovePawnForward(const float Value)
{
	if (Value != 0 && IsAutoMoveEnabled())
	{
		DisableAutoMove();
	}

	if (IsValid(EODCharacter))
	{
		EODCharacter->MoveForward(Value);
	}
}

void AEODPlayerController::MovePawnRight(const float Value)
{
	if (Value != 0 && IsAutoMoveEnabled())
	{
		DisableAutoMove();
	}

	if (IsValid(EODCharacter))
	{
		EODCharacter->MoveRight(Value);
	}
}

void AEODPlayerController::MakePawnJump()
{
	if (IsValid(EODCharacter) && EODCharacter->CanJump())
	{
		EODCharacter->Jump();
	}
}

void AEODPlayerController::ZoomInCamera()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->ZoomInCamera();
	}
}

void AEODPlayerController::ZoomOutCamera()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->ZoomOutCamera();
	}
}

void AEODPlayerController::OnPressingNormalAttackKey()
{
	if (IsAutoMoveEnabled())
	{
		DisableAutoMove();
	}

	if (IsValid(EODCharacter))
	{
		EODCharacter->SetWantsToNormalAttack(true);
		// EODCharacter->SetNormalAttackKeyPressed(true);
	}
}

void AEODPlayerController::OnReleasingNormalAttackKey()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->SetWantsToNormalAttack(false);
		// EODCharacter->SetNormalAttackKeyPressed(false);
	}
}

void AEODPlayerController::AttemptDodge()
{
	if (IsValid(EODCharacter) && IsValid(StatsComponent))
	{
		if (IsAutoMoveEnabled())
		{
			DisableAutoMove();
		}

		int32 DodgeCost = DodgeStaminaCost * StatsComponent->GetStaminaConsumptionModifier();
		int32 CurrentStamina = StatsComponent->GetCurrentStamina();

		bool bCanDodge = (CurrentStamina >= DodgeCost) && EODCharacter->CanDodge();
		if (bCanDodge)
		{
			EODCharacter->StartDodge();
			Server_OnSuccessfulDodge();
		}
	}
}

void AEODPlayerController::TriggerInteraction()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->TriggerInteraction();
	}
}

void AEODPlayerController::ToggleAutoMove()
{
	if (IsAutoMoveEnabled())
	{
		DisableAutoMove();
	}
	else
	{
		EnableAutoMove();
	}
}

void AEODPlayerController::ToggleMouseCursor()
{
	if (bShowMouseCursor)
	{
		SwitchToGameInput();
	}
	else
	{
		SwitchToUIInput();
	}
}

void AEODPlayerController::ToggleSheathe()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->ToggleSheathe();
	}
}

void AEODPlayerController::OnPressingGuardKey()
{
	if (IsAutoMoveEnabled())
	{
		DisableAutoMove();
	}

	if (IsValid(EODCharacter))
	{
		EODCharacter->SetWantsToGuard(true);
	}
}

void AEODPlayerController::OnReleasingGuardKey()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->SetWantsToGuard(false);
	}
}

void AEODPlayerController::OnPressingEscapeKey()
{
	if (IsPaused())
	{
		SetPause(false);
	}
	else
	{
		SetPause(true);
	}
}

void AEODPlayerController::OnPressingSkillKey(const int32 SkillKeyIndex)
{
	UPlayerSkillsComponent* SkillComp = EODCharacter ? Cast<UPlayerSkillsComponent>(EODCharacter->GetGameplaySkillsComponent()) : nullptr;
	if (SkillComp)
	{
		SkillComp->OnPressingSkillKey(SkillKeyIndex);
	}
}

void AEODPlayerController::OnReleasingSkillKey(const int32 SkillKeyIndex)
{
	UPlayerSkillsComponent* SkillComp = EODCharacter ? Cast<UPlayerSkillsComponent>(EODCharacter->GetGameplaySkillsComponent()) : nullptr;
	if (SkillComp)
	{
		SkillComp->OnReleasingSkillKey(SkillKeyIndex);
	}
}

void AEODPlayerController::SavePlayerState()
{
	if (IsValid(EODCharacter))
	{
		UGameplaySkillsComponent* GSComp = EODCharacter->GetGameplaySkillsComponent();
		if (IsValid(GSComp))
		{
			GSComp->SaveSkillBarLayout();
		}
	}
}

void AEODPlayerController::Server_SetGender_Implementation(ECharacterGender NewGender)
{
	SetGender(NewGender);

	APawn* TempPawn = GetPawn();
	UnPossess();
	TempPawn->Destroy();

	/* Just in case we didn't get the PawnClass on the Server in time... */
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}

bool AEODPlayerController::Server_SetGender_Validate(ECharacterGender NewGender)
{
	return true;
}

void AEODPlayerController::Server_OnSuccessfulDodge_Implementation()
{
	check(StatsComponent);
	int32 DodgeCost = DodgeStaminaCost * StatsComponent->GetStaminaConsumptionModifier();
	StatsComponent->ModifyCurrentStamina(-DodgeCost);
}

bool AEODPlayerController::Server_OnSuccessfulDodge_Validate()
{
	return true;
}

void AEODPlayerController::Client_SetupLocalPlayerOnUnpossess_Implementation(APawn* InPawn)
{
	AEODCharacterBase* EODChar = InPawn ? Cast<AEODCharacterBase>(InPawn) : nullptr;
	if (IsValid(EODChar) && IsValid(HUDWidget))
	{
		if (IsValid(EODChar->GetCharacterStatsComponent()) && IsValid(HUDWidget->GetStatusIndicatorWidget()))
		{
			UStatusIndicatorWidget* StatusIndicatorWidget = HUDWidget->GetStatusIndicatorWidget();
			UStatsComponentBase* StatsComp = EODChar->GetCharacterStatsComponent();
			StatsComp->OnHealthChanged.RemoveDynamic(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateHealthBar);
			StatsComp->OnManaChanged.RemoveDynamic(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateManaBar);
			StatsComp->OnStaminaChanged.RemoveDynamic(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateStaminaBar);
		}
	}
}
