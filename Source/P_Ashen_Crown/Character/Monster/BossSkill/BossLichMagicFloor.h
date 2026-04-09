// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GAS/GASCore.h"
#include "GameFramework/Actor.h"
#include "BossLichMagicFloor.generated.h"

UCLASS()
class P_ASHEN_CROWN_API ABossLichMagicFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossLichMagicFloor();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> mBody;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> mNiagaraComp;

	UPROPERTY()
	TObjectPtr<AController> mOwnerController;

	UPROPERTY()
	TObjectPtr<class AMonsterBase> mOwnerMonster;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> mDamageEffect;

	float mDamageRadius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterSkill")
	float mFloorDamage = 120.f;

public:
	void SetOwnerController(AController* Controller)
	{
		mOwnerController = Controller;
	}

	void SetDamageRadius(float DamageRadius)
	{
		mDamageRadius = DamageRadius;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void OnDamage();

	void BindToBoss(class ABossLich* Boss);
	
	UFUNCTION()
	void OnActorDestroyed(ABossLichMagicFloor* MagicFloor);
};
