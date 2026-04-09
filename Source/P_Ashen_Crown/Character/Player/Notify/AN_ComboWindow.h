// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameCore.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AN_ComboWindow.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UAN_ComboWindow : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComboWindowNotify")
	FGameplayTag EventTag;

protected:
	virtual bool Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) const;
};
