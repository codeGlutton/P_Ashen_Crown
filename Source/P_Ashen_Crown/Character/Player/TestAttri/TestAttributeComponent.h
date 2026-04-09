// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameCore.h"
#include "Components/ActorComponent.h"
#include "TestAttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P_ASHEN_CROWN_API UTestAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float mBaseStamina = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float mMaxStamina = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float mRegenrateStamina = 0.7f;

	FTimerHandle mStaminaRegenTimerHandle;

public:	
	// Sets default values for this component's properties
	UTestAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/*FORCEINLINE float GetBaseStamina()	const
	{
		return mBaseStamina;
	}*/

	/*FORCEINLINE float GetMaxStamina()	const
	{
		return mMaxStamina;
	}*/

	/// <summary>
	/// 주어진 스태미나 소모량에 대해 충분한 스태미나가 있는지 확인합니다.
	/// </summary>
	/// <param name="StaminaCost">필요한 스태미나의 양입니다</param>
	/// <returns>스태미나가 충분하면 true, 그렇지 않으면 false를 반환합니다</returns>
	bool CheckHasEnoughStamina(float StaminaCost) const;

	/// <summary>
	/// 스태미나를 지정된 양만큼 감소시킵니다.
	/// </summary>
	/// <param name="StaminaCost">감소시킬 스태미나의 양입니다</param>
	void DecreaseStamina(float StaminaCost);

	/// <summary>
	/// 스태미나 재생 기능을 활성화하거나 비활성화합니다.
	/// </summary>
	/// <param name="bEnabled"> 스태미나 재생을 활성화할지 여부를 지정합니다. true이면 활성화, false이면 비활성화됩니다 </param>
	/// <param name="StartDelay"> 재생이 시작되기 전까지 대기할 시간(초)입니다. 기본값은 2.0초입니다 </param>
	void ToggleStaminaRegeneration(bool bEnabled, float StartDelay = 2.f);

protected:
	/// <summary>
	/// 스태미나 재생 구현함수입니다.
	/// </summary>
	void RegenerateStaminaHandle();
};

