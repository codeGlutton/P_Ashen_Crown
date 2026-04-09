// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameCore.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class P_ASHEN_CROWN_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References")
	TObjectPtr<ACharacter>	mCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References")
	TObjectPtr<UCharacterMovementComponent>	mMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UAnimMontage>		mAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rolling")
	TObjectPtr<UAnimMontage>		mRollingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UAnimMontage>		mSkillMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	TObjectPtr<UAnimMontage>		mHealMontage;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement data")
	FVector mVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement data")
	float mGroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement data")
	bool bShouldMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement data")
	bool bIsFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State Data")
	bool bIsDeath = false;
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HitReaction")
	float HitX = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HitReaction")
	float HitY = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HitReaction")
	bool bIsHit = false;

	void SetHitReset(bool IsHit);

public:
	UPlayerAnimInstance();

protected:
	/// <summary>
	/// 애니메이션을 초기화하는 가상 함수입니다
	/// </summary>
	virtual void NativeInitializeAnimation();

	/// <summary>
	/// 애니메이션을 프레임마다 갱신합니다.
	/// </summary>
	/// <param name="DeltaSeconds"> 이전 프레임 이후 경과한 시간(초)입니다 </param>
	virtual void NativeUpdateAnimation(float DeltaSeconds);

public:
	void PlayAttack();
	void PlayRoll();
	void PlaySkill();
	void PlayDeath();

public:
	UFUNCTION()
	void AnimNotify_EndRoll();
	
	UFUNCTION()
	void AnimNotify_SkillStart();
};
