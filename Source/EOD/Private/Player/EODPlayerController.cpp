// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODPlayerController.h"
#include "EOD.h"
#include "EODGameInstance.h"
#include "PlayerCharacter.h"
#include "SkillTreeComponent.h"
#include "GameplaySkillsComponent.h"
#include "InventoryComponent.h"
#include "PlayerStatsComponent.h"
#include "PlayerSkillsComponent.h"
#include "InteractionInterface.h"
#include "DynamicHUDWidget.h"
#include "EODGameModeBase.h"
#include "StatusIndicatorWidget.h"
#include "InventoryWidget.h"
#include "PlayerStatsWidget.h"
#include "SkillBarWidget.h"
#include "SkillTreeWidget.h"
#include "InteractivePopupWidget.h"

#include "ActiveSkillBase.h"
#include "EODLibrary.h"
#include "PlayerSaveGame.h"

#include "SkillTreeComponent.h"
#include "PlayerStatsComponent.h"

#include "Engine/World.h"
#include "UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerInput.h"
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

	check(InputComponent);

	InputComponent->BindAction("Forward", IE_Pressed, this, &AEODPlayerController::OnPressedForward);
	InputComponent->BindAction("Forward", IE_Released, this, &AEODPlayerController::OnReleasedForward);
	InputComponent->BindAction("Backward", IE_Pressed, this, &AEODPlayerController::OnPressedBackward);
	InputComponent->BindAction("Backward", IE_Released, this, &AEODPlayerController::OnReleasedBackward);
	InputComponent->BindAction("Left", IE_Pressed, this, &AEODPlayerController::OnPressedLeft);
	InputComponent->BindAction("Left", IE_Released, this, &AEODPlayerController::OnReleasedLeft);
	InputComponent->BindAction("Right", IE_Pressed, this, &AEODPlayerController::OnPressedRight);
	InputComponent->BindAction("Right", IE_Released, this, &AEODPlayerController::OnReleasedRight);

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
	InputComponent->BindAction("ToggleJournal", IE_Pressed, this, &AEODPlayerController::TogglePlayerJournalUI);
	InputComponent->BindAction("ToggleWeapon", IE_Pressed, this, &AEODPlayerController::TogglePlayerWeapon);

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
}

void AEODPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void AEODPlayerController::BeginPlay()
{
	Super::BeginPlay();

	LoadPlayerState();

	CreateHUDWidget();
	InitWidgets();
	BindWidgetDelegates();

	SwitchToGameInput();
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
	bool bLocalPC = IsLocalPlayerController();
	if (!bLocalPC)
	{
		return;
	}

	UEODGameInstance* EODGI = Cast<UEODGameInstance>(GetGameInstance());
	UPlayerSaveGame* SaveGame = EODGI ? EODGI->GetCurrentPlayerSaveGameObject() : nullptr;
	UDataTable* PSDataTable = EODGI ? EODGI->PlayerStatsDataTable : nullptr;

	check(SaveGame);

	// Load player gender
	SetGender(SaveGame->CharacterGender);






	// SetLeveupEXP()
	

	// int32 CharacterLevel = 1;




	/*
	// Load player level and exp
	if (SaveGame->CharacterLevel <= 0)
	{
		SaveGame->CharacterLevel = 1;
		if (PSDataTable)
		{
			FName LevelName = FName(*FString::FromInt(SaveGame->CharacterLevel + 1));
			FPlayerStatsTableRow* TableRow = PSDataTable->FindRow<FPlayerStatsTableRow>(LevelName, FString("APlayerCharacter::LoadCharacterState()"));
			if (TableRow)
			{
				SetLeveupEXP(TableRow->ExpRequired);
				SaveGame->LevelupEXP = TableRow->ExpRequired;
			}
		}
	}
	else if (SaveGame->CharacterLevel >= 100)
	{
		SaveGame->CharacterLevel = 99;
		SetLeveupEXP(0);
		SaveGame->LevelupEXP = 0;
	}
	else
	{
		SetLeveupEXP(SaveGame->LevelupEXP);
	}
	*/
}

void AEODPlayerController::AddEXP(int32 Value)
{
	SetLeveupEXP(LeveupEXP - Value);
}

void AEODPlayerController::SetLeveupEXP(int32 EXP)
{
	//~ @todo Level up if LevelupEXP goes below zero

	LeveupEXP = EXP;

	if (HUDWidget)
	{
		UPlayerStatsWidget* PlayerStatsWidget = HUDWidget->GetPlayerStatsWidget();
		if (PlayerStatsWidget)
		{
			PlayerStatsWidget->UpdateEXP(EXP);
		}
	}
}

void AEODPlayerController::SetGender(ECharacterGender NewGender)
{
	Gender = NewGender;
	if (Role < ROLE_Authority)
	{
		Server_SetGender(NewGender);
	}
}

UInteractivePopupWidget* AEODPlayerController::GetActivePopupWidget() const
{
	return HUDWidget ? HUDWidget->GetInteractivePopupWidget() : nullptr;
}

void AEODPlayerController::RegisterPopupWidget(UObject* RegisteringObj, const FString& InKeyText, const FString& InDetailText, UTexture* InIcon)
{
	UInteractivePopupWidget* IPWidget = GetActivePopupWidget();
	if (RegisteringObj && IPWidget)
	{
		UnregisterActivePopupWidget();

		IPWidget->RegisterWithObject(RegisteringObj);

		if (InKeyText == TEXT(""))
		{
			IPWidget->ResetKeyText();
		}
		else
		{
			FString SubTextString = TEXT("<SubText>") + InKeyText + TEXT("</>");
			IPWidget->SetKeyText(FText::FromString(SubTextString));
		}

		if (InDetailText == TEXT(""))
		{
			IPWidget->ResetDetailText();
		}
		else
		{
			IPWidget->SetDetailText(FText::FromString(InDetailText));
		}

		if (InIcon)
		{
			IPWidget->SetIcon(InIcon);
		}
		else
		{
			IPWidget->ResetIcon();
		}
	}
}

void AEODPlayerController::UnregisterPopupWidget(UObject* RegisteringObj)
{
	UInteractivePopupWidget* IPWidget = GetActivePopupWidget();
	if (IPWidget)
	{
		bool bResult = IPWidget->IsRegisteredWithObject(RegisteringObj);
		if (bResult)
		{
			IPWidget->Unregister();
		}
	}
}

void AEODPlayerController::UnregisterActivePopupWidget()
{
	UInteractivePopupWidget* IPWidget = GetActivePopupWidget();
	if (IPWidget)
	{
		IPWidget->Unregister();
	}
}

void AEODPlayerController::CreateHUDWidget()
{
	bool bLocalPC = IsLocalPlayerController();
	if (!bLocalPC || (bLocalPC && HUDWidget) || HUDWidgetClass.Get() == nullptr)
	{
		return;
	}

	HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}
}

void AEODPlayerController::InitWidgets()
{
	if (HUDWidget == nullptr)
	{
		return;
	}

	InitHUDWidget();
	InitStatusIndicatorWidget();
	InitSkillTreeWidget();
	InitSkillBarWidget();
	InitInventoryWidget();
	InitPlayerStatsWidget();
}

void AEODPlayerController::BindWidgetDelegates()
{
	if (HUDWidget == nullptr)
	{
		return;
	}

	BindHUDDelegates();
	BindStatusIndicatorDelegates();
	BindSkillTreeDelegates();
	BindSkillBarDelegates();
	BindInventoryDelegates();
	BindPlayerStatsDelegates();
}

void AEODPlayerController::UnbindWidgetDelegates()
{
	if (HUDWidget == nullptr)
	{
		return;
	}

	UnbindHUDDelegates();
	UnbindStatusIndicatorDelegates();
	UnbindSkillTreeDelegates();
	UnbindSkillBarDelegates();
	UnbindInventoryDelegates();
	UnbindPlayerStatsDelegates();
}

void AEODPlayerController::InitHUDWidget()
{
	//~ @todo load player name and level
	if (HUDWidget)
	{
		UEODGameInstance* EODGI = Cast<UEODGameInstance>(GetGameInstance());
		UPlayerSaveGame* SaveGame = EODGI ? EODGI->GetCurrentPlayerSaveGameObject() : nullptr;
		check(SaveGame);

		HUDWidget->SetPlayerName(EODGI->GetPlayerName());
		HUDWidget->SetPlayerLevel(SaveGame->CharacterLevel);
	}
}

void AEODPlayerController::InitStatusIndicatorWidget()
{
	UStatusIndicatorWidget* SIWidget = GetStatusIndicatorWidget();
	if (SIWidget && StatsComponent)
	{
		SIWidget->UpdateHealthBar(StatsComponent->Health.GetMaxValue(), StatsComponent->Health.GetCurrentValue());
		SIWidget->UpdateManaBar(StatsComponent->Mana.GetMaxValue(), StatsComponent->Mana.GetCurrentValue());
		SIWidget->UpdateStaminaBar(StatsComponent->Stamina.GetMaxValue(), StatsComponent->Stamina.GetCurrentValue());
	}
}

void AEODPlayerController::InitInventoryWidget()
{
	UInventoryWidget* InvWidget = GetInventoryWidget();
	if (InvWidget && InventoryComponent)
	{
		const TArray<FInventoryItem>& Items = InventoryComponent->GetInventoryItems();
		for (const FInventoryItem& Item : Items)
		{
			InvWidget->AddItem(Item);
		}
	}
}

void AEODPlayerController::InitSkillTreeWidget()
{
	UPlayerSkillsComponent* SkillsComp = EODCharacter ? Cast<UPlayerSkillsComponent>(EODCharacter->GetGameplaySkillsComponent()) : nullptr;

	USkillTreeWidget* STWidget = GetSkillTreeWidget();
	USkillPointsInfoWidget* SPIWidget = STWidget ? STWidget->GetSkillPointsInfoWidget() : nullptr;

	UEODGameInstance* GI = Cast<UEODGameInstance>(GetGameInstance());
	UPlayerSaveGame* SaveGame = GI ? GI->GetCurrentPlayerSaveGameObject() : nullptr;
	
	if (STWidget && SPIWidget && SkillsComp)
	{
		FSkillPointsAllocationInfo SPAllocationInfo = SaveGame ? SaveGame->SkillPointsAllocationInfo : FSkillPointsAllocationInfo();

		int32 PointsUnlockedByDefault = SkillsComp->GetSkillPointsUnlockedByDefault();
		if ((SPAllocationInfo.AvailableSkillPoints + SPAllocationInfo.UsedSkillPoints) < PointsUnlockedByDefault)
		{
			SPAllocationInfo.AvailableSkillPoints = PointsUnlockedByDefault - SPAllocationInfo.UsedSkillPoints;
		}

		SPIWidget->UpdateAvailableSkillPointsText(SPAllocationInfo.AvailableSkillPoints);
		SPIWidget->UpdateUsedSkillPointsText(SPAllocationInfo.UsedSkillPoints);

		SPIWidget->UpdateAssassinPointsText(SPAllocationInfo.AssassinPoints);
		SPIWidget->UpdateBerserkerPointsText(SPAllocationInfo.BerserkerPoints);
		SPIWidget->UpdateClericPointsText(SPAllocationInfo.ClericPoints);
		SPIWidget->UpdateDefenderPointsText(SPAllocationInfo.DefenderPoints);
		SPIWidget->UpdateSorcererPointsText(SPAllocationInfo.SorcererPoints);

		STWidget->InitializeSkillTreeLayout(SkillsComp->SkillTreeLayoutTable, SkillsComp);

	}


	//~ @todo modularize the logic for widgets and local skill data

	//~ !todo
	/*
	AEODPlayerController* PC = Cast<AEODPlayerController>(GetOuter());
	// UDynamicHUDWidget* HUDWidget = PC ? PC->GetHUDWidget() : nullptr;
	UDynamicHUDWidget* HUDWidget = nullptr;
	SkillTreeWidget = HUDWidget ? HUDWidget->GetSkillTreeWidget() : nullptr;
	SkillPointsInfoWidget = SkillTreeWidget ? SkillTreeWidget->GetSkillPointsInfoWidget() : nullptr;

	UEODGameInstance* GI = Cast<UEODGameInstance>(PC->GetGameInstance());
	UPlayerSaveGame* SaveGame = GI ? GI->GetCurrentPlayerSaveGameObject() : nullptr;

	if (SaveGame)
	{
		SetSkillPointsAllocationInfo(SaveGame->SkillPointsAllocationInfo);
		SkillTreeSlotsSaveData = SaveGame->SkillTreeSlotsSaveData;
	}
	else
	{
		SetSkillPointsAllocationInfo(SkillPointsAllocationInfo);
	}

	if (SkillPointsAllocationInfo.AvailableSkillPoints + SkillPointsAllocationInfo.UsedSkillPoints < SkillPointsUnlockedByDefault)
	{
		SetAvailableSkillPoints(SkillPointsUnlockedByDefault - SkillPointsAllocationInfo.UsedSkillPoints);
	}

	if (SkillTreeWidget)
	{
		SkillTreeWidget->InitializeSkillTreeLayout(this, SkillTreeLayoutTable, SkillTreeSlotsSaveData);
	}

	SkillTreeWidget->UpdateSkillSlots();
	*/
}

void AEODPlayerController::InitSkillBarWidget()
{
	USkillBarWidget* SBWidget = GetSkillBarWidget();
	UPlayerSkillsComponent* SkillsComp = EODCharacter ? Cast<UPlayerSkillsComponent>(EODCharacter->GetGameplaySkillsComponent()) : nullptr;
	if (SBWidget && SkillsComp)
	{
		SBWidget->SetOwnerSkillsComponent(SkillsComp);
		SBWidget->InitializeSkillBarLayout(SkillsComp->GetSkillBarMap(), SkillsComp->GetSkillsMap());
	}
}

void AEODPlayerController::InitPlayerStatsWidget()
{
	//~ @todo
	/*
	if (HUDWidget && EODGI)
	{
		FString PlayerName = EODGI->GetCurrentPlayerSaveGameName();
		HUDWidget->SetPlayerName(PlayerName);

		UPlayerStatsWidget* PSWidget = HUDWidget->GetPlayerStatsWidget();
		if (PSWidget)
		{
			PSWidget->SetPlayerName(PlayerName);

			FString Type = FString("Human");
			if (Gender == ECharacterGender::Female)
			{
				Type += FString(" Female");
			}
			else if (Gender == ECharacterGender::Male)
			{
				Type += FString(" Male");
			}
			PSWidget->SetPlayerType(Type);
		}
	}
	*/


}

void AEODPlayerController::BindHUDDelegates()
{
}

void AEODPlayerController::BindStatusIndicatorDelegates()
{
	UStatusIndicatorWidget* StatusIndicatorWidget = GetStatusIndicatorWidget();
	if (StatusIndicatorWidget)
	{
		check(StatsComponent);

		if (!StatsComponent->Health.OnStatValueChanged.IsBoundToObject(StatusIndicatorWidget))
		{
			StatsComponent->Health.OnStatValueChanged.AddUObject(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateHealthBar);
		}

		if (!StatsComponent->Mana.OnStatValueChanged.IsBoundToObject(StatusIndicatorWidget))
		{
			StatsComponent->Mana.OnStatValueChanged.AddUObject(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateManaBar);
		}

		if (!StatsComponent->Stamina.OnStatValueChanged.IsBoundToObject(StatusIndicatorWidget))
		{
			StatsComponent->Stamina.OnStatValueChanged.AddUObject(StatusIndicatorWidget, &UStatusIndicatorWidget::UpdateStaminaBar);
		}
	}
}

void AEODPlayerController::BindSkillTreeDelegates()
{
}

void AEODPlayerController::BindSkillBarDelegates()
{
}

void AEODPlayerController::BindInventoryDelegates()
{
	UInventoryWidget* InvWidget = GetInventoryWidget();
	if (InvWidget && InventoryComponent)
	{
		InventoryComponent->OnInventoryItemAdded.AddDynamic(InvWidget, &UInventoryWidget::AddItem);
		InventoryComponent->OnInventoryItemRemoved.AddDynamic(InvWidget, &UInventoryWidget::RemoveItem);
	}
}

void AEODPlayerController::BindPlayerStatsDelegates()
{
	UPlayerStatsWidget* StatsWidget = GetPlayerStatsWidget();
	if (StatsWidget)
	{
		check(StatsComponent);

		if (!StatsComponent->Health.OnStatValueChanged.IsBoundToObject(StatsWidget))
		{
			StatsComponent->Health.OnStatValueChanged.AddUObject(StatsWidget, &UPlayerStatsWidget::UpdateHealth);
		}

		if (!StatsComponent->Mana.OnStatValueChanged.IsBoundToObject(StatsWidget))
		{
			StatsComponent->Mana.OnStatValueChanged.AddUObject(StatsWidget, &UPlayerStatsWidget::UpdateMana);
		}

		if (!StatsComponent->Stamina.OnStatValueChanged.IsBoundToObject(StatsWidget))
		{
			StatsComponent->Stamina.OnStatValueChanged.AddUObject(StatsWidget, &UPlayerStatsWidget::UpdateStamina);
		}

		//~ Catch up UI to current stat values
		StatsComponent->Health.ForceBroadcastDelegate();
		StatsComponent->Mana.ForceBroadcastDelegate();
		StatsComponent->Stamina.ForceBroadcastDelegate();

		if (!StatsComponent->PhysicalAttack.OnStatValueChanged.IsBoundToObject(StatsWidget))
		{
			StatsComponent->PhysicalAttack.OnStatValueChanged.AddUObject(StatsWidget, &UPlayerStatsWidget::UpdatePAtt);
		}

		if (!StatsComponent->PhysicalCritRate.OnStatValueChanged.IsBoundToObject(StatsWidget))
		{
			StatsComponent->PhysicalCritRate.OnStatValueChanged.AddUObject(StatsWidget, &UPlayerStatsWidget::UpdatePCrit);
		}

		if (!StatsComponent->PhysicalResistance.OnStatValueChanged.IsBoundToObject(StatsWidget))
		{
			StatsComponent->PhysicalResistance.OnStatValueChanged.AddUObject(StatsWidget, &UPlayerStatsWidget::UpdatePDef);
		}

		if (!StatsComponent->MagickalAttack.OnStatValueChanged.IsBoundToObject(StatsWidget))
		{
			StatsComponent->MagickalAttack.OnStatValueChanged.AddUObject(StatsWidget, &UPlayerStatsWidget::UpdateMAtt);
		}

		if (!StatsComponent->MagickalCritRate.OnStatValueChanged.IsBoundToObject(StatsWidget))
		{
			StatsComponent->MagickalCritRate.OnStatValueChanged.AddUObject(StatsWidget, &UPlayerStatsWidget::UpdateMCrit);
		}

		if (!StatsComponent->MagickalResistance.OnStatValueChanged.IsBoundToObject(StatsWidget))
		{
			StatsComponent->MagickalResistance.OnStatValueChanged.AddUObject(StatsWidget, &UPlayerStatsWidget::UpdateMDef);
		}

		//~ Catch up UI to current stat values
		StatsComponent->PhysicalAttack.ForceBroadcastDelegate();
		StatsComponent->PhysicalCritRate.ForceBroadcastDelegate();
		StatsComponent->PhysicalResistance.ForceBroadcastDelegate();
		StatsComponent->MagickalAttack.ForceBroadcastDelegate();
		StatsComponent->MagickalCritRate.ForceBroadcastDelegate();
		StatsComponent->MagickalResistance.ForceBroadcastDelegate();
	}
}

void AEODPlayerController::UnbindHUDDelegates()
{
}

void AEODPlayerController::UnbindStatusIndicatorDelegates()
{
}

void AEODPlayerController::UnbindSkillTreeDelegates()
{
}

void AEODPlayerController::UnbindSkillBarDelegates()
{
}

void AEODPlayerController::UnbindInventoryDelegates()
{
}

void AEODPlayerController::UnbindPlayerStatsDelegates()
{
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

void AEODPlayerController::TogglePlayerStatsUI()
{
	UPlayerStatsWidget* StatsWidget = GetPlayerStatsWidget();
	check(StatsWidget);

	if (StatsWidget->IsVisible())
	{
		StatsWidget->SetVisibility(ESlateVisibility::Hidden);
		UGameplayStatics::PlaySound2D(this, UIDownSound);

		UInventoryWidget* InvWidget = HUDWidget->GetInventoryWidget();
		USkillTreeWidget* STWidget = HUDWidget->GetSkillTreeWidget();

		check(InvWidget);
		check(STWidget);

		if (!InvWidget->IsVisible() && !STWidget->IsVisible())
		{
			SwitchToGameInput();
		}
	}
	else
	{
		StatsWidget->SetVisibility(ESlateVisibility::Visible);
		UGameplayStatics::PlaySound2D(this, UIUpSound);

		SwitchToUIInput();
	}
}

void AEODPlayerController::TogglePlayerSkillTreeUI()
{
	USkillTreeWidget* STWidget = GetSkillTreeWidget();
	check(STWidget);

	if (STWidget->IsVisible())
	{
		STWidget->SetVisibility(ESlateVisibility::Hidden);
		UGameplayStatics::PlaySound2D(this, UIDownSound);

		UPlayerStatsWidget* StatsWidget = GetPlayerStatsWidget();
		UInventoryWidget* InvWidget = GetInventoryWidget();

		check(StatsWidget);
		check(InvWidget);

		if (!InvWidget->IsVisible() && !StatsWidget->IsVisible())
		{
			SwitchToGameInput();
		}
	}
	else
	{
		STWidget->SetVisibility(ESlateVisibility::Visible);
		UGameplayStatics::PlaySound2D(this, UIUpSound);

		SwitchToUIInput();
	}
}

void AEODPlayerController::TogglePlayerInventoryUI()
{
	UInventoryWidget* InvWidget = GetInventoryWidget();
	check(InvWidget);

	if (InvWidget->IsVisible())
	{
		InvWidget->SetVisibility(ESlateVisibility::Hidden);
		UGameplayStatics::PlaySound2D(this, UIDownSound);

		UPlayerStatsWidget* StatsWidget = GetPlayerStatsWidget();
		USkillTreeWidget* STWidget = GetSkillTreeWidget();

		check(StatsWidget);
		check(STWidget);
		
		if (!StatsWidget->IsVisible() && !STWidget->IsVisible())
		{
			SwitchToGameInput();
		}
	}
	else
	{
		InvWidget->SetVisibility(ESlateVisibility::Visible);
		UGameplayStatics::PlaySound2D(this, UIUpSound);

		SwitchToUIInput();
	}
}

void AEODPlayerController::TogglePlayerJournalUI()
{

}

void AEODPlayerController::TogglePlayerWeapon()
{
	AHumanCharacter* HumanChar = Cast<AHumanCharacter>(EODCharacter);
	if (IsValid(HumanChar))
	{
		HumanChar->ToggleWeapon();
	}
}

FName AEODPlayerController::GetKeyNameForActionName(FName InActionName)
{
	if (PlayerInput)
	{
		const TArray<FInputActionKeyMapping>& ActionKeys = PlayerInput->GetKeysForAction(InActionName);
		for (const FInputActionKeyMapping ActionKey : ActionKeys)
		{
			FName KeyName = ActionKey.Key.GetFName();

			if (KeyName == TEXT("One"))
			{
				return TEXT("1");
			}
			else if (KeyName == TEXT("Two"))
			{
				return TEXT("2");
			}
			else if (KeyName == TEXT("Three"))
			{
				return TEXT("3");
			}
			else if (KeyName == TEXT("Four"))
			{
				return TEXT("4");
			}
			else if (KeyName == TEXT("Five"))
			{
				return TEXT("5");
			}
			else if (KeyName == TEXT("Six"))
			{
				return TEXT("6");
			}
			else if (KeyName == TEXT("Seven"))
			{
				return TEXT("7");
			}
			else if (KeyName == TEXT("Eight"))
			{
				return TEXT("8");
			}
			else if (KeyName == TEXT("Nine"))
			{
				return TEXT("9");
			}
			else if (KeyName == TEXT("Zero"))
			{
				return TEXT("0");
			}
			else
			{
				return KeyName;
			}
		}
	}

	return NAME_None;
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

void AEODPlayerController::OnPressedLeft()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->OnPressedLeft();
	}
}

void AEODPlayerController::OnReleasedLeft()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->OnReleasedLeft();
	}
}

void AEODPlayerController::OnPressedRight()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->OnPressedRight();
	}
}

void AEODPlayerController::OnReleasedRight()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->OnReleasedRight();
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
	// Zoom in only if the mouse cursor is hidden. If mouse cursor is not hidden, it means the player is most likely interacting with UI.
	if (!bShowMouseCursor && IsValid(EODCharacter))
	{
		EODCharacter->ZoomInCamera();
	}
}

void AEODPlayerController::ZoomOutCamera()
{
	// Zoom out only if the mouse cursor is hidden. If mouse cursor is not hidden, it means the player is most likely interacting with UI.
	if (!bShowMouseCursor && IsValid(EODCharacter))
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

	if (!bShowMouseCursor && IsValid(EODCharacter))
	{
		EODCharacter->SetWantsToNormalAttack(true);
	}
}

void AEODPlayerController::OnReleasingNormalAttackKey()
{
	if (IsValid(EODCharacter))
	{
		EODCharacter->SetWantsToNormalAttack(false);
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

		// int32 DodgeCost = DodgeStaminaCost * StatsComponent->GetStaminaConsumptionModifier();
		int32 DodgeCost = DodgeStaminaCost;
		int32 CurrentStamina = StatsComponent->Stamina.GetCurrentValue();

		bool bCanDodge = (CurrentStamina >= DodgeCost) && EODCharacter->CanDodge();
		if (bCanDodge)
		{
			EODCharacter->StartDodge();
			Server_OnInitiateDodge();
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

	if (!bShowMouseCursor && IsValid(EODCharacter))
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
	if (IsValid(EODCharacter) && EODCharacter->IsInteracting())
	{
		EODCharacter->CancelInteraction(EInteractionCancelType::ManualCancel);
	}

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
			// GSComp->SaveSkillBarLayout();
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

void AEODPlayerController::Server_OnInitiateDodge_Implementation()
{
	check(StatsComponent);
	// int32 DodgeCost = DodgeStaminaCost * StatsComponent->GetStaminaConsumptionModifier();
	int32 DodgeCost = DodgeStaminaCost;
	StatsComponent->Stamina.ModifyCurrentValue(-DodgeCost);
}

bool AEODPlayerController::Server_OnInitiateDodge_Validate()
{
	return true;
}

void AEODPlayerController::Client_SetupLocalPlayerOnUnpossess_Implementation(APawn* InPawn)
{
	/*
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
	*/
}
