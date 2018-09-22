// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EODItemContainer.generated.h"

UENUM(BlueprintType)
enum class EEODItemType : uint8
{
	None,
	ActiveSkill,
	PassiveSkill,
	Armor,
	Weapon,
	Necklace,
	Belt,
	Ring,
	Earring,
	Ingredient,
	Consumable,
	QuestItem,
	Potion,
	Scrap,
};

class UTextBlock;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EODItemInfo)
	bool bCanBeClicked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EODItemInfo)
	bool bCanBeDragged;

	UPROPERTY(Transient, BlueprintReadWrite, Category = EODItemInfo)
	bool bInCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EODItemInfo)
	FName EODItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EODItemInfo)
	EEODItemType EODItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EODItemInfo)
	UTexture* EODItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_StackCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_Cooldown;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartCooldown(float Duration);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StopCooldown();
	
};
