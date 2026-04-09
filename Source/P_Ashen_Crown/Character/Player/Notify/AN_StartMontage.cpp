// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_StartMontage.h"
#include "../PlayerCharacter.h"

void UAN_StartMontage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!MeshComp)
		return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
		return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(Owner);
	if (!Player)
		return;

	Player->CharacterMoveStop();
}
