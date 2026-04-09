// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttributeSet.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> BaseMetaData(TEXT("/Script/Engine.DataTable'/Game/AC/Data/Player/DT_BaseAttributeSet.DT_BaseAttributeSet'"));
    if (BaseMetaData.Succeeded())
    {
        UAttributeSet::InitFromMetaDataTable(BaseMetaData.Object);
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> PlayerMetaData(TEXT("/Script/Engine.DataTable'/Game/AC/Data/Player/DT_PlayerAttributeSet.DT_PlayerAttributeSet'"));
    if (PlayerMetaData.Succeeded())
    {
        UAttributeSet::InitFromMetaDataTable(PlayerMetaData.Object);
    }
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHPAttribute())
    {
        if (GetHP() <= 0.f)
        {
            SetHP(0.f);

            UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
            if (!ASC->HasMatchingGameplayTag(AC_PlayerState::Player_State_Death.GetTag()))
            {
                FGameplayTagContainer AllTag = ASC->GetOwnedGameplayTags();
                ASC->RemoveLooseGameplayTags(AllTag);
                ASC->AddLooseGameplayTag(AC_PlayerState::Player_State_Death.GetTag());
            }
            UE_LOG(LogTemp, Warning, TEXT("사망"));
        }
        else
        {
            const float Delta = Data.EvaluatedData.Magnitude;
            if (Delta > 0.f)
            {
                if (GetHP() > GetMaxHP())
                {
                    SetHP(FMath::Clamp(GetHP(), 0.0f, GetMaxHP()));
                    UE_LOG(LogTemp, Warning, TEXT("최대회복"));
                }
            }
            else if (Delta < 0.f)
            {
                UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
                if (!ASC->HasMatchingGameplayTag(AC_PlayerState::Player_State_Hit.GetTag()))
                {
                    ASC->AddLooseGameplayTag(AC_PlayerState::Player_State_Hit.GetTag());
                }
                UE_LOG(LogTemp, Warning, TEXT("피격"));
            }
        }
    }
}
