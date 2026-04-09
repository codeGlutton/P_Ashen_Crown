// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_ComboWindow.h"
#include "AbilitySystemBlueprintLibrary.h"

bool UAN_ComboWindow::Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) const
{
    if (!MeshComp)
        return false;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner)
        return false;

    FGameplayEventData Payload;
    Payload.Instigator = Owner;
    Payload.Target = Owner;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        Owner,
        EventTag,
        Payload
    );

    return true;
}