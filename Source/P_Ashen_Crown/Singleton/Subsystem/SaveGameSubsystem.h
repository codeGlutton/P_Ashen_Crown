#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsystem.generated.h"

class USavePlayer;
class UUserGameSettingTraker;
class UUserGameSettingTracker;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangePlayerSaveList, const TDoubleLinkedList<TObjectPtr<USavePlayer>>&);

/**
 * 모든 게임 세이브에 관한 게임 인스턴스 서브시스템
 */
UCLASS(Config=Game)
class P_ASHEN_CROWN_API USaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	USaveGameSubsystem();

public:
	/* Player 정보 저장 및 로드 함수 */

	UFUNCTION(BlueprintCallable, Category = "SaveGameSubsystem")
	void LoadAllPlayers();
	UFUNCTION(BlueprintCallable, Category = "SaveGameSubsystem")
	void SaveAllPlayers(bool bAutoUpdateCurPlayer);

public:
	/* Player 연관 데이터 Get/Set */

	int32 GetMaxSavePlayerCount() const;
	
	/**
	 * 인덱스로 SavePlayers 리스트에서 가져오는 함수
	 * @param Index 인덱스
	 * @return 인덱스 번호에 위치한 SavePlayer 포인터
	 */
	TObjectPtr<USavePlayer> GetSavePlayer(int32 Index);
	const TDoubleLinkedList<TObjectPtr<USavePlayer>>& GetSavePlayers();

	TObjectPtr<USavePlayer> GetCurSavePlayer();
	void SetCurSavePlayer(TObjectPtr<USavePlayer> CurSavePlayer);

public:
	/**
	 * 새로운 저장 데이터를 SavePlayers에 넣어두는 함수
	 * (최대 저장 공간 mMaxSavePlayerCount을 초과하면 과거 순으로 제거)
	 * @param SavePlayer 새로운 SavePlayer 저장 데이터
	 */
	void AddSavePlayer(TObjectPtr<USavePlayer> SavePlayer);

	/**
	 * 현재 SavePlayer를 SavePlayers에 넣어두는 함수
	 * @param bAutoUpdateCurPlayer 현재 게임 정보를 자동으로 반영할지
	 */
	void AddCurPlayer(bool bAutoUpdateCurPlayer = true);

	/**
	 * 기존의 SavePlayer를 삭제하고, 리스트 정렬해두는 함수
	 * @param TargetSavePlayer 삭제할 대상 SavePlayer
	 */
	void RemoveSavePlayer(TObjectPtr<USavePlayer> TargetSavePlayer);
	/**
	 * 기존의 SavePlayer를 삭제하고, 리스트 정렬해두는 함수
	 * @param TargetIndex 삭제할 대상 SavePlayer의 인덱스
	 */
	void RemoveSavePlayer(int32 TargetIndex);

protected:
	/**
	 * 이벤트를 로드해 초기화할 함수
	 * @param Key 이벤트 키 값
	 * @param bWasTriggered 트리거 여부 반환 값
	 */
	void LoadEventData(const FName& Key, bool& bWasTriggered);

public:
	/* 게임 세팅 저장 및 로드 함수 */

	UFUNCTION(BlueprintCallable, Category = "SaveGameSubsystem")
	void LoadOptionSetting();
	UFUNCTION(BlueprintCallable, Category = "SaveGameSubsystem")
	void SaveOptionSetting();

public:
	/* 게임 세팅 연관 데이터 Get/Set */

	TObjectPtr<UUserGameSettingTracker> GetUserGameSettingTracker();

public:
	FOnChangePlayerSaveList OnChangePlayerSaveList;

protected:
	// 최대 SavePlayer 저장 갯수. DefaultGame.ini에서 설정
	UPROPERTY(Config)
	int32 mMaxSavePlayerCount;

	// 플레이어 캐릭터 SavePlayer 파일 이름. DefaultGame.ini에서 설정
	UPROPERTY(Config)
	FString mSavePlayerFileName;

	// GC 방지용으로 저장해두는 SavePlayer 포인터 Set
	UPROPERTY()
	TSet<TObjectPtr<USavePlayer>> mSavePlayerOrigins;
	// 모든 SavePlayer
	TDoubleLinkedList<TObjectPtr<USavePlayer>> mSavePlayers;

	// 현재 SavePlayer (게임 도중에만 사용)
	UPROPERTY()
	TObjectPtr<USavePlayer> mCurSavePlayer;

protected:
	UPROPERTY()
	TObjectPtr<UUserGameSettingTracker> mSettingTracker;

private:
	bool bWasLoadedSavePlayer;
	bool bWasLoadedUserOptions;
};
