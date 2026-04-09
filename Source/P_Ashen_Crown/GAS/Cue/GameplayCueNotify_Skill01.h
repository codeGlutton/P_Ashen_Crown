// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GASCore.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueNotify_Skill01.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API UGameplayCueNotify_Skill01 : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UGameplayCueNotify_Skill01();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem>	mNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<USoundBase>	mSound;

public:
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;
};
