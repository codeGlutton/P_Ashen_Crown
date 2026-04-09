#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "MVVMViewModelBase.h"
#include "Components/SlateWrapperTypes.h"
#include "ComposableViewModelBase.generated.h"

/************************
 UComposableViewModelBase
*************************/

/**
 * 내부 하위 ViewModel과 상호작용할 수 있는 ViewModel 베이스 클래스
 */
UCLASS(Abstract)
class P_ASHEN_CROWN_API UComposableViewModelBase : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/**
	 * 델리게이트 바인딩 등의 초기화 작업 함수. OnConstruct 이후에 호출 필요
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewModel")
	void Initialize();
	virtual void Initialize_Implementation() {}

	/**
	 * 델리게이트 언바인딩 등의 소멸 시기 작업 함수. OnDestruct 이후에 호출 필요
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewModel")
	void Deinitialize();
	virtual void Deinitialize_Implementation() {}
};
