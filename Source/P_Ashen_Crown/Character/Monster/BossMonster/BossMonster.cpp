// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include "../MonsterAnimInstance.h"

ABossMonster::ABossMonster()
{
	//PrimaryActorTick.bCanEverTick = true;
}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay();
}


void ABossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어를 바라보게 회전
	/*AActor* Target = GetTargetActor();
	if (IsValid(Target))
	{
		FVector Dir = Target->GetActorLocation() - GetActorLocation();

		Dir.Normalize();

		FRotator LookRot = Dir.Rotation();
		SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));
	}*/
}