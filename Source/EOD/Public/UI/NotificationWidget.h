// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class EOD_API UNotificationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	UNotificationWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//  Child Widgets
	// --------------------------------------

	/** The image used as icon for notification */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UImage* NotificationIcon;

	/** The notification text */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* NotificationText;

	// --------------------------------------
	//  Utility
	// --------------------------------------

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Effects")
	void DeleteNotification();
	virtual void DeleteNotification_Implementation();

};
