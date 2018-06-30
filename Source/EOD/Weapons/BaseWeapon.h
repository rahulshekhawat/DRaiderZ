// Copyright 2018 Moikkai Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statics/WeaponLibrary.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class ABaseCharacter;

UCLASS(Abstract)
class EOD_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnEquip(FWeaponData* NewWeaponData) PURE_VIRTUAL(ABaseWeapon::OnEquip, );

	virtual void OnUnEquip() PURE_VIRTUAL(ABaseWeapon::OnUnEquip, );
	
	UPROPERTY(EditDefaultsOnly, Category = BaseInfo)
	EWeaponType WeaponType;

	void SetOwningCharacter(ABaseCharacter* NewCharacter);

protected:
	
	//~ @todo network replication
	ABaseCharacter* OwningCharacter;

private:

	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWeaponStatsComponent* StatsComp;


};
