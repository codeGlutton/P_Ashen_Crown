#include "EventTriggerActor.h"
#include "P_Ashen_Crown/Singleton/Subsystem/GameEventSubsystem.h"

AEventTriggerActor::AEventTriggerActor()
{
	PrimaryActorTick.bCanEverTick = true;

	mTriggerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerCollision"));
	mTriggerCollision->SetCollisionProfileName(TEXT("OverlapOnlyPlayer"));
	SetRootComponent(mTriggerCollision);
	
	mTriggerCollision->SetCanEverAffectNavigation(false);
}

void AEventTriggerActor::BeginPlay()
{
	Super::BeginPlay();
}

void AEventTriggerActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	mTriggerCollision->OnComponentBeginOverlap.AddDynamic(this, &AEventTriggerActor::CallEvent);
}

void AEventTriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEventTriggerActor::CallEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bWasTriggered == true || OtherActor != GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		return;
	}

	auto* GameEventSubsystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
	if (IsValid(GameEventSubsystem) == false)
	{
		return;
	}

	bool bSuccessed = GameEventSubsystem->CallEvent(mEventName, this, OtherActor);
	if (bSuccessed == true)
	{
		bWasTriggered = true;
		if (mResetTime > 0.f)
		{
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
				{
					bWasTriggered = false;
				}, mResetTime, false);
		}
	}
}

