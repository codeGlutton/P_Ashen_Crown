#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "GameFramework/SaveGame.h"
#include "P_Ashen_Crown/Component/InventoryComponent.h"
#include "SavePlayer.generated.h"

USTRUCT(BlueprintType)
struct P_ASHEN_CROWN_API FSaveItemKeyInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FName mName;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	EItemType mItemType;
};

/**
 * 플레이어 게임 데이터
 */
UCLASS()
class P_ASHEN_CROWN_API USavePlayer : public USaveGame
{
	GENERATED_BODY()
	
public:
	USavePlayer();

public:
	const FString& GetStageName() const;
	int32 GetLevel() const;
	FDateTime GetDate() const;
	float GetHP() const;
	const FVector& GetPosition() const;
	const FRotator& GetRotation() const;
	bool WasTriggeredEvent(const FName& Name) const;
	const TArray<FSaveItemKeyInfo>& GetEquipItemKeys() const;
	const TArray<FSaveItemKeyInfo>& GetInventoryItemKeys() const;
	const TSet<FName>& GetTriggeredEvents() const;

	void SetStageName(const FString& StageName);
	void SetLevel(int32 Level);
	void SetDate(FDateTime Date);
	void SetHP(float HP);
	void SetPosition(const FVector& Position);
	void SetRotation(const FRotator& Rotation);
	void SetTiggeredEvent(const FName& Name, bool bTriggered);
	void SetEquipItemKeys(const TArray<FSaveItemKeyInfo>& EquipItemKeys);
	void SetInventoryItemKeys(const TArray<FSaveItemKeyInfo>& InventoryItemKeys);
	void SetTiggeredEvents(const TSet<FName>& TriggeredEvents);

public:
	/**
	 * 객체 복제 함수
	 * @return 복제 생성된 객체
	 */
	USavePlayer* Clone() const;

protected:
	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FString mStageName;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	int32 mLevel;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FDateTime mDate;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	float mHP;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FVector mPosition;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	FRotator mRotation;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	TArray<FSaveItemKeyInfo> mEquipItemKeys;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	TArray<FSaveItemKeyInfo> mInventoryItemKeys;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadWrite)
	TSet<FName> mTriggeredEvents;
};
