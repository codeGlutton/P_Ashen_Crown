// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameCore.h"
#include "UObject/Interface.h"
#include "PotionProvider.generated.h"

UINTERFACE(MinimalAPI)
class UPotionProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * Potion 개수확인 및 소모
 */
class P_ASHEN_CROWN_API IPotionProvider
{
	GENERATED_BODY()

public:
	virtual bool HasPotion() const = 0;
	virtual bool ConsumePotion() = 0;
};
