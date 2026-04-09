#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "GameFramework/SaveGame.h"
#include "SaveSound.generated.h"

UENUM(BlueprintType)
enum class ESoundSettingType : uint8
{
	Master = 0 UMETA(DisplayName = "Master"),
	SFX UMETA(DisplayName = "SFX"),
	Voice UMETA(DisplayName = "Voice"),
	BGM UMETA(DisplayName = "BGM"),
	Count UMETA(DisplayName = "Count", Hidden)
};
DECLARE_ENUM_TO_STRING(ESoundSettingType);

/**
 * 사운드 세팅 데이터
 */
UCLASS()
class P_ASHEN_CROWN_API USaveSound : public USaveGame
{
	GENERATED_BODY()
	
public:
	USaveSound();

public:
	float GetVolume(ESoundSettingType SettingType) const;
	const TArray<float>& GetVolumes() const;

	void SetVolume(ESoundSettingType SettingType, float Volume);
	void SetVolumes(const TArray<float>& Volumes);

public:
	/**
	 * 객체 복제 함수
	 * @return 복제 생성된 객체
	 */
	USaveSound* Clone() const;

protected:
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	TArray<float> mVolumes;
};
