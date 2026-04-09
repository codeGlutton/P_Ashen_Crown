// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonsterAnimInstance.h"
#include "../MonsterAIController.h"
#include "BossLich.h"
#include "../BossSkill/BossLichMagicFloor.h"

UBossMonsterAnimInstance::UBossMonsterAnimInstance()
{
}

void UBossMonsterAnimInstance::AnimNotify_AttackHit()
{
    Super::AnimNotify_AttackHit();
}

void UBossMonsterAnimInstance::AnimNotify_AttackEnd()
{
    Super::AnimNotify_AttackEnd();

    //UE_LOG(LogTemp, Warning, TEXT(">>> EndMagicFloor Notify 진입"));

    if (ABossLich* Boss = Cast<ABossLich>(mOwnerMonster))
    {
        
        // 장판 제거
        ABossLichMagicFloor* MagicFloor = Boss->GetMagicFloor();
        if (IsValid(MagicFloor))
        {
            MagicFloor->Destroy();
            Boss->SetMagicFloor(nullptr);
            UE_LOG(LogTemp, Warning, TEXT("EndMagicFloor - 장판 파괴"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("mOwnerMonster가 BossLich가 아님"));
    }
}

void UBossMonsterAnimInstance::AnimNotify_Skill1Fire()
{
    // 보스 몬스터가 ABossLich인 경우 스킬1 발사 처리
    if (ABossLich* Boss = Cast<ABossLich>(mOwnerMonster))
    {
        Boss->UseProjectileSkill();
        UE_LOG(LogTemp, Warning, TEXT("Use SkillStart"));
    }
}

void UBossMonsterAnimInstance::AnimNotify_SkillStart()
{
    ABossLich* Boss = Cast<ABossLich>(mOwnerMonster);

    if (!IsValid(Boss))
    {
        UE_LOG(LogTemp, Error, TEXT("mOwnerMonster가 null입니다!"));
        return;
    }

    Boss->SpawnMagicFloorActor();
    UE_LOG(LogTemp, Warning, TEXT("Spawn MagicFloor Actor"));

}

void UBossMonsterAnimInstance::AnimNotify_MagicFloor()
{
    if (ABossLich* Boss = Cast<ABossLich>(mOwnerMonster))
    {
        if (!Boss->mASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.MagicFloorSkill"))))
        {
            Boss->UseMagicFloorSkill();
            UE_LOG(LogTemp, Warning, TEXT("Use MagicFloorSkill"));
        }
    }
}

void UBossMonsterAnimInstance::AnimNotify_SkillEnd()
{
    UE_LOG(LogTemp, Warning, TEXT("<^> EndMagicFloor Notify 진입"));

    if (ABossLich* Boss = Cast<ABossLich>(mOwnerMonster))
    {
        // 장판 제거
        ABossLichMagicFloor* MagicFloor = Boss->GetMagicFloor();
        if (IsValid(MagicFloor))
        {
            MagicFloor->Destroy();
            Boss->SetMagicFloor(nullptr);
            UE_LOG(LogTemp, Warning, TEXT("EndMagicFloor - 장판 파괴"));
        }

        AAIController* AIController = Cast<AAIController>(Boss->GetController());
        if (IsValid(AIController))
        {
            UE_LOG(LogTemp, Warning, TEXT("AIController 유효함"));

            UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
            if (IsValid(Blackboard))
            {
                Blackboard->SetValueAsBool(MonsterAIKey::SkillEnd, true);
                UE_LOG(LogTemp, Warning, TEXT("SkillEnd = true 설정됨"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Blackboard가 유효하지 않음"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AIController가 유효하지 않음"));
        }

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("mOwnerMonster가 BossLich가 아님"));
    }

}

void UBossMonsterAnimInstance::AnimNotify_Teleport()
{
    if (ABossLich* Boss = Cast<ABossLich>(mOwnerMonster))
    {
        if (!Boss->mASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.CoolDown.TeleportSkill"))))
        {
            UE_LOG(LogTemp, Warning, TEXT("Use TeleportSkill"));
            Boss->UseTeleportSkill();
        }
    }

    UAnimMontage* Montage = GetCurrentActiveMontage();
    if (Montage)
    {
        UE_LOG(LogTemp, Warning, TEXT("현재 재생 중인 몽타주: %s, 위치: %f / %f"),
            *Montage->GetName(),
            Montage_GetPosition(Montage),
            Montage->GetPlayLength());
    }
}


void UBossMonsterAnimInstance::AnimNotify_TeleportEnd()
{
    UE_LOG(LogTemp, Warning, TEXT("<<<^^^^^^^^^^^ TeleportEnd Notify 진입"));

    UAnimMontage* Montage = GetCurrentActiveMontage();
    if (Montage)
    {
        UE_LOG(LogTemp, Warning, TEXT("현재 재생 중인 몽타주: %s, 위치: %f / %f"),
            *Montage->GetName(),
            Montage_GetPosition(Montage),
            Montage->GetPlayLength());
    }

    ABossLich* Boss = Cast<ABossLich>(mOwnerMonster);
    if (!IsValid(mOwnerMonster))
        return;

    AAIController* AIController = Cast<AAIController>(mOwnerMonster->GetController());
    if (!IsValid(AIController))
        return;

    UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();

    if (IsValid(Blackboard))
    {
        Blackboard->SetValueAsBool(MonsterAIKey::TeleportEnd, true);
        UE_LOG(LogTemp, Warning, TEXT("TeleportEnd = true 설정됨"));
    }

}

void UBossMonsterAnimInstance::PlayHit(AActor* Attacker)
{
    if (bIsPlayingHit)
        return;

    if (IsValid(mOwnerMonster) && IsValid(Attacker))
        mHitDirection = GetHitDirection(mOwnerMonster, Attacker);
    else
        mHitDirection = EHitDirection::Front;

    //UE_LOG(LogTemp, Warning, TEXT(" [PlayHit] HitAlpha: %f, bIsPlayingHit: %s"), mHitAlpha, bIsPlayingHit ? TEXT("true") : TEXT("false"));

    Super::PlayHit(Attacker);
}
