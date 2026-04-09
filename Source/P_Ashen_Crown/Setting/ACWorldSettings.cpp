#include "ACWorldSettings.h"

const TMap<FName, FMVVMViewInfo>& AACWorldSettings::GetWorldSharedViews() const
{
    return mWorldSharedViews;
}

const TMap<FName, FGameEventContext>& AACWorldSettings::GetWorldGameEvents() const
{
    return mWorldGameEvents;
}

const FString& AACWorldSettings::GetStageName() const
{
    return mStageName;
}
