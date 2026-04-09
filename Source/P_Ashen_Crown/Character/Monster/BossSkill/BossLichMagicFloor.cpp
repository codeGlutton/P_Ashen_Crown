// Fill out your copyright notice in the Description page of Project Settings.


#include "BossLichMagicFloor.h"
#include "AbilitySystemGlobals.h"
#include "../MonsterBase.h"
#include "../BossMonster/BossLich.h"

// Sets default values
ABossLichMagicFloor::ABossLichMagicFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mBody = CreateDefaultSubobject<USphereComponent>(TEXT("Body"));
	mNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));

    static ConstructorHelpers::FClassFinder<UGameplayEffect>
        DamageEffectBP(TEXT("/Script/Engine.Blueprint'/Game/AC/Blueprint/GAS/Effect/BP_BossLichSkill2.BP_BossLichSkill2_C'"));

    if (DamageEffectBP.Succeeded())
    {
        mDamageEffect = DamageEffectBP.Class;
    }

	SetRootComponent(mBody);
	mNiagaraComp->SetupAttachment(mBody);
	mNiagaraComp->SetAutoActivate(true);

    mBody->SetCollisionProfileName(TEXT("MonsterProjectile"));

    /*static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
        NiagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/AC/SourceArt/VFX/Niagara/BloodVFX/NS_BloodPool.NS_BloodPool'"));

    if (NiagaraAsset.Succeeded())
    {
        mHitNiagara = NiagaraAsset.Object;
        mNiagaraComp->SetAsset(mHitNiagara);
    }*/
}

// Called when the game starts or when spawned
void ABossLichMagicFloor::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("MagicFloor BeginPlay 호출됨"));

    ABossLich* Boss = Cast<ABossLich>(GetOwner());
    BindToBoss(Boss);
}

void ABossLichMagicFloor::OnDamage()
{
    UE_LOG(LogTemp, Warning, TEXT("MagicFloor OnDamage 호출됨"));

    TArray<FHitResult> HitArray = {};
    FCollisionQueryParams param = {};

    FVector SweepStart = GetActorLocation();
    //FVector SweepEnd = SweepStart + GetActorForwardVector() + mDamageRadius * 2;

    // Trace 채널로 임시 충돌체 생성
    bool Collision = GetWorld()->SweepMultiByChannel
    (
        HitArray,
        SweepStart,
        SweepStart,
        FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel4,
        FCollisionShape::MakeSphere(mDamageRadius),
        param
    );

    if (Collision)
    {
        for (FHitResult& Result : HitArray)
        {
            AActor* Target = Result.GetActor();
            if (IsValid(Target))
            {
                // CollisionProfileName 체크
                UPrimitiveComponent* HitComp = Cast<UPrimitiveComponent>(Result.Component.Get());
                if (!HitComp || HitComp->GetCollisionProfileName() != FName(TEXT("Player")))
                {
                    continue;
                }

                // ASC 찾기
                UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
                if (IsValid(TargetASC) && mDamageEffect)
                {
                    FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
                    EffectContext.AddSourceObject(this);
                    EffectContext.AddHitResult(Result);

                    FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(mDamageEffect, 1.0f, EffectContext);
                    if (SpecHandle.IsValid())
                    {
                        // SetByCaller로 데미지 지정
                        SpecHandle.Data->SetSetByCallerMagnitude(
                            FGameplayTag::RequestGameplayTag(TEXT("Monster.Data.Damage")),
                            -mFloorDamage);

                        TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
                        UE_LOG(LogTemp, Warning, TEXT("MagicFloor 데미지 적용 성공: %s"), *Target->GetName());
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("타겟 ASC 또는 DamageEffect가 없습니다!"));
                }
            }
        }
    }

    DrawDebugSphere(GetWorld(), GetActorLocation(), mDamageRadius, 20, FColor::Red, false, 2.0f);

    mBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    mNiagaraComp->Activate();
    mNiagaraComp->Deactivate();

    //SetLifeSpan(4.f);
}
 
void ABossLichMagicFloor::BindToBoss(ABossLich* Boss)
{
    if (IsValid(Boss))
    {
        Boss->OnActorDestroy.AddDynamic(this, &ABossLichMagicFloor::OnActorDestroyed);
    }
}

void ABossLichMagicFloor::OnActorDestroyed(ABossLichMagicFloor* MagicFloor)
{
    mNiagaraComp->Deactivate();
}
