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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Behavior)
	bool bCanBeClicked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Behavior)
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_StackCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_Cooldown;

	UFUNCTION(BlueprintCallable, Category = EODItemContainer)
	void StartCooldown(float Duration, float Interval = 1.f);

	UFUNCTION(BlueprintCallable, Category = EODItemContainer)
	void StopCooldown();

	void RefreshContainerVisuals();

protected:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:

	FTimerHandle CooldownTimerHandle;

	void UpdateItemImage();
	
	// void UpdateItemButton();

	void UpdateCooldown();

};
