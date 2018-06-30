// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "PrimaryWeapon.generated.h"

class USkeletalMeshComponent;

UENUM(BlueprintType)
enum class EPrimaryWeaponType : uint8
{
	SingleHanded,
	DualHanded
};

/**
 * 
 */
UCLASS()
class EOD_API APrimaryWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APrimaryWeapon(const FObjectInitializer& ObjectInitializer);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnEquip(FWeaponData* NewWeaponData) override;

	virtual void OnUnEquip() override;
	
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	EWeaponType PrimaryWeaponType;
	
private:

	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* LeftHandedMeshComp;
		
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* RightHandedMeshComp;
		
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* FallenMeshComp;
	
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* SheathedMeshComp;

	
};
