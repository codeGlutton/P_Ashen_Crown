// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_ComboAttack.h"

void UANS_ComboAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
    
    if (!MeshComp) 
        return;

    AActor* OwnerActor = MeshComp->GetOwner();
    if (!OwnerActor) 
        return;

    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
    if (!ASC) 
        return;

    ASC->AddLooseGameplayTag(AC_PlayerState::Event_ComboWindow_Open.GetTag());
}

void UANS_ComboAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
    
    if (!MeshComp)
        return;

    AActor* OwnerActor = MeshComp->GetOwner();
    if (!OwnerActor)
        return;

    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
    if (!ASC)
        return;

    ASC->RemoveLooseGameplayTag(AC_PlayerState::Event_ComboWindow_Open.GetTag());
}