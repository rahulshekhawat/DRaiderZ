// Copyright 2018 Moikkai Games. All Rights Reserved.

#include "EODCharacterBase.h"
#include "CharAnimInstance.h"
#include "Core/EODPreprocessors.h"
#include "Components/StatsComponentBase.h"
#include "Statics/EODBlueprintFunctionLibrary.h"

#include "UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AEODCharacterBase::AEODCharacterBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Stats Component
	StatsComp = ObjectInitializer.CreateDefaultSubobject<UStatsComponentBase>(this, FName("Character Stats Component"));
	
	// Initialize variables
	CharacterState = ECharacterState::IdleWalkRun;

	CurrentActiveSkill = nullptr;
	bGodMode = false;
}

void AEODCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEODCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEODCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AEODCharacterBase, CharacterState, COND_SkipOwner);

}

bool AEODCharacterBase::IsAlive() const
{
	return StatsComp->GetCurrentHealth() > 0;
}

bool AEODCharacterBase::IsDead() const
{
	return StatsComp->GetCurrentHealth() <= 0;
}

bool AEODCharacterBase::IsIdle() const
{
	return (CharacterState == ECharacterState::IdleWalkRun && GetVelocity().Size() == 0);
}

bool AEODCharacterBase::IsMoving() const
{
	return (CharacterState == ECharacterState::IdleWalkRun && GetVelocity().Size() != 0);
}

bool AEODCharacterBase::IsIdleOrMoving() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool AEODCharacterBase::IsJumping() const
{
	return CharacterState == ECharacterState::Jumping;
}

bool AEODCharacterBase::IsBlocking() const
{
	return CharacterState == ECharacterState::Blocking;
}

bool AEODCharacterBase::IsBlockingDamage() const
{
	return false;
}

bool AEODCharacterBase::IsCastingSpell() const
{
	return CharacterState == ECharacterState::CastingSpell;
}

bool AEODCharacterBase::IsNormalAttacking() const
{
	return CharacterState == ECharacterState::Attacking && CurrentActiveSkill != nullptr;
}

bool AEODCharacterBase::IsUsingAnySkill() const
{
	return CharacterState == ECharacterState::UsingActiveSkill && CurrentActiveSkill != nullptr;
}

bool AEODCharacterBase::IsUsingSkill(int32 SkillIndex) const
{
	return IsUsingAnySkill() && CurrentActiveSkill == GetSkill(SkillIndex);
}

bool AEODCharacterBase::IsCriticalHit(const FSkill * HitSkill) const
{
	bool bCriticalHit = false;

	if (HitSkill)
	{
		switch (HitSkill->DamageType)
		{
		case EDamageType::Physical:
			bCriticalHit = StatsComp->GetPhysicalCritRate() >= FMath::RandRange(0.f, 100.f) ? true : false;
			break;
		case EDamageType::Magickal:
			bCriticalHit = StatsComp->GetMagickCritRate() >= FMath::RandRange(0.f, 100.f) ? true : false;
			break;
		default:
			break;
		}
	}

	return bCriticalHit;
}

bool AEODCharacterBase::IsDodging() const
{
	return CharacterState == ECharacterState::Dodging;
}

bool AEODCharacterBase::IsDodgingDamage() const
{
	return bHasActiveiFrames;
}

bool AEODCharacterBase::NeedsHeal() const
{
	return StatsComp->IsLowOnHealth();
}

bool AEODCharacterBase::IsHealing() const
{
	return false;
}

void AEODCharacterBase::ApplyStatusEffect(const UStatusEffectBase * StatusEffect)
{
	// @todo definition
}

void AEODCharacterBase::RemoveStatusEffect(const UStatusEffectBase * StatusEffect)
{
	// @todo definition
}

void AEODCharacterBase::OnMeleeCollision(UAnimSequenceBase* Animation, TArray<FHitResult>& HitResults, bool bHit)
{
	FSkill* ActiveSkill = GetCurrentActiveSkill();
	check(ActiveSkill); // Make sure ActiveSkil is not a nullptr

	TArray<TWeakObjectPtr<AEODCharacterBase>> CharactersSuccessfullyHit;
	TArray<TWeakObjectPtr<AEODCharacterBase>> CharactersHitWithCriticalDamage;

	for (FHitResult& HitResult : HitResults)
	{
		if (!HitResult.Actor.Get())
		{
			continue;
		}

		// @todo handle damage for non AEODCharacterBase actors
		AEODCharacterBase* HitCharacter = Cast<AEODCharacterBase>(HitResult.Actor.Get());
		if (!HitCharacter)
		{
			continue;
		}

		// If the skill is dodgable and the hit character is currently dodging damage
		if (HitCharacter->IsDodgingDamage() && !ActiveSkill->SkillLevelUpInfo.bUndodgable)
		{
			HitCharacter->OnSuccessfulDodge.Broadcast(TArray<TWeakObjectPtr<AEODCharacterBase>>());
			continue;
		}

		TArray<FHitResult> LineHitResults;
		FVector LineStart = GetActorLocation();
		FVector LineEnd = FVector(HitCharacter->GetActorLocation().X, HitCharacter->GetActorLocation().Y, LineStart.Z);

		FCollisionQueryParams Params = UCombatLibrary::GenerateCombatCollisionQueryParams(this);
		GetWorld()->LineTraceMultiByChannel(LineHitResults, LineStart, LineEnd, COLLISION_COMBAT, Params);

		FHitResult LineHitResultToHitCharacter;
		bool bLineHitResultFound = false;

		for (FHitResult& LineHitResult : LineHitResults)
		{
			if (LineHitResult.Actor.Get() && LineHitResult.Actor.Get() == HitCharacter)
			{
				LineHitResultToHitCharacter = LineHitResult;
				bLineHitResultFound = true;
				break;
			}
		}

#if DEVSTAGE_CODE_ENABLED
		if (bLineHitResultFound)
		{
			FVector Start = LineHitResultToHitCharacter.ImpactPoint;
			FVector End = LineHitResultToHitCharacter.ImpactPoint + LineHitResultToHitCharacter.ImpactNormal * 50;
			UKismetSystemLibrary::DrawDebugArrow(this, Start, End, 200, FLinearColor::Blue, 5.f, 2.f);
		}
#endif

		TWeakObjectPtr<AEODCharacterBase> HitCharacterWeakPtr(HitCharacter);
		CharactersSuccessfullyHit.Add(HitCharacterWeakPtr);

		bool bCriticalHit = IsCriticalHit(ActiveSkill);
		if (bCriticalHit)
		{
			CharactersHitWithCriticalDamage.Add(HitCharacterWeakPtr);
		}

		FEODDamage EODDamage;
		EODDamage.Instigator = this;
		EODDamage.CollisionHitResult = HitResult;
		EODDamage.LineHitResult = LineHitResultToHitCharacter;
		EODDamage.bCriticalHit = bCriticalHit;

		FEODDamageResult EODDamageResult = HitCharacter->ApplyEODDamage(EODDamage);
	}

	if (CharactersSuccessfullyHit.Num() == 0)
	{
		OnUnsuccessfulHit.Broadcast(TArray<TWeakObjectPtr<AEODCharacterBase>>());
	}
	else
	{
		OnSuccessfulHit.Broadcast(CharactersSuccessfullyHit);

		switch (ActiveSkill->DamageType)
		{
		case EDamageType::Physical:
			OnSuccessfulPhysicalAttack.Broadcast(CharactersSuccessfullyHit);
			break;
		case EDamageType::Magickal:
			OnSuccessfulMagickAttack.Broadcast(CharactersSuccessfullyHit);
			break;
		default:
			break;
		}

		if (CharactersHitWithCriticalDamage.Num() > 0)
		{
			OnCriticalHit.Broadcast(CharactersHitWithCriticalDamage);
		}
	}
}

FEODDamageResult AEODCharacterBase::ApplyEODDamage(FEODDamage& EODDamage)
{
	//~ @todo crowd control effects

	FSkill* HitBySkill = EODDamage.Instigator->GetCurrentActiveSkill();
	AEODCharacterBase* Instigator = EODDamage.Instigator;

	bool bBlockSuccessful = false;
	float BlockedDamageModifier = 1.f;
	if (IsBlockingDamage())
	{
		FVector MyDirection = GetActorForwardVector();
		FVector HitNormal = EODDamage.LineHitResult.ImpactNormal;
		
		float Angle = UEODBlueprintFunctionLibrary::CalculateAngleBetweenVectors(MyDirection, HitNormal);
		if (Angle < 60)
		{
			bBlockSuccessful = true;
		}
	}

	int32 DamageApplied = 0;
	if (HitBySkill->DamageType == EDamageType::Physical)
	{
		int32 PhysicalAttack = Instigator->StatsComp->GetPhysicalAttack();
		int32 MyPhysicalResistance = this->StatsComp->GetPhysicalResistance();
		int32 CritBonus = 0;
		if (EODDamage.bCriticalHit)
		{
			CritBonus = PhysicalAttack * UCombatLibrary::PhysicalCritMultiplier + Instigator->StatsComp->GetPhysicalCritBonus();
		}

		DamageApplied = UCombatLibrary::CalculateDamage(PhysicalAttack + CritBonus, MyPhysicalResistance);
		this->StatsComp->ModifyCurrentHealth(-DamageApplied);
	}
	else if (HitBySkill->DamageType == EDamageType::Magickal)
	{
		int32 MagickAttack = Instigator->StatsComp->GetMagickAttack();
		int32 MyMagickResistance = this->StatsComp->GetMagickResistance();
		int32 CritBonus = 0;
		if (EODDamage.bCriticalHit)
		{
			CritBonus = MagickAttack * UCombatLibrary::MagickalCritMultiplier + Instigator->StatsComp->GetMagickCritBonus();
		}

		DamageApplied = UCombatLibrary::CalculateDamage(MagickAttack + CritBonus, MyMagickResistance);
		this->StatsComp->ModifyCurrentHealth(-DamageApplied);
	}

	// return DamageApplied;
	return FEODDamageResult();
}

int32 AEODCharacterBase::GetMostWeightedSkillIndex() const
{
	// @todo definition
	return 0;
}

bool AEODCharacterBase::UseSkill(int32 SkillIndex)
{
	if (CanUseAnySkill())
	{
		FSkill* SkillToUse = GetSkill(SkillIndex);

		if (!SkillToUse)
		{
			// unable to use skill - return false
			return false;
		}

		// SkillToUse->AnimationMontage
		PlayAnimationMontage(SkillToUse->AnimationMontage, SkillToUse->SkillStartMontageSectionName, ECharacterState::UsingActiveSkill);
		CurrentActiveSkill = SkillToUse;
		return true;
	}

	return false;
}

EEODTaskStatus AEODCharacterBase::CheckSkillStatus(int32 SkillIndex)
{
	EEODTaskStatus TaskStatus = EEODTaskStatus::Inactive;

	FSkill* SkillToCheck = GetSkill(SkillIndex);
	if (SkillToCheck == CurrentActiveSkill)
	{
		return EEODTaskStatus::Active;
	}

	if (GetLastUsedSkill().LastUsedSkill != CurrentActiveSkill)
	{
		return EEODTaskStatus::Inactive;
	}

	if (GetLastUsedSkill().bInterrupted)
	{
		return EEODTaskStatus::Aborted;
	}
	else
	{
		return EEODTaskStatus::Finished;
	}
}

void AEODCharacterBase::ApplyStun(float Duration)
{
}

void AEODCharacterBase::EnableiFrames(float Duration)
{
	bHasActiveiFrames = true;

	if (Duration > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, this, &AEODCharacterBase::DisableiFrames, Duration, false);
	}
}

void AEODCharacterBase::DisableiFrames()
{
	bHasActiveiFrames = false;
}

void AEODCharacterBase::EnableDamageBlocking()
{
	bIsBlockingDamage = true;
}

void AEODCharacterBase::DisableDamageBlocking()
{
	bIsBlockingDamage = false;
}

FSkill * AEODCharacterBase::GetCurrentActiveSkill() const
{
	return CurrentActiveSkill;
}

FSkill * AEODCharacterBase::GetSkill(int32 SkillIndex) const
{
	if (Skills.Num() > SkillIndex)
	{
		return Skills[SkillIndex];
	}

	return nullptr;
}

FLastUsedSkillInfo& AEODCharacterBase::GetLastUsedSkill()
{
	return LastUsedSkillInfo;
}

void AEODCharacterBase::OnMontageBlendingOut(UAnimMontage * AnimMontage, bool bInterrupted)
{
}

void AEODCharacterBase::OnMontageEnded(UAnimMontage * AnimMontage, bool bInterrupted)
{
}

void AEODCharacterBase::SetCharacterState(const ECharacterState NewState)
{
	CharacterState = NewState;

	if (Role < ROLE_Authority)
	{
		Server_SetCharacterState(NewState);
	}
}

void AEODCharacterBase::Server_SetCharacterState_Implementation(ECharacterState NewState)
{
	SetCharacterState(NewState);
}

bool AEODCharacterBase::Server_SetCharacterState_Validate(ECharacterState NewState)
{
	return true;
}

void AEODCharacterBase::SetWalkSpeed(const float WalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	if (Role < ROLE_Authority)
	{
		Server_SetWalkSpeed(WalkSpeed);
	}
}

void AEODCharacterBase::SetCharacterRotation(const FRotator NewRotation)
{
	GetCharacterMovement()->FlushServerMoves();
	SetActorRotation(NewRotation);

	if (Role < ROLE_Authority)
	{
		Server_SetCharacterRotation(NewRotation);
	}
}

void AEODCharacterBase::SetUseControllerRotationYaw(const bool bNewBool)
{
	bUseControllerRotationYaw = bNewBool;

	if (Role < ROLE_Authority)
	{
		Server_SetUseControllerRotationYaw(bNewBool);
	}
}

void AEODCharacterBase::SetNextMontageSection(FName CurrentSection, FName NextSection)
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_SetNextSection(CurrentSection, NextSection);
	}

	Server_SetNextMontageSection(CurrentSection, NextSection);
}

void AEODCharacterBase::Server_SetNextMontageSection_Implementation(FName CurrentSection, FName NextSection)
{

	Multicast_SetNextMontageSection(CurrentSection, NextSection);
}

bool AEODCharacterBase::Server_SetNextMontageSection_Validate(FName CurrentSection, FName NextSection)
{
	return true;
}

void AEODCharacterBase::Server_SetUseControllerRotationYaw_Implementation(bool bNewBool)
{
	SetUseControllerRotationYaw(bNewBool);
}

bool AEODCharacterBase::Server_SetUseControllerRotationYaw_Validate(bool bNewBool)
{
	return true;
}

void AEODCharacterBase::Server_SetCharacterRotation_Implementation(FRotator NewRotation)
{
	SetCharacterRotation(NewRotation);
}

bool AEODCharacterBase::Server_SetCharacterRotation_Validate(FRotator NewRotation)
{
	return true;
}

void AEODCharacterBase::Server_SetWalkSpeed_Implementation(float WalkSpeed)
{
	SetWalkSpeed(WalkSpeed);
}

bool AEODCharacterBase::Server_SetWalkSpeed_Validate(float WalkSpeed)
{
	return true;
}

void AEODCharacterBase::Multicast_SetNextMontageSection_Implementation(FName CurrentSection, FName NextSection)
{
	FString Message = FString("Multi cast called");
	UKismetSystemLibrary::PrintString(this, Message, true, false);

	if (!IsLocallyControlled() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_SetNextSection(CurrentSection, NextSection);
	}
}

void AEODCharacterBase::PlayAnimationMontage(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay);
		CharacterState = NewState;
	}

	Server_PlayAnimationMontage(MontageToPlay, SectionToPlay, NewState);
}

void AEODCharacterBase::Server_PlayAnimationMontage_Implementation(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	MultiCast_PlayAnimationMontage(MontageToPlay, SectionToPlay, NewState);
}

bool AEODCharacterBase::Server_PlayAnimationMontage_Validate(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	return true;
}

void AEODCharacterBase::MultiCast_PlayAnimationMontage_Implementation(UAnimMontage * MontageToPlay, FName SectionToPlay, ECharacterState NewState)
{
	if (!IsLocallyControlled() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionToPlay);
		CharacterState = NewState;
	}
}

bool AEODCharacterBase::CanMove() const
{
	// Mobs can only move in IdleWalkRun state
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool AEODCharacterBase::CanJump() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool AEODCharacterBase::CanBlock() const
{
	return IsIdleOrMoving();
}

bool AEODCharacterBase::CanRespawn() const
{
	return false;
}

bool AEODCharacterBase::CanNormalAttack() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

bool AEODCharacterBase::CanBeStunned() const
{
	return false;
}

bool AEODCharacterBase::CanUseAnySkill() const
{
	return true;
}

bool AEODCharacterBase::CanUseSkill(int32 SkillIndex) const
{
	return false;
}

bool AEODCharacterBase::CanDodge() const
{
	return CharacterState == ECharacterState::IdleWalkRun;
}

EFaction AEODCharacterBase::GetFaction() const
{
	return Faction;
}

void AEODCharacterBase::Die(ECauseOfDeath CauseOfDeath, AEODCharacterBase * Instigator)
{
	if (bGodMode || IsDead())
	{
		// cannot die
		return;
	}

	if (CauseOfDeath == ECauseOfDeath::ZeroHP)
	{

	}
	else
	{
		// Set current hp to 0
		StatsComp->ModifyBaseHealth(-StatsComp->GetMaxHealth());
		SetCharacterState(ECharacterState::Dead);

		// @todo play death animation and death sound
	}
}

ECharacterState AEODCharacterBase::GetCharacterState() const
{
	return CharacterState;
}

bool AEODCharacterBase::DeltaRotateCharacterToDesiredYaw(float DesiredYaw, float DeltaTime, float Precision, float RotationRate)
{
	float CurrentYaw = GetActorRotation().Yaw;
	float YawDiff = FMath::FindDeltaAngleDegrees(CurrentYaw, DesiredYaw);
	if (FMath::Abs(YawDiff) < Precision)
	{
		return true;
	}

	float Multiplier = YawDiff / FMath::Abs(YawDiff);
	float RotateBy = Multiplier * RotationRate * DeltaTime;
	if (FMath::Abs(YawDiff) <= FMath::Abs(RotateBy))
	{
		SetCharacterRotation(FRotator(0.f, DesiredYaw, 0.f));
		return true;
	}
	else if (FMath::Abs(YawDiff) <= FMath::Abs(RotateBy) + Precision)
	{
		SetCharacterRotation(FRotator(0.f, CurrentYaw + RotateBy, 0.f));
		return true;
	}
	else
	{
		SetCharacterRotation(FRotator(0.f, CurrentYaw + RotateBy, 0.f));
		return false;
	}
}

void AEODCharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

void AEODCharacterBase::OnRep_CharacterState(ECharacterState OldState)
{
	//~ @todo : Cleanup old state
}
