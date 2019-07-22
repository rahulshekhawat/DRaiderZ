// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillBarWidget.generated.h"

class UGameplaySkillBase;
class UPlayerSkillsComponent;
class USkillBarContainerWidget;

/**
 * 
 */
UCLASS()
class EOD_API USkillBarWidget : public UUserWidget
{
	GENERATED_BODY()

	///////////////////////////////////////////////////////////////////////////
	//  UE4 Method Overrides
public:

	USkillBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


	///////////////////////////////////////////////////////////////////////////
	//  Child Widgets
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_7;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_8;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_9;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_11;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_12;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_13;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_14;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_15;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_16;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_17;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_18;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_19;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USkillBarContainerWidget* SkillContainer_20;


	///////////////////////////////////////////////////////////////////////////
	//
public:

	void InitializeSkillBarLayout(const TMap<uint8, uint8>& SkillBarMap, const TMap<uint8, UGameplaySkillBase*>& SkillsMap);

	void SetOwnerSkillsComponent(UPlayerSkillsComponent* SkillsComponent);

	inline UPlayerSkillsComponent* GetOwnerSkillsComponent() const;

	/** Returns container at given skill bar index */
	inline USkillBarContainerWidget* GetContainerAtIndex(const int32 Index) const;

protected:

	UPROPERTY(Transient)
	TWeakObjectPtr<UPlayerSkillsComponent> OwnerSkillsComponent;

};

inline UPlayerSkillsComponent* USkillBarWidget::GetOwnerSkillsComponent() const
{
	return OwnerSkillsComponent.Get();
}

inline USkillBarContainerWidget* USkillBarWidget::GetContainerAtIndex(const int32 Index) const
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
