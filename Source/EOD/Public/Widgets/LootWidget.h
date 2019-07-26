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
	//  Behaviour
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Behaviour)
	void PickAllLoot();
	virtual void PickAllLoot_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Behaviour)
	void AddItemList(const TArray<FGeneratedLootInfo>& LootInfoArray);
	virtual void AddItemList_Implementation(const TArray<FGeneratedLootInfo>& LootInfoArray);

	inline void SetLootSource(UObject* InLootSource) { LootSource = InLootSource; }

	inline UObject* GetLootSource() const { return LootSource.Get(); }

protected:

	TWeakObjectPtr<UObject> LootSource;
	

};
