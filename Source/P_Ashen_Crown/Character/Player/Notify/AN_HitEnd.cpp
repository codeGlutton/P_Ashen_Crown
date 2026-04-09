// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_HitEnd.h"
#include "../PlayerCharacter.h"
#include "../PlayerAnimInstance.h"

void UAN_HitEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                        const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (APlayerCharacter* PC = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		PC->GetAnimInstance()->SetHitReset(false);
		PC->GetASC()->RemoveLooseGameplayTag(AC_PlayerState::Player_State_Hit.GetTag());
	}
}
