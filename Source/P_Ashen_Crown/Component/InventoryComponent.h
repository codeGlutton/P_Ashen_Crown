#pragma once

#include "P_Ashen_Crown/GameCore.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

/************************
   IInventoryViewHandler
*************************/

class UInventoryComponent;
class UGameplayEffect;

UINTERFACE(MinimalAPI)
class UInventoryViewHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 인벤토리 View와 연결해주는 객체 인터페이스
 */
class P_ASHEN_CROWN_API IInventoryViewHandler
{
	GENERATED_BODY()

public:
	/**
	 * 인벤토리 컴포넌트와 초기 연결
	 * @param InventoryComponent 대상 컴포넌트
	 */
	virtual void ConnectToInventory(UInventoryComponent* const InventoryComponent) = 0;
	/**
	 * 인벤토리 View 열기
	 */
	virtual void ShowView() = 0;
	/**
	 * 인벤토리 View 닫기
	 */
	virtual void CloseView() = 0;
};

/************************
   UInventoryComponent
*************************/

class UUserWidget;

/**
 * 아이템 타입
 */
UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0,
	Equip,
	Potion,
	Rune,
};

/**
 * 장비 타입
 */
UENUM(BlueprintType)
enum class EEquipType : uint8
{
	Weapon,
	Armor,
	Count UMETA(Hidden)
};

/**
 * 게임 아이템 정보
 */
USTRUCT(BlueprintType)
struct FGameItemInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	/**
	 * 장비할 수 있는 아이템 인지 여부 확인 함수
	 * @return 확인 결과
	 */
	bool CanEquip() const;
	/**
	 * 아이템에 의한 능력치 효과 설명문 반환 함수
	 * @return 능력치 효과 설명문
	 */
	virtual const FText& GetAbilityText() const { return mAbilityText; };

public:
	UPROPERTY(EditAnywhere, Category = "GameItemInfo")
	FName mKeyName;
	UPROPERTY(EditAnywhere, Category = "GameItemInfo")
	EItemType mType;
	UPROPERTY(EditAnywhere, Category = "GameItemInfo")
	TObjectPtr<UTexture2D> mTexture;
	UPROPERTY(EditAnywhere, Category = "GameItemInfo")
	FText mName;
	UPROPERTY(EditAnywhere, Category = "GameItemInfo")
	FText mDesc;

	mutable FText mAbilityText;
};

USTRUCT(BlueprintType)
struct FGameEquipInfo : public FGameItemInfo
{
	GENERATED_BODY()

public:
	virtual const FText& GetAbilityText() const override;

public:
	UPROPERTY(EditAnywhere, Category = "GameEquipInfo")
	EEquipType mEquipType;
	UPROPERTY(EditAnywhere, Category = "GameEquipInfo")
	float mAttackPower;
	UPROPERTY(EditAnywhere, Category = "GameEquipInfo")
	float mDefense;
	UPROPERTY(EditAnywhere, Category = "GameEquipInfo")
	float mMoveSpeed;
	UPROPERTY(EditAnywhere, Category = "GameEquipInfo")
	float mStamina;
	UPROPERTY(EditAnywhere, Category = "GameEquipInfo")
	float mHP;
	UPROPERTY(EditAnywhere, Category = "GameEquipInfo")
	TSubclassOf<UGameplayEffect> mEquipItemStatusEffect;
};

USTRUCT(BlueprintType)
struct FGamePotionInfo : public FGameItemInfo
{
	GENERATED_BODY()

public:
	virtual const FText& GetAbilityText() const override;

public:
	UPROPERTY(EditAnywhere, Category = "GamePotionInfo")
	float mHealAmount;
};

USTRUCT(BlueprintType)
struct FGameRuneInfo : public FGameItemInfo
{
	GENERATED_BODY()

public:
	virtual const FText& GetAbilityText() const override;

public:
	UPROPERTY(EditAnywhere, Category = "GameRuneInfo")
	float mAttackPowerPercent;
	UPROPERTY(EditAnywhere, Category = "GameRuneInfo")
	float mDefensePercent;
	UPROPERTY(EditAnywhere, Category = "GameRuneInfo")
	float mMoveSpeedPercent;
	UPROPERTY(EditAnywhere, Category = "GameRuneInfo")
	float mStaminaPercent;
	UPROPERTY(EditAnywhere, Category = "GameRuneInfo")
	float mHPPercent;
};

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnEquipItem, int32 /* EquipIndex */, TSharedPtr<const FGameItemInfo> /* ItemInfo */, TSharedPtr<const FGameItemInfo> /* PreItemInfo */);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnUpdateInventoryItem, int32 /* InventoryIndex */, TSharedPtr<const FGameItemInfo> /* ItemInfo */, TSharedPtr<const FGameItemInfo> /* PreItemInfo */);

/**
 * 인벤토리 컴포넌트
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P_ASHEN_CROWN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	
public:
	/**
	 * 위젯과 데이터 전송을 위한 매개체 handler를 초기에 등록하는 함수
	 * @param Handler 데이터를 주고받을 Handler 인터페이스
	 * @param ToolTipWidget 인벤토리에 사용될 툴팁 위젯
	 */
	void RegisterInventoryView(IInventoryViewHandler* Handler, UUserWidget* ToolTipWidget);
	/**
	 * Handler를 제거. 만약 뷰포트에 표기 시, 자동으로 내림
	 */
	void UnregisterInventoryView();

	/**
	 * 해당 인덱스에 장비 지정
	 * @param EquipIndex 장비 인덱스
	 * @param Item 대상 아이템
	 */
	void LoadEquipItem(int32 EquipIndex, TSharedPtr<FGameItemInfo> Item);
	/**
	 * 해당 인덱스에 아이템 지정
	 * @param InventoryIndex 장비 인덱스
	 * @param Item 대상 아이템
	 */
	void LoadInventoryItem(int32 InventoryIndex, TSharedPtr<FGameItemInfo> Item);

public:
	/**
	 * 인벤토리 View를 여는 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "InventoryComponent")
	void OpenInventory();
	/**
	 * 인벤토리 View를 닫는 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "InventoryComponent")
	void CloseInventory();
	/**
	 * 인벤토리 View를 토글하는 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "InventoryComponent")
	void ToggleInventory();

public:
	const TArray<TSharedPtr<FGameItemInfo>>& GetInventoryItems() const;
	const TArray<TSharedPtr<FGameItemInfo>>& GetEquipItems() const;
	UUserWidget* GetToolTipWidget() const;

	/**
	 * 인벤토리가 가득 찼는지 여부를 알려주는 함수
	 * @return 가득 찼는지 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "InventoryComponent")
	bool IsInventoryFull() const;

public:
	/**
	 * 아이템 인벤토리에 추가 함수
	 * @param NewItem 새로운 아이템 정보
	 * @return 성공 여부
	 */
	bool AddItem(TSharedPtr<FGameItemInfo> NewItem);
	template<typename T>
	bool AddItem(const T& NewItem);
	UFUNCTION(BlueprintCallable, Category = "InventoryComponent") // TODO : 테스트용도. 삭제!!!
	void AddTestEquipItem(UDataTable* Table, const FName& Name);
	/**
	 * 아이템 이벤토리에서 삭제 함수
	 * @param InvenotryIndex 제거될 아이템 인덱스
	 * @return 성공 여부
	 */
	bool RemoveInventoryItem(int32 InvenotryIndex);
	bool RemoveEquipItem(int32 EquipIndex);
	bool RemoveItem(TSharedPtr<const FGameItemInfo>& TargetItem);
	/**
	 * 아이템 위치를 서로 교환하는 함수
	 * @param LhsInventoryIndex 왼쪽 피연산 인벤토리 인덱스
	 * @param RhsInventoryIndex 오른쪽 피연산 인벤토리 인덱스
	 * @return 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "InventoryComponent")
	bool SwapItems(int32 LhsInventoryIndex, int32 RhsInventoryIndex);

public:
	/**
	 * 아이템을 장착 혹은 해제하는 함수
	 * @param EquipIndex 장비칸 인덱스
	 * @param InventoryIndex 인벤토리칸 인덱스
	 * @return 성공 여부
	 */
	bool EquipItem(int32 EquipIndex, int32 InventoryIndex);

public:
	// 장비창에 변환점이 생겼을 때 호출되는 대리자
	FOnEquipItem OnChangeEquipItem;
	// 장비창 아이템을 강화했을 때 호출되는 대리자
	FOnEquipItem OnUpgradeEquipItem;
	// 인벤토리에 변화점이 생겼을 때 호출되는 대리자
	FOnUpdateInventoryItem OnUpdateInventoryItem;

protected:
	UPROPERTY()
	TScriptInterface<IInventoryViewHandler> mInventoryHandler;

	// 인벤토리 View에서 공용으로 사용할 툴팁 위젯
	UPROPERTY()
	TObjectPtr<UUserWidget> mToolTipWidget;

protected:
	static const int32 mMaxItemCount = 40;
	int32 mCurItemCount;
	TArray<TSharedPtr<FGameItemInfo>> mInventoryItems;
	TArray<TSharedPtr<FGameItemInfo>> mEquipItems;

	bool bIsOpen;
};

template<typename T>
bool UInventoryComponent::AddItem(const T& NewItem)
{
	static_assert(TIsDerivedFrom<T, FGameItemInfo>::IsDerived, "T must derive from FGameItemInfo");
	TSharedPtr<T> NewItemPtr = MakeShared<T>(NewItem);
	return AddItem(StaticCastSharedPtr<FGameItemInfo>(NewItemPtr));
}
