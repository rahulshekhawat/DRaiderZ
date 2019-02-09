// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EOD/Core/EODPreprocessors.h"
#include "EOD/Statics/EODLibrary.h"
#include "EOD/Statics/EODGlobalNames.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "EODItemContainer.generated.h"

class UDragVisualWidget;

/**
 * 
 */
UCLASS()
class EOD_API UEODItemContainer : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UEODItemContainer(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "EOD Item Container", meta = (BindWidget))
	UImage* EmptyBorderImage;

	UPROPERTY(BlueprintReadOnly, Category = "EOD Item Container", meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(BlueprintReadOnly, Category = "EOD Item Container", meta = (BindWidget))
	UTextBlock* StackCountText;

	UPROPERTY(BlueprintReadOnly, Category = "EOD Item Container", meta = (BindWidget))
	UTextBlock* CooldownText;

protected:
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EOD Item Container")
	FEODItemInfo EODItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EOD Item Container")
	EEODContainerType ContainerType;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Item Container")
	bool bInCooldown;

protected:	
	UPROPERTY(EditAnywhere, Category = "EOD Item Container")
	UMaterialInterface* EmptyBorderMaterial;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Item Container")
	UMaterialInstanceDynamic* EmptyBorderMID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EOD Item Container")
	FLinearColor NormalBorderColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EOD Item Container")
	FLinearColor HoveredBorderColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EOD Item Container")
	FLinearColor PressedBorderColor;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Item Container")
	bool bCanBeClicked;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "EOD Item Container")
	bool bCanBeDragged;

private:
	UPROPERTY(Transient)
	float CooldownTimeRemaining;

	UPROPERTY(Transient)
	float CooldownInterval;

	FTimerHandle CooldownTimerHandle;

	UPROPERTY()
	UUserWidget* ParentWidget;

public:
	/** Puts this container on cooldown for a given duration */
	inline void StartCooldown(float Duration, float Interval = 1.f);

	/** Removes cooldown from this container */
	inline void StopCooldown();

	/** Resets and nulls all container variables. Deletes any references */
	inline void ResetContainer();

	/** Puts this container on cooldown for a given duration */
	UFUNCTION(BlueprintCallable, Category = EODItemContainer, meta = (DisplayName = "Start Cooldown"))
	void BP_StartCooldown(float Duration, float Interval = 1.f);

	/** Removes cooldown from this container */
	UFUNCTION(BlueprintCallable, Category = EODItemContainer, meta = (DisplayName = "Stop Cooldown"))
	void BP_StopCooldown();

	/** Refresh and update the displayed visuals of this container */
	virtual void RefreshContainerVisuals();

	FORCEINLINE void SetParentWidget(UUserWidget* Widget)
	{
		ParentWidget = IsValid(Widget) ? Widget : nullptr;
	}

	inline void DisableContainer();

	inline void DisableClicking();

	inline void DisableDragging();

	inline void EnableContainer();

	inline void EnableClicking();

	inline void EnableDragging();

	void AddSkill(FString& SkillGroup);

private:
	inline void UpdateItemImage();

	inline void UpdateStackCountText();

	inline void SetupEmptyBorderMaterial();

	void UpdateCooldown();


};

inline void UEODItemContainer::ResetContainer()
{
	EODItemInfo 				= FEODItemInfo();
	EODItemInfo.StackCount 		= 0;
	bCanBeClicked 				= false;
	bCanBeDragged 				= false;
	bInCooldown 				= false;
	CooldownTimeRemaining 		= 0;
	CooldownInterval 			= 0;
	
	// @note can't and shouldn't reset container type
}

inline void UEODItemContainer::DisableContainer()
{
	DisableClicking();
	DisableDragging();

	if (IsValid(ItemImage))
	{
		ItemImage->SetIsEnabled(false);
	}

	if (IsValid(EmptyBorderMID))
	{
		EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, NormalBorderColor);
	}
}

inline void UEODItemContainer::DisableClicking()
{
	bCanBeClicked = false;
}

inline void UEODItemContainer::DisableDragging()
{
	bCanBeDragged = false;
}

inline void UEODItemContainer::EnableContainer()
{
	EnableClicking();
	EnableDragging();

	if (IsValid(ItemImage))
	{
		ItemImage->SetIsEnabled(true);
	}
}

inline void UEODItemContainer::EnableClicking()
{
	bCanBeClicked = true;
}

inline void UEODItemContainer::EnableDragging()
{
	bCanBeDragged = true;
}

inline void UEODItemContainer::StartCooldown(float Duration, float Interval)
{
	UWorld* World = IsValid(GEngine) ? GEngine->GetWorldFromContextObjectChecked(this) : nullptr;
	if (IsValid(World) && IsValid(ItemImage) && IsValid(CooldownText))
	{
		World->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UEODItemContainer::UpdateCooldown, Interval, true, 0.f);

		CooldownTimeRemaining = Duration;
		CooldownInterval = Interval;
		bInCooldown = true;
		ItemImage->SetIsEnabled(false);
		CooldownText->SetVisibility(ESlateVisibility::Visible);
	}
}

inline void UEODItemContainer::StopCooldown()
{
	UWorld* World = IsValid(GEngine) ? GEngine->GetWorldFromContextObjectChecked(this) : nullptr;
	if (IsValid(World) && IsValid(ItemImage) && IsValid(CooldownText))
	{
		World->GetTimerManager().ClearTimer(CooldownTimerHandle);

		bInCooldown = false;
		ItemImage->SetIsEnabled(true);
		CooldownText->SetVisibility(ESlateVisibility::Hidden);
	}
}

inline void UEODItemContainer::UpdateItemImage()
{
	if (IsValid(ItemImage))
	{
		if (IsValid(EODItemInfo.Icon))
		{
			FSlateBrush SlateBrush;
			SlateBrush.ImageSize = FVector2D(52.0, 52.0);
			SlateBrush.DrawAs = ESlateBrushDrawType::Image;
			SlateBrush.ImageType = ESlateBrushImageType::FullColor;
			SlateBrush.SetResourceObject(EODItemInfo.Icon);
			ItemImage->SetBrush(SlateBrush);
		}
		else
		{
			FSlateBrush SlateBrush;
			SlateBrush.ImageSize = FVector2D(52.0, 52.0);
			SlateBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
			SlateBrush.ImageType = ESlateBrushImageType::NoImage;
			ItemImage->SetBrush(SlateBrush);
		}
	}
}

inline void UEODItemContainer::UpdateStackCountText()
{
	if (IsValid(StackCountText))
	{
		if (EODItemInfo.StackCount > 1)
		{
			FText Text = FText::FromString(FString::FromInt(EODItemInfo.StackCount));
			StackCountText->SetText(Text);
			StackCountText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			FText Text = FText::FromString(FString(""));
			StackCountText->SetText(Text);
			StackCountText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

inline void UEODItemContainer::SetupEmptyBorderMaterial()
{
	if (IsValid(EmptyBorderMaterial) && IsValid(EmptyBorderImage))
	{
		EmptyBorderMID = UMaterialInstanceDynamic::Create(EmptyBorderMaterial, this);
		if (IsValid(EmptyBorderMID))
		{
			EmptyBorderMID->SetVectorParameterValue(MaterialParameterNames::BaseColor, NormalBorderColor);

			FSlateBrush SlateBrush;
			SlateBrush.ImageSize = FVector2D(64.0, 64.0);
			SlateBrush.DrawAs = ESlateBrushDrawType::Image;
			SlateBrush.ImageType = ESlateBrushImageType::FullColor;
			SlateBrush.SetResourceObject(EmptyBorderMID);
			EmptyBorderImage->SetBrush(SlateBrush);
		}
	}
}
