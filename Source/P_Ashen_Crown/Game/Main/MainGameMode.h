#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

/**
 * 메인 게임 모드 클래스 (실제 전투할 레벨)
 */
UCLASS()
class P_ASHEN_CROWN_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGameMode();

public:
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
};
