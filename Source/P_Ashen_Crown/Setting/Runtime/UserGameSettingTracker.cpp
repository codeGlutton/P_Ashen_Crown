#include "UserGameSettingTracker.h"

void UUserGameSettingTracker::SaveSettings()
{
	for (auto& CommandRow : mCommandRows)
	{
		// 각 옵션 설정 클래스에 따라 저장
		auto* LinkerPtr = mLinkers.Find(CommandRow.Key);
		if (LinkerPtr != nullptr)
		{
			auto* LinkerObj = Cast<UUserSettingLinker>(*LinkerPtr);
			if (LinkerObj != nullptr)
			{
				LinkerObj->SaveSettings();
			}
		}

		// 완전히 저장되었기 때문에, 커맨드의 기본값을 현재값으로 수정
		for (auto& Command : CommandRow.Value.mCommands)
		{
			Command.Value->CommitCommand();
		}
	}
}

void UUserGameSettingTracker::CancelSettings()
{
	for (auto& CommandRow : mCommandRows)
	{
		// 모든 커맨드 롤백 시도
		for (auto& Command : CommandRow.Value.mCommands)
		{
			Command.Value->RollbackCommand();
		}
	}
	OnCancelSetting.Broadcast();
}
