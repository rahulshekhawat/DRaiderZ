// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActorBase.generated.h"

class USkeletalMesh;
class AEODCharacterBase;
class UWeaponStatsComponent;


/**
 * An abstract base actor class that is responsible for managing and attaching weapon meshes to a character
 */
UCLASS()
class EOD_API AWeaponActorBase : public AActor
{
	GENERATED_BODY()
	
public:	

	// --------------------------------------
	//  UE4 Method Overrides
	// --------------------------------------

	AWeaponActorBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// --------------------------------------
	//  Weapon Interface
	// --------------------------------------

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon Interface")
	// void Equip(AEODCharacterBase* EquippingCharacter)
	// virtual void Equip_Implementation(AEODCharacterBase* EquippingCharacter)



	// --------------------------------------
	//  Components
	// --------------------------------------

	static const FName WeaponStatsComponentName;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	UWeaponStatsComponent* WeaponStats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	USkeletalMesh* WeaponMesh;



};
