// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/EODLibrary.h"
#include "Blueprint/UserWidget.h"
#include "EODItemContainer.generated.h"

class UTextBlock;
class UImage;
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

	UPROPERTY(Transient, BlueprintReadOnly, Category = Behavior)
	bool bCanBeClicked;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Behavior)
	bool bCanBeDragged;

	UPROPERTY(Transient, BlueprintReadWrite, Category = Behavior)
	bool bInCooldown;

	UPROPERTY(Transient)
	float CooldownTimeRemaining;

	UPROPERTY(Transient)
	float CooldownInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EODItemInfo)
	FEODItemInfo EODItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EODItemInfo)
	EEODContainerType ContainerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* EmptyBorderMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor NormalBorderColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor HoveredBorderColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor PressedBorderColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* EmptyBorderImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* StackCountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CooldownText;

	/** Puts this container on cooldown for a given duration */
	FORCEINLINE void StartCooldown(float Duration, float Interval = 1.f);

	/** Removes cooldown from this container */
	FORCEINLINE void StopCooldown();

	/** Puts this container on cooldown for a given duration */
	UFUNCTION(BlueprintCallable, Category = EODItemContainer, meta = (DisplayName = "Start Cooldown"))
	void BP_StartCooldown(float Duration, float Interval = 1.f);

	/** Removes cooldown from this container */
	UFUNCTION(BlueprintCallable, Category = EODItemContainer, meta = (DisplayName = "Stop Cooldown"))
	void BP_StopCooldown();

	/** Refresh and update the displayed visuals of this container */
	FORCEINLINE void RefreshContainerVisuals();

	/** Resets and nulls all container variables. Deletes any references */
	FORCEINLINE void ResetContainer();

protected:

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:

	FORCEINLINE void UpdateItemImage();

	FORCEINLINE void UpdateStackCountText();

	FORCEINLINE void SetupEmptyBorderMaterial();

	void UpdateCooldown();

	FTimerHandle CooldownTimerHandle;

	// void UpdateItemButton();


};
