// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameCore.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_StartMontage.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UAN_StartMontage : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
