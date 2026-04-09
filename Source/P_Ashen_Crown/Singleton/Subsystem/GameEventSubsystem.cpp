#include "GameEventSubsystem.h"
#include "P_Ashen_Crown/Setting/ACWorldSettings.h"
#include "P_Ashen_Crown/Setting/GameEventSettings.h"

void UGameEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	OnLoadEventData.BindStatic(&UGameEventSubsystem::LoadEventDefaultSolver);

	auto* GameEventSettingsCDO = GetDefault<UGameEventSettings>();
	if (IsValid(GameEventSettingsCDO) == false)
	{
		return;
	}

	for (auto& Event : GameEventSettingsCDO->mGlobalGameEvents)
	{
		RegisterEvent(Event.Key, Event.Value);
	}

	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UGameEventSubsystem::RegisterWorldEvents);
	FWorldDelegates::OnPreWorldFinishDestroy.AddUObject(this, &UGameEventSubsystem::UnRegisterWorldEvents);
}

void UGameEventSubsystem::Deinitialize()
{
	auto* GameEventSettingsCDO = GetDefault<UGameEventSettings>();
	if (IsValid(GameEventSettingsCDO) == false)
	{
		return;
	}

	for (auto& Event : GameEventSettingsCDO->mGlobalGameEvents)
	{
		UnregisterEvent(Event.Key);
	}

	Super::Deinitialize();
}

void UGameEventSubsystem::RegisterLoadEventSolver(const FOnLoadEventData& LoadEventDataSolver)
{
	if (LoadEventDataSolver.IsBound() == false)
	{
		return;
	}
	OnLoadEventData.Unbind();

	OnLoadEventData = LoadEventDataSolver;
}

void UGameEventSubsystem::LoadEvents()
{
	for (auto& EventContext : mEventContexts)
	{
		OnLoadEventData.Execute(EventContext.Key, EventContext.Value.bIsTriggered);
	}
}

void UGameEventSubsystem::RegisterEvent(const FName& Key, const FGameEventContext& Context)
{
	mEventContexts.Add(Key, Context);
	OnLoadEventData.Execute(Key, mEventContexts[Key].bIsTriggered);
}

void UGameEventSubsystem::UnregisterEvent(const FName& Key)
{
	mEventContexts.Remove(Key);
}

FDelegateHandle UGameEventSubsystem::BindToEvent(const FName& Key, const FOnCallEvent::FDelegate& Callback)
{
	auto* Context = mEventContexts.Find(Key);
	if (Context == nullptr)
	{
		return FDelegateHandle();
	}
	return Context->mEvent.Add(Callback);
}

void UGameEventSubsystem::UnbindToEvent(const FName& Key, const FDelegateHandle& Handle)
{
	auto* Context = mEventContexts.Find(Key);
	if (Context != nullptr)
	{
		Context->mEvent.Remove(Handle);
	}
}

bool UGameEventSubsystem::CallEvent(const FName& Key, AActor* Instigator, AActor* MainTarget)
{
	auto* Context = mEventContexts.Find(Key);
	if (Context != nullptr && Context->bIsTriggered == false)
	{
		if (Context->bAutoReset == false)
		{
			Context->bIsTriggered = true;
		}
		Context->mEvent.Broadcast(Instigator, MainTarget);
		return true;
	}
	return false;
}

bool UGameEventSubsystem::WasTriggerEvent(const FName& Key) const
{
	auto* Context = mEventContexts.Find(Key);
	if (Context == nullptr)
	{
		return false;
	}
	return Context->bIsTriggered;
}

void UGameEventSubsystem::ResetEvent(const FName& Key)
{
	auto* Context = mEventContexts.Find(Key);
	if (Context != nullptr)
	{
		Context->bIsTriggered = false;
	}
}

const TMap<FName, FGameEventContext>& UGameEventSubsystem::GetAllEventContext() const
{
	return mEventContexts;
}

void UGameEventSubsystem::RegisterWorldEvents(UWorld* World, const UWorld::InitializationValues IVS)
{
	auto* WorldSettings = Cast<AACWorldSettings>(World->GetWorldSettings());
	if (IsValid(WorldSettings) == false)
	{
		return;
	}

	for (auto& Event : WorldSettings->GetWorldGameEvents())
	{
		RegisterEvent(Event.Key, Event.Value);
	}
}

void UGameEventSubsystem::UnRegisterWorldEvents(UWorld* World)
{
	auto* WorldSettings = Cast<AACWorldSettings>(World->GetWorldSettings());
	if (IsValid(WorldSettings) == false)
	{
		return;
	}

	for (auto& Event : WorldSettings->GetWorldGameEvents())
	{
		UnregisterEvent(Event.Key);
	}
}
