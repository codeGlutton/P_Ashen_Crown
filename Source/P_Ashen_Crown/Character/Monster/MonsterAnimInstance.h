// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameCore.h"
#include "MonsterInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

DECLARE_DELEGATE_OneParam(FOnHitEndDelegate, AActor* /*Attacker*/);

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMonsterAnimInstance();

public:
	FOnHitEndDelegate OnHitEnd;

	AActor* mAttacker = nullptr;

protected:
	// 현재 이동 속도
	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	float mMoveSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	float	mAimWidth = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	float	mAimHeight = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	TObjectPtr<class AMonsterBase> mOwnerMonster;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	TMap<EMonsterAnim, TObjectPtr<UAnimMontage>> mMontageMap;

	EMonsterAnim mAnimType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UAnimSequence>	mHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float		mHitAlpha = 0.f;

	bool bIsPlayingHit = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EHitDirection mHitDirection;

	FTimerHandle mHitAlphaHandle;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void PlayMontageByType(EMonsterAnim Type);

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetMontageByType(EMonsterAnim Type);

	void SetAimInfo(float Width, float Height)
	{
		mAimWidth = Width;
		mAimHeight = Height;
	}

	UFUNCTION(BlueprintCallable)
	void SetHitDirection(EHitDirection Dir)
	{
		mHitDirection = Dir;
	}

public:
	// mMontageMap에서 원하는 몽타주로 변경 해주는 함수
	void ChangeAnim(EMonsterAnim Anim);
	
	// Hit 모션 한번만 재생하게 하고 mHitAlpha 값 설정
	UFUNCTION()
	virtual void PlayHit(AActor* Attacker);

	void ReduceHitAlpha();

	// 공격 시점
	UFUNCTION()
	virtual void AnimNotify_AttackHit();

	// 몽타주 재생 종료 시 호출될 함수
	UFUNCTION()
	virtual void AnimNotify_AttackEnd();

	// 죽은 시점
	UFUNCTION()
	virtual void AnimNotify_Death();

	// Hit 끝나는 시점
	UFUNCTION()
	virtual void AnimNotify_HitEnd();

};
