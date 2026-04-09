#include "MainGameMode.h"
#include "P_Ashen_Crown/Singleton/Subsystem/GameEventSubsystem.h"
#include "P_Ashen_Crown/Singleton/Subsystem/SaveGameSubsystem.h"
#include "P_Ashen_Crown/SaveGame/SavePlayer.h"

AMainGameMode::AMainGameMode()
{
}

APawn* AMainGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	// 로드할 트랜스폼
	FTransform LoadedSpawnTransform = SpawnTransform;

	auto* SaveGameSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	TObjectPtr<USavePlayer> SavePlayer = SaveGameSubsystem->GetCurSavePlayer();
	if (IsValid(SavePlayer) == true)
	{
		/* 처음 이 게임을 시작하는지 확인 */

		bool bAlreadyStartGame = SavePlayer->WasTriggeredEvent(TEXT("StartGame"));
		if (bAlreadyStartGame == true)
		{
			/* 저장된 트랜스폼 로드 */

			LoadedSpawnTransform.SetLocation(SavePlayer->GetPosition());
			LoadedSpawnTransform.SetRotation(SavePlayer->GetRotation().Quaternion());
		}
	}

	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, LoadedSpawnTransform);
}
