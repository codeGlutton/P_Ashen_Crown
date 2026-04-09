// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameCore.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "TestStateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P_ASHEN_CROWN_API UTestStateComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	FGameplayTag mCurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bMovementInputEnabled = true;

public:	
	UTestStateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/// <summary>
	/// 게임플레이 상태를 결정합니다.
	/// </summary>
	/// <param name="NewState"> 설정할 새로운 FGameplayTag 상태입니다 </param>
	void SetState(const FGameplayTag NewState);

	/// <summary>
	/// 현재 상태를 나타내는 FGameplayTag를 반환합니다.
	/// </summary>
	/// <returns> 현재 상태를 나타내는 FGameplayTag 객체입니다 </returns>
	FGameplayTag GetCurrentState() const;

	/// <summary>
	/// 현재 상태가 주어진 태그 중 하나와 동일한지 확인합니다
	/// </summary>
	/// <param name="TagsToCheck"> 비교할 게임플레이 태그들의 컨테이너입니다 </param>
	/// <returns> 현재 상태가 TagsToCheck에 포함된 태그 중 하나와 같으면 true, 그렇지 않으면 false를 반환합니다 </returns>
	bool IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const;

	/// <summary>
	/// 이동 입력이 활성화되어 있는지 확인합니다.
	/// </summary>
	/// <returns> 이동 입력이 활성화되어 있으면 true, 그렇지 않으면 false를 반환합니다 </returns>
	bool MovementInputEnabled() const;

	/// <summary>
	/// 이동 입력을 활성화하거나 비활성화합니다.
	/// </summary>
	/// <param name="bEnabled"> 이동 입력을 활성화할지 여부를 지정합니다. true이면 활성화, false이면 비활성화합니다 </param>
	/// <param name="Duration"> 이동 입력의 활성화 또는 비활성화가 적용될 지속 시간(초)입니다. 기본값은 0.0f로, 즉시 적용됩니다 </param>
	void ToggleMovementInput(bool bEnabled, float Duration = 0.f);

protected:
	/// <summary>
	/// 이동 입력을 활성화하는 액션을 수행합니다
	/// </summary>
	UFUNCTION()
	void MovementInputEnableAction();
		
};
