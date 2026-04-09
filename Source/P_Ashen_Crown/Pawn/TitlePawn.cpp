#include "TitlePawn.h"

#include "EnhancedInputSubsystems.h"
#include "P_Ashen_Crown/Singleton/Subsystem/SaveGameSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

ATitlePawn::ATitlePawn()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATitlePawn::BeginPlay()
{
	Super::BeginPlay();
	
	auto* PC = GetController<APlayerController>();
	if (IsValid(PC) == false)
	{
		return;
	}
	PC->SetShowMouseCursor(true);

	auto* EnhancedInputSubsystem = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (IsValid(EnhancedInputSubsystem) == false)
	{
		return;
	}
	FModifyContextOptions ContextOptions;
	ContextOptions.bNotifyUserSettings = true;
	EnhancedInputSubsystem->AddMappingContext(mSettingInputContext, 0, ContextOptions);

	auto* SaveGameSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	if (IsValid(SaveGameSubSystem) == false)
	{
		return;
	}
	SaveGameSubSystem->LoadAllPlayers();
	SaveGameSubSystem->LoadOptionSetting();

	UUserWidget* TitleWidget = CreateWidget(GetWorld(), mTitleWidgetClass);
	if (IsValid(TitleWidget) == false)
	{
		return;
	}
	TitleWidget->AddToViewport();
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, TitleWidget, EMouseLockMode::LockAlways);
}

