// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EODLibrary.h"
#include "Blueprint/UserWidget.h"
#include "ContainerWidget.generated.h"

class UImage;
class UTextBlock;

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
	int32 StackCount;

	FContainerData()
	{
		ItemID = NAME_None;
		ItemGroup = NAME_None;
		Icon = nullptr;
		InGameName = FString("");
		Description = FString("");
		EODItemType = EEODItemType::None;
		StackCount = 1;
	}
};

/**  */
UCLASS()
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
	//	Necessary Child Widgets
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

protected:

	// --------------------------------------
	//	Handle Mouse Events
	// --------------------------------------

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// --------------------------------------
	//	Constants : Variables that aren't meant to be changed once initialized
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container Constants")
	EContainerType ContainerType;

	// --------------------------------------
	//	Container Data and Cache
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container Config")
	FContainerData ContainerData;

public:

	UFUNCTION()
	void UpdateDescription(const FString& NewDescription);


};
