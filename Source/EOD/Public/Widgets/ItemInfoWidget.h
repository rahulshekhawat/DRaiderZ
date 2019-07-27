// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemInfoWidget.generated.h"

class UButton;
class UImage;
class URichTextBlock;
class UItemInfoWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemInfoPressedMCDelegate, UItemInfoWidget*, PressedWidget);

/**
 * 
 */
UCLASS()
class EOD_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	UItemInfoWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	

	///////////////////////////////////////////////////////////////////////////
	//  Child Widgets
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ChildWidget, meta = (BindWidget))
	UButton* BackgroundButton;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ChildWidget, meta = (BindWidget))
	UImage* ItemIcon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ChildWidget, meta = (BindWidget))
	URichTextBlock* TitleText;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ChildWidget, meta = (BindWidget))
	URichTextBlock* SubText;


	///////////////////////////////////////////////////////////////////////////
	//  Behaviour
public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Events)
	FItemInfoPressedMCDelegate OnPressed;

protected:

	UFUNCTION()
	void OnBackgroundButtonPressed();

	UPROPERTY(Transient, BlueprintReadWrite, Category = Behaviour)
	bool bSelected;

	///////////////////////////////////////////////////////////////////////////
	//  Utility
public:

	UFUNCTION(BlueprintCallable, Category = Utility)
	void SetItemName(FString InValue);

	UFUNCTION(BlueprintCallable, Category = Utility)
	void SetItemSubText(FString InValue);

	UFUNCTION(BlueprintCallable, Category = Utility)
	void SetIcon(UTexture* InIcon);

	UFUNCTION(BlueprintCallable, Category = Utility)
	void SetSelected(bool bValue);

	UFUNCTION(BlueprintCallable, Category = Utility)
	bool IsSelected() const;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Defaults)
	FText DefaultItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Defaults)
	FText DefaultItemSubText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Defaults)
	UTexture* DefaultIcon;



};
