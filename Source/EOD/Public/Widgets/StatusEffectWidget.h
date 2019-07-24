// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusEffectWidget.generated.h"

/**
 * 
 */
UCLASS()
class EOD_API UStatusEffectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UStatusEffectWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	///////////////////////////////////////////////////////////////////////////
	//  Child Widgets
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UImage* StatusEffectIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UBorder* RootBorder;


	///////////////////////////////////////////////////////////////////////////
	//  Utility
public:

	UFUNCTION(BlueprintCallable, Category = Utility)
	void SetDuration(float InValue);

	UFUNCTION(BlueprintCallable, Category = Utility)
	void SetIcon(class UTexture* InIcon);

protected:

	UPROPERTY(Transient)
	class UMaterialInstanceDynamic* RootMID;

	UPROPERTY(Transient)
	float Duration;

};
