// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "MonsterBase.h"
#include "./BossMonster/BossLich.h"
#include "MonsterAIController.h"

UMonsterAnimInstance::UMonsterAnimInstance()
{
	mMoveSpeed = 0.f;
	mOwnerMonster = nullptr;
}

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 소유자 캐릭터 캐스팅
	APawn* Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		mOwnerMonster = Cast<AMonsterBase>(Pawn);
	}
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(mOwnerMonster))
		return;

	if (mOwnerMonster)
	{
		// 현재 속도 저장
		mMoveSpeed = mOwnerMonster->GetVelocity().Size();
	} 

	//UE_LOG(LogTemp, Warning, TEXT("Tick: mHitAlpha: %f, bIsPlayingHit: %s"), mHitAlpha, bIsPlayingHit ? TEXT("true") : TEXT("false"));
}

void UMonsterAnimInstance::PlayMontageByType(EMonsterAnim Type)
{
	if (mMontageMap.Contains(Type) && mMontageMap[Type])
	{
		Montage_Play(mMontageMap[Type]);
	}
}

UAnimMontage* UMonsterAnimInstance::GetMontageByType(EMonsterAnim Type)
{
	if (mMontageMap.Contains(Type))
		return mMontageMap[Type];

	return nullptr;
}

void UMonsterAnimInstance::ChangeAnim(EMonsterAnim Anim)
{
	mAnimType = Anim;

	TObjectPtr<UAnimMontage>* FoundMontage = mMontageMap.Find(Anim);

	if (FoundMontage)
	{
		if (IsValid(*FoundMontage))
		{
			// 현재와 동일한 몽타주면 재생 생략 가능
			if (GetCurrentActiveMontage() != *FoundMontage)
			{
				Montage_Play(*FoundMontage);
			}
		}
	}
}

void UMonsterAnimInstance::PlayHit(AActor* Attacker)
{
	// 이미 재생 중이면 무시
	if (bIsPlayingHit)
		return;

	mHitAlpha = 1.0f;
	bIsPlayingHit = true;
	mAttacker = Attacker;

	// 타이머로 종료 제어
	GetWorld()->GetTimerManager().ClearTimer(mHitAlphaHandle);
	GetWorld()->GetTimerManager().SetTimer(mHitAlphaHandle, this, &UMonsterAnimInstance::ReduceHitAlpha, 0.02f, true);

	UE_LOG(LogTemp, Warning, TEXT("[AnimInstance] PlayHitReaction: Alpha = %f"), mHitAlpha);
	UE_LOG(LogTemp, Warning, TEXT("[PlayHit] Direction: %s, Alpha: %f"), *UEnum::GetValueAsString(mHitDirection), mHitAlpha);
}

void UMonsterAnimInstance::ReduceHitAlpha()
{
	mHitAlpha -= 0.05f;

	if (mHitAlpha <= 0.f)
	{
		mHitAlpha = 0.f;
		bIsPlayingHit = false;

		GetWorld()->GetTimerManager().ClearTimer(mHitAlphaHandle);

		UE_LOG(LogTemp, Warning, TEXT(" [HitEnd] HitAlpha: %f, bIsPlayingHit: %s"), 
			mHitAlpha, bIsPlayingHit ? TEXT("true") : TEXT("false"));

		// 회전 델리게이트 호출
		if (OnHitEnd.IsBound())
		{
			OnHitEnd.Execute(mAttacker);
		}

	}
}

void UMonsterAnimInstance::AnimNotify_AttackHit()
{
	if (IsValid(mOwnerMonster))
	{
		mOwnerMonster->NormalAttack();
	}
}

void UMonsterAnimInstance::AnimNotify_AttackEnd()
{

	if (!IsValid(mOwnerMonster)) 
		return;

	AAIController* AIController = Cast<AAIController>(mOwnerMonster->GetController());
	if (!IsValid(AIController))
		return;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();

	//UE_LOG(LogTemp, Warning, TEXT("Notify Blackboard Ptr: %p"), Blackboard);

	if (IsValid(Blackboard))
	{

		Blackboard->SetValueAsBool(MonsterAIKey::AttackEnd, true);
		UE_LOG(LogTemp, Warning, TEXT("AttackEnd = true 설정됨"));
	}
}

void UMonsterAnimInstance::AnimNotify_Death()
{
	AMonsterBase* Monster = Cast<AMonsterBase>(TryGetPawnOwner());
	if (IsValid(Monster))
	{
		Monster->Destroy();
		//Monster->RagDoll();
	}
}

void UMonsterAnimInstance::AnimNotify_HitEnd()
{
	mHitAlpha = 0.f;
}
