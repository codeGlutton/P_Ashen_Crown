// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterBase.h"
#include "NormalMonster.h"

AMonsterAIController::AMonsterAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    mAIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SetPerceptionComponent(*mAIPerceptionComp);

    mBlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
    mBehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

    // 시야 감각기관을 생성한다.
    mSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

    //// 시야 반경
    //mSightConfig->SightRadius = 800.0f;
    //// 시야를 잃는 반경
    //mSightConfig->LoseSightRadius = 1000.0f;
    //// 주변 시야각
    //mSightConfig->PeripheralVisionAngleDegrees = 90.0f;

    // 적 감지
    mSightConfig->DetectionByAffiliation.bDetectEnemies = true;
    // 아군 감지
    mSightConfig->DetectionByAffiliation.bDetectFriendlies = false;
    // 중립 감지
    mSightConfig->DetectionByAffiliation.bDetectNeutrals = false;

    // 시야를 AIPerception에 추가한다.
    mAIPerceptionComp->ConfigureSense(*mSightConfig);

    // 주 감각기관을 시야로 설정한다.
    mAIPerceptionComp->SetDominantSense(mSightConfig->GetSenseImplementation());

    mSightConfig->SetMaxAge(5.f);

    SetGenericTeamId((FGenericTeamId)(uint8)ETeamType::Monster);
}

void AMonsterAIController::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

void AMonsterAIController::BeginPlay()
{
    Super::BeginPlay();

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("BehaviorTree Started!"));
    //Blackboard->SetValueAsFloat(TEXT("LookAroundTime"), 3.f);
}

void AMonsterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AMonsterBase* Monster = Cast<AMonsterBase>(GetPawn());

    /*if (Blackboard && (!Blackboard->GetValueAsObject(MonsterAIKey::TargetActor)))
        UC_Util::Print("Not Valid");*/
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    AMonsterBase* Monster = Cast<AMonsterBase>(GetPawn());
    if (IsValid(Monster))
    {
        mAIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnTarget);
        mAIPerceptionComp->OnTargetPerceptionForgotten.AddDynamic(this, &AMonsterAIController::OnTargetForgotten);
    }

    UBlackboardComponent* TempBlackboard = nullptr;
    if (IsValid(Monster) && Monster->GetBehaviorTree())
    {
        // Blackboard 초기화
        if (UseBlackboard(Monster->GetBlackboardData(), TempBlackboard))
        {
            mBlackboardComp = TempBlackboard;
            // BehaviorTree 실행
            RunBehaviorTree(Monster->GetBehaviorTree());
        }
    }

    if (IsValid(Monster) && IsValid(mSightConfig))
    {
        mSightConfig->SightRadius = Monster->mSightRadius;
        mSightConfig->LoseSightRadius = Monster->mLoseSightRadius;
        mSightConfig->PeripheralVisionAngleDegrees = Monster->mPeripheralVisionAngle;

        mAIPerceptionComp->ConfigureSense(*mSightConfig);
        mAIPerceptionComp->RequestStimuliListenerUpdate();
    }
}

void AMonsterAIController::OnTarget(AActor* Actor, FAIStimulus Stimulus)
{
    AMonsterBase* Monster = Cast<AMonsterBase>(GetPawn());

    AActor* TargetActor = Actor;

    // 몬스터 AI가 적(플레이어)을 인지했을 때
    if (Blackboard)
    {
        // 타겟이 감지된 경우
        if (Stimulus.WasSuccessfullySensed())
        {
            // Blackboard에 인지된 타겟 위치 및 상태 업데이트
            Blackboard->SetValueAsObject(MonsterAIKey::TargetActor, TargetActor);
            //Blackboard->SetValueAsVector(TEXT("MemoryLocation"), TargetActor->GetActorLocation());

            float Dist = FVector::Dist(Monster->GetActorLocation(), TargetActor->GetActorLocation());
            bool bInAttackRange = Dist <= Monster->GetMonsterAttribute()->GetAttackDistance();
            Blackboard->SetValueAsBool(MonsterAIKey::IsInAttackRange, bInAttackRange);
        }
    }
    
    ANormalMonster* NormalMon = Cast<ANormalMonster>(GetPawn());

    if (IsValid(NormalMon))
    {
        NormalMon->ClearReturnTimer();
    }
}

void AMonsterAIController::OnTargetForgotten(AActor* Actor)
{
    AActor* TargetActor = Actor;

    if (TargetActor)
    {
        // 인지 실패 또는 시야에서 벗어날 경우
        TargetActor = nullptr;
        // Blackboard에서 타겟 정보 제거 또는 상태 변경
        Blackboard->SetValueAsObject(MonsterAIKey::TargetActor, nullptr);
        Blackboard->SetValueAsBool(MonsterAIKey::IsInAttackRange, false);
    }
}
