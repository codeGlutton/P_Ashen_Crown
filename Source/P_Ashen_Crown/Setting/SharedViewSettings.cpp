#include "SharedViewSettings.h"

FName USharedViewSettings::GetCategoryName() const
{
	return FName(TEXT("Game"));
}

FText USharedViewSettings::GetSectionText() const
{
	return FText::FromString(TEXT("Shared View Setting"));
}

FText USharedViewSettings::GetSectionDescription() const
{
	return FText::FromString(TEXT("Set up a shared view object that shares the same lifetime as the instance"));
}
