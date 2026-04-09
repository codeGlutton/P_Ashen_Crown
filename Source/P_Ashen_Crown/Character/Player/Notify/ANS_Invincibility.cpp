// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_Invincibility.h"

void UANS_Invincibility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (!MeshComp) 
		return;

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) 
		return;
	
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor);
	if (!OwnerCharacter)
		return;

	UCapsuleComponent* CapsuleComp = OwnerCharacter->GetCapsuleComponent();
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionProfileName(TEXT("PlayerRoll"));
	}
}

void UANS_Invincibility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (!MeshComp) 
		return;

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) 
		return;
	
	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor);
	if (!OwnerCharacter)
		return;

	UCapsuleComponent* CapsuleComp = OwnerCharacter->GetCapsuleComponent();
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionProfileName(TEXT("Player"));
	}
}
