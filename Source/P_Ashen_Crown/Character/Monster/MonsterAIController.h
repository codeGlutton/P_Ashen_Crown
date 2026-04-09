// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameCore.h"
#include "../Monster/MonsterInfo.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterAIController();

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAIPerceptionComponent>	mAIPerceptionComp;

    TObjectPtr<UAISenseConfig_Sight> mSightConfig;

    TObjectPtr<UBehaviorTree>		mAITree;

    UPROPERTY()
    TObjectPtr<UBehaviorTreeComponent> mBehaviorTreeComp;

    UPROPERTY()
    TObjectPtr<UBlackboardComponent> mBlackboardComp;

public:
    virtual void OnConstruction(const FTransform& Transform);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void OnPossess(APawn* InPawn) override;

public:
    // 시야에 타겟을 인지했을 때 호출되는 함수입니다.
    // @param Actor       인지된 액터(플레이어)
    // @param Stimulus    자극 정보(시야)
    UFUNCTION()
    void OnTarget(AActor* Actor, FAIStimulus Stimulus);

    // 이전에 인지했던 타겟을 감지하지 못했을 때 호출되는 함수입니다.
    // @param Actor       감지에서 벗어난 액터
    UFUNCTION()
    void OnTargetForgotten(AActor* Actor);
	
};
