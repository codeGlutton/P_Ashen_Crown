// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GAS/GASCore.h"
#include "GameFramework/Actor.h"
#include "SwordProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class P_ASHEN_CROWN_API ASwordProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	ASwordProjectile();
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	/**
	 * 투사체 방향관련 초기화 내용
	 * @param Direction 방향
	 */
	void InitDirection(const FVector& Direction);

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

	UPROPERTY(EditAnywhere, Category = "Collision")
	TObjectPtr<UBoxComponent> mTriggerBox;
	
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	TObjectPtr<UProjectileMovementComponent> mProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> mDamageEffectClass;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	TObjectPtr<UNiagaraComponent> mNiagaraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TObjectPtr<UNiagaraSystem> mSlashEffectSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<UNiagaraSystem> mHitNiagara;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<USoundBase> mHitSound;

private:
	FVector mMoveDirection;

	UPROPERTY(EditAnywhere, Category = "VFX")
	float mMoveSpeed = 1200.f;

	void DeactivateEffect();
	
public:
	UPROPERTY()
	AActor* mInstigatorActor;

	UPROPERTY()
	AActor* mInstigatorController;
};
