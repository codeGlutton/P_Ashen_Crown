#include "TeamSettings.h"

UTeamSettings::UTeamSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    FGenericTeamId::SetAttitudeSolver(&UTeamSettings::GetAttitude);
}

FName UTeamSettings::GetCategoryName() const
{
    return FName(TEXT("Game"));
}

#if WITH_EDITOR
FText UTeamSettings::GetSectionText() const
{
    return FText::FromString(TEXT("Team Setting"));
}

FText UTeamSettings::GetSectionDescription() const
{
    return FText::FromString(TEXT("Set up relationships between teams so that AI can recognize relationships"));
}
#endif

ETeamAttitude::Type UTeamSettings::GetAttitude(FGenericTeamId Own, FGenericTeamId Other)
{
    const auto& TeamRelations = GetDefault<UTeamSettings>()->mTeamRelations;
    const auto* TeamRelation = TeamRelations.Find(StaticCast<ETeamType>(Own.GetId()));
    if (TeamRelation != nullptr)
    {
        const auto* Attitude = TeamRelation->mAttitudes.Find(StaticCast<ETeamType>(Other.GetId()));
        if (Attitude != nullptr)
        {
            return *Attitude;
        }
    }
    return ETeamAttitude::Neutral;
}
