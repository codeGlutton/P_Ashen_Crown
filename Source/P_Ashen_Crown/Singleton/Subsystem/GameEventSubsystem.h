#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameEventSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCallEvent, AActor* /* Instigator */, AActor* /* MainTarget */);
DECLARE_DELEGATE_TwoParams(FOnLoadEventData, const FName& /* Key */, bool& /* bWasTriggered */);

/**
 * 게임 이벤트 정보 구조체
 */
USTRUCT(BlueprintType)
struct FGameEventContext
{
	GENERATED_BODY()

public:
	FOnCallEvent mEvent;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "GameEventContext")
	bool bIsTriggered;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameEventContext")
	bool bAutoReset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameEventContext", meta = (ToolTip="휘발적인 비저장 이벤트"))
	bool bIsTransient;
};

/**
 * 게임 이벤트를 관리하는 서브시스템
 */
UCLASS()
class P_ASHEN_CROWN_API UGameEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	/**
	 * 이벤트 데이터를 로드해 초기화해주는 함수를 저장
	 * @param LoadEventDataSolver 해당 함수가 연결된 대리자
	 */
	void RegisterLoadEventSolver(const FOnLoadEventData& LoadEventDataSolver);
	void LoadEvents();

private:
	static void LoadEventDefaultSolver(const FName& Key, bool& bWasTriggered) {}
	
public:
	/**
	 * 이벤트를 등록하는 함수
	 * @param Key 이벤트 이름
	 * @param Context 이벤트 정보
	 */
	void RegisterEvent(const FName& Key, const FGameEventContext& Context);
	/**
	 * 이벤트를 해제하는 함수
	 * @param Key 이벤트 이름
	 * @param Context 이벤트 정보
	 */
	void UnregisterEvent(const FName& Key);

	/**
	 * 해당하는 이벤트에 호출될 함수 바인딩
	 * @param Key 이벤트 이름
	 * @param Callback 호출될 콜백 함수
	 * @return 대리자 핸들
	 */
	FDelegateHandle BindToEvent(const FName& Key, const FOnCallEvent::FDelegate& Callback);
	/**
	 * 해당하는 이벤트에 호출되는 함수 언바인딩
	 * @param Key 이벤트 이름
	 * @param Handle 대리자 핸들
	 */
	void UnbindToEvent(const FName& Key, const FDelegateHandle& Handle);

public:
	/**
	 * 이벤트 브로드캐스팅용 함수
	 * @param Key 이벤트 이름
	 * @param Instigator 이벤트 유발자
	 * @param MainTarget 메인 타겟
	 * @return 호출 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "GameEventSubsystem")
	bool CallEvent(const FName& Key, AActor* Instigator, AActor* MainTarget = nullptr);

	/**
	 * 이미 해당 이벤트가 트리거 되었는지 확인하는 함수
	 * @param Key 이벤트 이름
	 * @return 트리거 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "GameEventSubsystem")
	bool WasTriggerEvent(const FName& Key) const;
	/**
	 * 해당 이벤트의 트리거 여부를 리셋하는 함수
	 * @param Key 이벤트 이름
	 */
	UFUNCTION(BlueprintCallable, Category = "GameEventSubsystem")
	void ResetEvent(const FName& Key);

public:
	UFUNCTION(BlueprintCallable, Category = "GameEventSubsystem")
	const TMap<FName, FGameEventContext>& GetAllEventContext() const;

protected:
	void RegisterWorldEvents(UWorld* World, const UWorld::InitializationValues IVS);
	void UnRegisterWorldEvents(UWorld* World);

protected:
	// 각 이벤트 데이터를 로드하는 대리자
	FOnLoadEventData OnLoadEventData;
	TMap<FName, FGameEventContext> mEventContexts;
};
