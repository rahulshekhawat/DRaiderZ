// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LootableInterface.h"
#include "Blueprint/UserWidget.h"
#include "LootWidget.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API ULootWidget : public UUserWidget
{
	GENERATED_BODY()

	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	ULootWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


	///////////////////////////////////////////////////////////////////////////
	//  Child Widgets
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ChildWidget, meta = (BindWidget))
	class UScrollBox* ItemsList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ChildWidget, meta = (BindWidget))
	class UButton* AcquireButton;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ChildWidget, meta = (BindWidget))
	class UButton* AcquireAllButton;


	///////////////////////////////////////////////////////////////////////////
	//  Utility
public:
	
	UFUNCTION(BlueprintCallable, Category = Utility)
	bool InitializeWidget(UObject* InObj);

	UFUNCTION(BlueprintCallable, Category = Utility)
	void DestroyWidget();

	UFUNCTION(BlueprintCallable, Category = Utility)
	void GenerateItemList(const TArray<FGeneratedLootInfo>& LootInfoArray);

	UFUNCTION(BlueprintCallable, Category = Utility)
	void SelectFirstLootItem();

	UFUNCTION(BlueprintCallable, Category = Utility)
	void SelectLootItem(class UItemInfoWidget* InWidget);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Classes)
	TSubclassOf<class UItemInfoWidget> ItemInfoWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	USoundBase* UIUpSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	USoundBase* UIDownSound;

	UPROPERTY(Transient)
	TMap<UItemInfoWidget*, FGeneratedLootInfo> LootItemsMap;


	///////////////////////////////////////////////////////////////////////////
	//  Behaviour
public:

	UFUNCTION(BlueprintCallable, Category = Behaviour)
	void PickAllLoot();

	UFUNCTION(BlueprintCallable, Category = Behaviour)
	void AcquireLootFromWidget(UItemInfoWidget* InWidget);

	UFUNCTION(BlueprintCallable, Category = Utility, meta = (DisplayName = "Set Loot Source"))
	void BP_SetLootSource(UObject* InLootSource);

	UFUNCTION(BlueprintPure, Category = Utility, meta = (DisplayName = "Get Loot Source"))
	UObject* BP_GetLootSource() const;

	inline void SetLootSource(UObject* InLootSource) { LootSource = InLootSource; }

	inline UObject* GetLootSource() const { return LootSource.Get(); }

protected:

	UPROPERTY(Transient)
	TWeakObjectPtr<UObject> LootSource;
	
	UFUNCTION()
	void OnLootItemPressed(class UItemInfoWidget* PressedWidget);

	UFUNCTION()
	void OnAcquireButtonClicked();
	
	UFUNCTION()
	void OnAcquireAllButtonClicked();

};
