#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Engine/DeveloperSettings.h"

#include "GenericTeamAgentInterface.h"

#include "TeamSettings.generated.h"

UENUM(BlueprintType)
enum class ETeamType : uint8
{
    NoTeam = 0,
    Player,
    Monster,
    NPC
};

/**
 * 한 팀에서 다른 팀들을 대하는 관계 정보 구조체
 */
USTRUCT(BlueprintType)
struct FTeamRelation
{
    GENERATED_BODY()

public:
    FTeamRelation() : mAttitudes() {}
    FTeamRelation(std::initializer_list<TPairInitializer<const ETeamType&, const TEnumAsByte<ETeamAttitude::Type>&>> Attitudes) : mAttitudes(MoveTemp(Attitudes)) {}

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Relationship")
    TMap<ETeamType, TEnumAsByte<ETeamAttitude::Type>> mAttitudes;
};

/************************
	  UTeamSettings
*************************/

/**
 * Team Id에 대한 개발자 세팅창
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "TeamSetting"))
class P_ASHEN_CROWN_API UTeamSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UTeamSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
public:
    /* Delveloper Setting 함수들 */

    virtual FName GetCategoryName() const override;

#if WITH_EDITOR
    virtual FText GetSectionText() const override;
    virtual FText GetSectionDescription() const override;
#endif

public:
    /**
     * 팀에 대해서 어떤 상태를 가지고 대할지 결정하는 정적 함수
     * @param Own 관측자 TeamID
     * @param Other 관측대상 TeamID
     * @return 관측자가 관측대상을 어떤 자세로 바라볼지에 대한 관계값
     */
    UFUNCTION(BlueprintPure, Category = "AI | TeamSettings")
    static ETeamAttitude::Type GetAttitude(FGenericTeamId Own, FGenericTeamId Other);

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Relations | TeamSettings")
    TMap<ETeamType, FTeamRelation> mTeamRelations;
};
