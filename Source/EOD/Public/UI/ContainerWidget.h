// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODLibrary.h"
#include "Blueprint/UserWidget.h"
#include "ContainerWidget.generated.h"

class UImage;
class UTextBlock;
class UMaterialInstanceDynamic;
class UTooltipWidget;
class UContainerWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnContainerClickedMCDelegate, UContainerWidget*, ClickedContainer, UUserWidget*, ParentWidget);

/** Determines the type of container widget */
UENUM(BlueprintType)
enum class EContainerType : uint8
{
	None,			// This container has not yet been initialized
	Inventory,		// This container belongs to an inventory widget
	SkillTree,		// This container belongs to a skill tree widget
	SkillBar,		// This container belongs to a skill bar 

	//~ @todo Add character stats widget
};


USTRUCT(BlueprintType)
struct EOD_API FContainerData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InGameName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEODItemType EODItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxValue;

	FString ToString() const
	{
		FString Str = FString("ItemID: ") + ItemID.ToString() + FString(", ItemGroup: ") + ItemGroup.ToString() +
			FString(", InGameName: ") + InGameName + FString(", CurrentValue: ") + FString::FromInt(CurrentValue) +
			FString(", MaxValue: ") + FString::FromInt(MaxValue);
		return Str;
	}

	FContainerData() :
		ItemID(NAME_None),
		ItemGroup(NAME_None),
		Icon(nullptr),
		InGameName(""),
		Description(""),
		EODItemType(EEODItemType::None),
		CurrentValue(0),
		MaxValue(1)
	{
	}

	bool operator==(const FContainerData& Other)
	{
		return this->ItemID == Other.ItemID &&
			this->ItemGroup == Other.ItemGroup &&
			this->Icon == Other.Icon &&
			this->InGameName == Other.InGameName &&
			this->Description == Other.Description &&
			this->EODItemType == Other.EODItemType &&
			this->CurrentValue == Other.CurrentValue &&
			this->MaxValue == Other.MaxValue;
	}
};

/**  */
UCLASS(meta = (DisableNativeTick))
class EOD_API UContainerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	UContainerWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	// --------------------------------------
	//	Child Widgets
	// --------------------------------------

	/** The border around the icon */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UImage* BorderImage;

	/** The icon of the item placed in this container */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UImage* ItemImage;

	/** SubText displays the number of items placed in the container */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* SubText;

	/** Displays the remaining cooldown */
	UPROPERTY(BlueprintReadOnly, Category = "Container Child", meta = (BindWidget))
	UTextBlock* CooldownText;

	// --------------------------------------
	//	Container Initialization
	// --------------------------------------

	/**
	 * If this container widget has been created dynamically, call this method to initialize some important variables like ContainerType, etc.
	 * @note Make sure ParentWidget is not NULL
	 */
	void InitializeWithParent(UUserWidget* ParentWidget);

	/** Returns true if this container is valid */
	inline bool IsContainerValid() const { return ContainerType != EContainerType::None; }

	/** Returns true if this container is initialized */
	inline bool IsContainerInitialized() const { return IsContainerValid(); }

private:

	/** Internal method (private) to initialie and do some initial setup on child widgets */
	void Internal_InitializeContainer();

public:

	// --------------------------------------
	//	Handle Mouse Events
	// --------------------------------------

	FOnContainerClickedMCDelegate OnClicked;

protected:

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:

	// --------------------------------------
	//	Constants : Variables that aren't meant to be changed once initialized
	// --------------------------------------

	FORCEINLINE EContainerType GetContainerType() const { return ContainerType; }

	FORCEINLINE UUserWidget* GetContainerParent() const { return ContainerParentWidget; }

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Container Constants")
	UMaterialInstanceDynamic* EmptyBorderMID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container Constants")
	EContainerType ContainerType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container Constants")
	FLinearColor NormalBorderColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container Constants")
	FLinearColor HoveredBorderColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container Constants")
	FLinearColor PressedBorderColor;

	/** Determines whether the sub text should be displayed in the form of 'current_value / max_value' ratio, or just as 'current_value' */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container Behaviour")
	bool bDisplaySubTextAsRatio;

	/** Determines whether the SubText should be displayed to player at all */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container Behaviour")
	bool bSubTextVisible;

	/** Parent widget of this container */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Container Behaviour")
	UUserWidget* ContainerParentWidget;

	// --------------------------------------
	//	Container Data and Cache
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container Config")
	FContainerData ContainerData;

public:

	FORCEINLINE FContainerData GetContainerData() const { return ContainerData; }

	// --------------------------------------
	//	Container Behaviour
	// --------------------------------------

	/** Sets the current enabled status of this container */
	virtual void SetIsEnabled(bool bInIsEnabled) override;

	/** Sets whether this container can be clicked */
	void SetCanBeClicked(bool bValue);

	/** Sets whether this container can be dragged */
	void SetCanBeDragged(bool bValue);

protected:

	/** Determines whether this container can be clicked */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Container Behaviour")
	uint32 bCanBeClicked : 1;

	/** Determines whether this container can be dragged */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Container Behaviour")
	uint32 bCanBeDragged : 1;

	UPROPERTY(Transient)
	bool bIsInCooldown;

	float CooldownRemaining;

	void EnableCooldownText();

	void DisableCooldownText();

public:

	// --------------------------------------
	//	Updating Container and it's Child Widgets
	// --------------------------------------

	UFUNCTION()
	void SetItemID(FName NewID);

	UFUNCTION()
	void SetItemGroup(FName NewItemGroup);

	/** Update the display icon of this container */
	UFUNCTION()
	void SetIcon(UTexture* NewIcon);

	/** Update the description for this container */
	UFUNCTION()
	void SetInGameName(const FString& NewInGameName);

	/** Update the description for this container */
	UFUNCTION()
	void SetDescription(const FString& NewDescription);

	/** Set the type of item placed in this container */
	UFUNCTION()
	void SetItemType(EEODItemType EODItemType);

	UFUNCTION()
	void SetCurrentValue(int32 NewValue);

	UFUNCTION()
	void SetMaxValue(int32 NewValue);

	UFUNCTION()
	void SetContainerData(const FContainerData& NewData);

	UFUNCTION()
	void UpdateCooldown(float CooldownTimeRemaining);

	inline void ResetContainer();

};

inline void UContainerWidget::ResetContainer()
{
	FContainerData NewData;
	SetContainerData(NewData);
}
