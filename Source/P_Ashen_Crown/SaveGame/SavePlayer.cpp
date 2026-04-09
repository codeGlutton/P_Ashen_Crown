#include "SavePlayer.h"

USavePlayer::USavePlayer() : mStageName(TEXT("Empty")), mLevel(0), mDate(), mPosition(), mRotation(), mEquipItemKeys(), mInventoryItemKeys(), mTriggeredEvents()
{
}

const FString& USavePlayer::GetStageName() const
{
	return mStageName;
}

int32 USavePlayer::GetLevel() const
{
	return mLevel;
}

FDateTime USavePlayer::GetDate() const
{
	return mDate;
}

float USavePlayer::GetHP() const
{
	return mHP;
}

const FVector& USavePlayer::GetPosition() const
{
	return mPosition;
}

const FRotator& USavePlayer::GetRotation() const
{
	return mRotation;
}

bool USavePlayer::WasTriggeredEvent(const FName& Name) const
{
	return mTriggeredEvents.Contains(Name);
}

const TArray<FSaveItemKeyInfo>& USavePlayer::GetEquipItemKeys() const
{
	return mEquipItemKeys;
}

const TArray<FSaveItemKeyInfo>& USavePlayer::GetInventoryItemKeys() const
{
	return mInventoryItemKeys;
}

const TSet<FName>& USavePlayer::GetTriggeredEvents() const
{
	return mTriggeredEvents;
}

void USavePlayer::SetStageName(const FString& StageName)
{
	mStageName = StageName;
}

void USavePlayer::SetLevel(int32 Level)
{
	mLevel = Level;
}

void USavePlayer::SetDate(FDateTime Date)
{
	mDate = Date;
}

void USavePlayer::SetHP(float HP)
{
	mHP = HP;
}

void USavePlayer::SetPosition(const FVector& Position)
{
	mPosition = Position;
}

void USavePlayer::SetRotation(const FRotator& Rotation)
{
	mRotation = Rotation;
}

void USavePlayer::SetTiggeredEvent(const FName& Name, bool bTriggered)
{
	if (bTriggered == true)
	{
		mTriggeredEvents.Emplace(Name);
	}
	else
	{
		mTriggeredEvents.Remove(Name);
	}
}

void USavePlayer::SetEquipItemKeys(const TArray<FSaveItemKeyInfo>& EquipItemKeys)
{
	mEquipItemKeys = EquipItemKeys;
}

void USavePlayer::SetInventoryItemKeys(const TArray<FSaveItemKeyInfo>& InventoryItemKeys)
{
	mInventoryItemKeys = InventoryItemKeys;
}

void USavePlayer::SetTiggeredEvents(const TSet<FName>& TriggeredEvents)
{
	mTriggeredEvents = TriggeredEvents;
}

USavePlayer* USavePlayer::Clone() const
{
	auto* CloneObj = NewObject<USavePlayer>(GetOuter());
	CloneObj->SetStageName(GetStageName());
	CloneObj->SetLevel(GetLevel());
	CloneObj->SetDate(GetDate());
	CloneObj->SetHP(GetHP());
	CloneObj->SetPosition(GetPosition());
	CloneObj->SetRotation(GetRotation());
	CloneObj->SetEquipItemKeys(GetEquipItemKeys());
	CloneObj->SetInventoryItemKeys(GetInventoryItemKeys());
	CloneObj->SetTiggeredEvents(GetTriggeredEvents());
	return CloneObj;
}
