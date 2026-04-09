// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GAS/GASCore.h"
#include "GameFramework/Actor.h"
#include "BossLichProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class P_ASHEN_CROWN_API ABossLichProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossLichProjectile();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> mBody;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> mNiagaraComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> mMovement;

	UPROPERTY()
	TObjectPtr<AController> mOwnerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> mDamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterSkill")
	float mSpellDamage = 150.f;

public:
	UPROPERTY()
	TObjectPtr<class AMonsterBase> mOwnerMonster;

	void SetOwnerController(AController* Controller)
	{
		mOwnerController = Controller;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/**
	* 투사체가 다른 오브젝트와 충돌했을 때 호출되는 함수
	* @param HitComp 충돌한 투사체의 컴포넌트
	* @param OtherActor 충돌한 상대 액터
	* @param OtherComp 상대 액터의 충돌한 컴포넌트
	* @param NormalImpulse 충돌 시의 힘
	* @param Hit 충돌에 대한 정보
	*/
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
