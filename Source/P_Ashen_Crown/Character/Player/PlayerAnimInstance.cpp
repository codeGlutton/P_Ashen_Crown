// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "P_Ashen_Crown/GAS/GameplayAbility/GA_Skill01.h"
#include "TestAttri/TestStateComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
}


void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	mCharacter = Cast<ACharacter>(GetOwningActor());

	if (mCharacter)
	{
		mMovementComponent = mCharacter->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (mCharacter == nullptr)
		return;

	if (mMovementComponent == nullptr)
		return;

	mVelocity = mMovementComponent->Velocity;
	mGroundSpeed = mVelocity.Size2D();

	bShouldMove = mGroundSpeed > 3.f && mMovementComponent->GetCurrentAcceleration() != FVector::ZeroVector;

	bIsFalling = mMovementComponent->IsFalling();
}

void UPlayerAnimInstance::PlayAttack()
{
	if (!IsValid(mAttackMontage))
		return;

	Montage_Play(mAttackMontage);
}

void UPlayerAnimInstance::PlayRoll()
{
	// if (!IsValid(mRollingMontage))
	// 	return;
	//
	// Montage_Play(mRollingMontage);
}

void UPlayerAnimInstance::PlaySkill()
{
	// if (!IsValid(mSkillMontage))
	// 	return;
	//
	// Montage_Play(mSkillMontage);
}

void UPlayerAnimInstance::PlayDeath()
{
	bIsDeath = true;
}

void UPlayerAnimInstance::AnimNotify_SkillStart()
{
	AActor* Owner = TryGetPawnOwner();
	APlayerCharacter* Player = Cast<APlayerCharacter>(Owner);
	if (Player)
	{
		Player->mASC->TryActivateAbilityByClass(UGA_Skill01::StaticClass());
	}
}

void UPlayerAnimInstance::AnimNotify_EndRoll()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningActor()))
	{
		PlayerCharacter->GetStateComponent()->ToggleMovementInput(true);
	}
}

void UPlayerAnimInstance::SetHitReset(bool IsHit)
{
	HitX = 0.f;
	HitY = 0.f;
	bIsHit = IsHit;
}