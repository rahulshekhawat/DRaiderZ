// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DynamicSkillBarWidget.generated.h"

class UContainerWidget;

/**
 * 
 */
UCLASS()
class EOD_API UDynamicSkillBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// --------------------------------------
	//	UE4 Method Overrides
	// --------------------------------------

	UDynamicSkillBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

protected:

	// --------------------------------------
	//	Child Widgets
	// --------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_7;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_8;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_9;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_11;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_12;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_13;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_14;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_15;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_16;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_17;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_18;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_19;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UContainerWidget* SkillContainer_20;

public:

	// --------------------------------------
	//	Update Skill Bar
	// --------------------------------------

	/**
	 * Called when a container from a different parent widget is dropped on one of the children containers of skill bar
	 * @note This usually means a skill was dropped on the skill bar
	 */
	bool OnContainerDropped(UContainerWidget* FromContainer, UContainerWidget* ToChildContainer);

	/**
	 * Called when a child container is swapped with another child container on this skill bar
	 * @note This means two skills swapped positions on the skill bar
	 */
	bool OnContainersSwapped(UContainerWidget* Container1, UContainerWidget* Container2);

	/**
	 * Called when a child container is removed from this skill bar
	 * @note This means a skill was removed from skill bar
	 */
	bool OnContainerRemoved(UContainerWidget* Container);

	/** Returns container at given skill bar index */
	inline UContainerWidget* GetContainerAtIndex(const int32 Index) const;

public:

	/** Returns true if skill at given skill index is in cooldown */
	// inline bool IsSkillInCooldown(const int32 SkillIndex) const;

	/** Returns true the given skill container exists on skill bar */
	// inline bool HasSkillContainer(UContainerWidget* Container) const;

	/** Returns true if the given skill group is in cooldown */
	// bool IsSkillGroupInCooldown(const FString& SkillGroup) const;

	/** Put skill at given skill index on cooldown */
	// void PutSkillOnCooldownTimer(int32 SkillIndex, float Duration, float Interval);

	// void PutSkillOnCooldownTimer(FString SkillGroup, float Duration, float Interval);

	/** Save current skill bar layout to the current save slot */
	// void SaveSkillBarLayout();

	/** Called when a new skill is dropped on skill bar from skill tree */
	// bool OnNewSkillDropped(UContainerWidget* FromContainer, UContainerWidget* ToContainer);

	/*
	void UpdateSkillBarLayout(TMap<int32, FString>& NewLayout, bool bResize = false);

private:
	inline void InitializeSkillContainer(UContainerWidget* Container, const int32 ContainerIndex);
	*/


	/** Returns the skill index of given container. Returns -1 if the container couldn't be found. */
	// inline int32 GetIndexOfSkillContainer(UContainerWidget* Container) const;

	/** Get the owning EOD player of this widget */
	// FORCEINLINE APlayerCharacter* GetOwningEODPlayer() const;

private:

	void InitializeContainersParent();


};

inline UContainerWidget* UDynamicSkillBarWidget::GetContainerAtIndex(const int32 Index) const
{
	switch (Index)
	{
	case 1:
		return SkillContainer_1;
	case 2:
		return SkillContainer_2;
	case 3:
		return SkillContainer_3;
	case 4:
		return SkillContainer_4;
	case 5:
		return SkillContainer_5;
	case 6:
		return SkillContainer_6;
	case 7:
		return SkillContainer_7;
	case 8:
		return SkillContainer_8;
	case 9:
		return SkillContainer_9;
	case 10:
		return SkillContainer_10;
	case 11:
		return SkillContainer_11;
	case 12:
		return SkillContainer_12;
	case 13:
		return SkillContainer_13;
	case 14:
		return SkillContainer_14;
	case 15:
		return SkillContainer_15;
	case 16:
		return SkillContainer_16;
	case 17:
		return SkillContainer_17;
	case 18:
		return SkillContainer_18;
	case 19:
		return SkillContainer_19;
	case 20:
		return SkillContainer_20;
	default:
		break;
	}

	return nullptr;
}
