// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GASCore.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueNotify_Effect.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UGameplayCueNotify_Effect : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UGameplayCueNotify_Effect();
	
protected:
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	// TObjectPtr<UParticleSystem>	mParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem>	mNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<USoundBase>	mSound;

	// Camera Shake?

public:
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;
};
