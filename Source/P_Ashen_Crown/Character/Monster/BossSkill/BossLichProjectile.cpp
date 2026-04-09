// Fill out your copyright notice in the Description page of Project Settings.


#include "BossLichProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemGlobals.h"
#include "../../Player/PlayerCharacter.h"
#include "../MonsterBase.h"

// Sets default values
ABossLichProjectile::ABossLichProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	mBody = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	mNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	mMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

	static ConstructorHelpers::FClassFinder<UGameplayEffect> 
		DamageEffectBP(TEXT("/Script/Engine.Blueprint'/Game/AC/Blueprint/GAS/Effect/BP_BossLichSkill1.BP_BossLichSkill1_C'"));

	if (DamageEffectBP.Succeeded())
	{
		mDamageEffect = DamageEffectBP.Class;
	}

	SetRootComponent(mBody);
	mNiagaraComp->SetupAttachment(mBody);
	mMovement->SetUpdatedComponent(mBody);
	mNiagaraComp->SetAutoActivate(true);

	mMovement->InitialSpeed = 2000.f;
	mMovement->ProjectileGravityScale = 0.f;

	mBody->SetBoxExtent(FVector(60.0, 10.0, 10.0));
	mBody->SetCollisionProfileName(TEXT("MonsterProjectile"));

}

void ABossLichProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	mMovement->Velocity = GetActorForwardVector() * mMovement->InitialSpeed;
	mBody->OnComponentHit.AddDynamic(this, &ABossLichProjectile::OnProjectileHit);

	SetLifeSpan(5.f);
}

void ABossLichProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossLichProjectile::OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 자신과 충돌한 경우 무시
	if (!IsValid(OtherActor) || OtherActor == GetOwner())
		return;

	mBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!mDamageEffect)
	{
		UE_LOG(LogTemp, Error, TEXT("mDamageEffect가 비어 있습니다."));
		return;
	}

	/*if (OtherComp->GetCollisionProfileName() != FName(TEXT("Player")))
	{
		UE_LOG(LogTemp, Warning, TEXT("충돌한 대상이 Player가 아닙 : %s"), *OtherComp->GetCollisionProfileName().ToString());
		return;
	}*/

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (!TargetASC)
	{
		UE_LOG(LogTemp, Error, TEXT("타겟 ASC가 없음: %s"), *OtherActor->GetName());
		return;
	}

	// 이펙트 컨텍스트 생성
	FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
	EffectContext.AddHitResult(Hit);
	EffectContext.AddSourceObject(this);

	// Spec 생성 및 적용
	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(mDamageEffect, 1.0f, EffectContext);
	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SpecHandle 생성 실패"));
		return;
	}

	if (SpecHandle.IsValid())
	{
		// SetByCaller 데미지 값 설정
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Monster.Data.Damage")),-mSpellDamage);

		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		UE_LOG(LogTemp, Warning, TEXT("Projectile 데미지 적용 성공: %s"), *OtherActor->GetName());
	}

	// 나이아가라 꺼주기
	mNiagaraComp->Deactivate();
	mNiagaraComp->SetAutoDestroy(true);

}