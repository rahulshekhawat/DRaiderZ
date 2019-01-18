// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODLoadingScreen.h"

#include "Modules/ModuleManager.h"
#include "GenericApplication.h"
#include "GenericApplicationMessageHandler.h"
#include "MoviePlayer.h"
#include "SlateBasics.h"
#include "SlateExtras.h"

// #include "GenericApplication.h"


struct FEODLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FEODLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize) : FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		UObject* TextureObject = LoadObject<UObject>(NULL, *InTextureName.ToString());
		if (TextureObject)
		{
			SetResourceObject(TextureObject);
		}
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		if (GetResourceObject())
		{
			UObject* Obj = GetResourceObject();
			Collector.AddReferencedObject(Obj);
		}
	}
};

class SEODLoadingScreenWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEODLoadingScreenWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		// @todo loading screen image
		// static const FName LoadingScreenName(TEXT("/Game/UI/Menu/LoadingScreen.LoadingScreen"));
		static const FName LoadingScreenName(TEXT("/Game/Dev/UI/Background/T_DeskSS_01.T_DeskSS_01"));

		//since we are not using game styles here, just load one image
		LoadingScreenBrush = MakeShareable(new FEODLoadingScreenBrush(LoadingScreenName, FVector2D(1920, 1080)));

		ChildSlot
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(LoadingScreenBrush.Get())
			]
		+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SSafeZone)
				.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Right)
			.Padding(10.0f)
			.IsTitleSafe(true)
			[
				SNew(SThrobber)
				.Visibility(this, &SEODLoadingScreenWidget::GetLoadIndicatorVisibility)
			]
			]
			];
	}

private:
	EVisibility GetLoadIndicatorVisibility() const
	{
		return EVisibility::Visible;
	}

	/** loading screen image brush */
	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};


// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

class FEODLoadingScreenModule : public IEODLoadingScreenModule
{
public:
	virtual void StartupModule() override
	{
		// @todo reference to loading screen image
		// Load for cooker reference
		// LoadObject<UObject>(NULL, TEXT("/Game/UI/Menu/LoadingScreen.LoadingScreen"));
		LoadObject<UObject>(NULL, TEXT("/Game/Dev/UI/Background/T_DeskSS_01.T_DeskSS_01"));

		if (IsMoviePlayerEnabled())
		{
			FLoadingScreenAttributes LoadingScreen;
			LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
			LoadingScreen.MoviePaths.Add(TEXT("LoadingScreen"));
			GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
		}
	}

	// virtual void ShutdownModule() override {}

	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartInGameLoadingScreen() override
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.WidgetLoadingScreen = SNew(SEODLoadingScreenWidget);

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
};


IMPLEMENT_GAME_MODULE(FEODLoadingScreenModule, EODLoadingScreen);
