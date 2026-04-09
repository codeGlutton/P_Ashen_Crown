// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterInfo.h"

FRotator GetTargetRotator(const FVector& Target, const FVector& Current)
{
	return UKismetMathLibrary::FindLookAtRotation(Current, Target);
}

FRotator GetTargetRotatorYaw(FVector Target, FVector Current)
{
	Target.Z = 0.0;
	Current.Z = 0.0;

	return UKismetMathLibrary::FindLookAtRotation(Current, Target);
}

EHitDirection GetHitDirection(AActor* Victim, AActor* Attacker)
{
	// Victim : 몬스터
	// Attacker : 플레이어
	FVector Forward = Victim->GetActorForwardVector();
	FVector Right = Victim->GetActorRightVector();
	FVector ToAttacker = (Attacker->GetActorLocation() - Victim->GetActorLocation()).GetSafeNormal();

	float ForwardDot = FVector::DotProduct(Forward, ToAttacker);
	float RightDot = FVector::DotProduct(Right, ToAttacker);

	// 공격자 정면 45도 이내
	if (ForwardDot > 0.7f)
		return EHitDirection::Front;

	// 공격자 후면 135도 이상
	if (ForwardDot < -0.7f)
		return EHitDirection::Back;

	// 공격자 Right& Left / 정면과 직각 90도
	if (RightDot > 0.f)
		return EHitDirection::Right;

	return EHitDirection::Left;
}