#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Engine/DeveloperSettings.h"
#include "P_Ashen_Crown/Singleton/Subsystem/GameEventSubsystem.h"
#include "GameEventSettings.generated.h"

/**
 * 게임 이벤트 세팅 클래스
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "GameEventSetting"))
class P_ASHEN_CROWN_API UGameEventSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
    /* Delveloper Setting 함수들 */

    virtual FName GetCategoryName() const override;

#if WITH_EDITOR
    virtual FText GetSectionText() const override;
    virtual FText GetSectionDescription() const override;
#endif

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "GameEvent | UGameEventSettings")
    TMap<FName, FGameEventContext> mGlobalGameEvents;
};
