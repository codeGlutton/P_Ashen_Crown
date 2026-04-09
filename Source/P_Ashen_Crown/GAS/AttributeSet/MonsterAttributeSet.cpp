// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAttributeSet.h"
#include "../../Character/Monster/MonsterBase.h"

UMonsterAttributeSet::UMonsterAttributeSet()
{
	//static ConstructorHelpers::FObjectFinder<UDataTable> 
	//	MonsterData(TEXT("/Script/Engine.DataTable'/Game/AC/Data/Monster/DT_MonsterAttriuteSet1.DT_MonsterAttriuteSet'"));

	/*if (MonsterData.Succeeded())
	{
		InitFromMetaDataTable(MonsterData.Object);

		UE_LOG(LogTemp, Warning, TEXT("Monster AttackPower : %f"), GetAttackPower());
		UE_LOG(LogTemp, Warning, TEXT("Monster HP : %f"), GetHP());
		UE_LOG(LogTemp, Warning, TEXT("Monster Defense : %f"), GetDefense());
		UE_LOG(LogTemp, Warning, TEXT("Monster MoveSpeed : %f"), GetMoveSpeed());
		UE_LOG(LogTemp, Warning, TEXT("Monster AttackDistance : %f"), GetAttackDistance());

	}*/
}

void UMonsterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	AMonsterBase* Monster = Cast<AMonsterBase>(Data.Target.GetOwnerActor());
	AActor* Player = Data.EffectSpec.GetEffectContext().GetOriginalInstigator();

	const FHitResult* HitResult = Data.EffectSpec.GetEffectContext().GetHitResult();

	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		float CurrentHP = GetHP();

		if (!IsValid(Monster)) return;

		if (CurrentHP <= 0.f)
		{
			// 사망처리
			Monster->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Monster.State.Death")));
		}
		else if (CurrentHP > 0.f)
		{
			//Monster->OnDamageTaken(HitResult);
			Monster->OnDamageTaken(Player, HitResult);
		}

		UE_LOG(LogTemp, Warning, TEXT("Monster HP : %f"), CurrentHP);
	}
	
}

