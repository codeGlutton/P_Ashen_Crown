// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GAS/GASCore.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "P_Ashen_Crown/Setting/TeamSettings.h"
#include "BaseCharacter.generated.h"

class UBaseAttributeSet;

/**
* 플레이어, 몬스터의 부모가 되는 클래스, IAbilitySystemInterface, IGenericTeamAgentInterface 포함
* 반드시 SetupSightConfig함수를 정의해주어야 한다.
*/
UCLASS()
class P_ASHEN_CROWN_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UAIPerceptionStimuliSourceComponent>   mStimuliSourceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeamID")
	ETeamType										mTeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FText											mDisplayName;


protected:
	virtual void BeginPlay() override;

public:	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	void SetGenericTeamId(ETeamType TeamID);
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual void SetDisplayName(const FText& DisplayName);
	const FText& GetDisplayName() const;

	/// <summary>
	/// 상대방의 AttributeSet정보를 얻어오는 함수
	/// </summary>
	/// <typeparam name="T"> 반환받고 싶은 AttributeSet을 지닌 클래스 </typeparam>
	/// <returns> Class의 AttributeSet을 반환합니다. </returns>
	template <typename T>
	T* GetAttributeSet();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UAbilitySystemComponent>				mASC;
};

template <typename T>
T* ABaseCharacter::GetAttributeSet()
{
	if (UAbilitySystemComponent* ASC = this->GetAbilitySystemComponent())
	{
		for (UAttributeSet* Set : ASC->GetSpawnedAttributes())
		{
			if (T* TargetSet = Cast<T>(Set))
			{
				return TargetSet;
			}
		}
	}
	return nullptr;
}
