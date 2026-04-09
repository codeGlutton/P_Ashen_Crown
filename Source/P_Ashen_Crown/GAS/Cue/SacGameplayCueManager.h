// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GASCore.h"
#include "GameplayCueManager.h"
#include "SacGameplayCueManager.generated.h"

/**
 * 
 */
UCLASS()
class P_ASHEN_CROWN_API USacGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()
	
public:
	virtual bool ShouldAsyncLoadRuntimeObjectLibraries() const override;
	
	
};
