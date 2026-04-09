#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "GameplayEffect.h"
#include "GE_LoadHP.generated.h"

/**
 * HP 로드 이펙트 클래스
 */
UCLASS()
class P_ASHEN_CROWN_API UGE_LoadHP : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UGE_LoadHP();
};
