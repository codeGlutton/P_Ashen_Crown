// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GAS/GASCore.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_ComboAttack3.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UANS_ComboAttack3 : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
