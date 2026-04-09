#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "UObject/Interface.h"
#include "PauseViewHandler.generated.h"

UINTERFACE(MinimalAPI)
class UPauseViewHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 일시 정지 View와 연결해주는 객체 인터페이스
 */
class P_ASHEN_CROWN_API IPauseViewHandler
{
	GENERATED_BODY()

public:
	virtual void ToggleView() = 0;
};
