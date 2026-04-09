// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GAS/GASCore.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_ApplyHeal.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UAN_ApplyHeal : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
