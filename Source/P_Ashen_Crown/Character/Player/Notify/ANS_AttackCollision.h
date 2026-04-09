// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GAS/GASCore.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_AttackCollision.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UANS_AttackCollision : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UANS_AttackCollision();
	
public:
	UPROPERTY(EditAnywhere, Category = "Attack")
	float Radius;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName AttackSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TObjectPtr<UNiagaraSystem> mHitNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TObjectPtr<USoundBase> mHitSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<class UGameplayEffect> mDamageEffectClass;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	TSet<AActor*> mAlreadyHitActors;

	void PerformHitCheck(AActor* Owner, USkeletalMeshComponent* MeshComp);
};
