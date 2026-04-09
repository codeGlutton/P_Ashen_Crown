#include "GameEventSettings.h"

FName UGameEventSettings::GetCategoryName() const
{
	return FName(TEXT("Game"));
}

FText UGameEventSettings::GetSectionText() const
{
	return FText::FromString(TEXT("Game Event Setting"));
}

FText UGameEventSettings::GetSectionDescription() const
{
	return FText::FromString(TEXT("Set up game events that exist globally and within the world"));
}
