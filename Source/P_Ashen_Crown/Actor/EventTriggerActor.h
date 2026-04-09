#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "GameFramework/Actor.h"
#include "EventTriggerActor.generated.h"

UCLASS()
class P_ASHEN_CROWN_API AEventTriggerActor : public AActor
{
	GENERATED_BODY()

public:
	AEventTriggerActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void CallEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// 트리거 감지용 콜리전
	UPROPERTY(EditAnywhere, Category = "EventTriggerActor", meta = (DisplayName = "TriggerCollision"))
	TObjectPtr<UBoxComponent> mTriggerCollision;

private:
	UPROPERTY(EditAnywhere, Category = "EventTriggerActor", meta = (DisplayName = "EventName"))
	FName mEventName;

	UPROPERTY(EditAnywhere, Category = "EventTriggerActor", meta = (DisplayName = "ResetTime"))
	float mResetTime;

private:
	bool bWasTriggered;
};
