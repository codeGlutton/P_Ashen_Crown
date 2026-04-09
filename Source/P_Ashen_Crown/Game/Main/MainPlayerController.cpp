#include "MainPlayerController.h"
#include "P_Ashen_Crown/Component/CameraTargetingBlendComponent.h"
#include "Components/Widget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "EnhancedInputSubsystems.h"
#include "P_Ashen_Crown/Character/Player/Input/PlayerInputData.h"

AMainPlayerController::AMainPlayerController()
{
	mCameraTargetingBlendComponent = CreateDefaultSubobject<UCameraTargetingBlendComponent>(TEXT("CameraTargetingBlendComponent"));
	bShowMouseCursor = false;
	SetTickableWhenPaused(true);
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ClearUIOnlyModeCount();
}

void AMainPlayerController::DecreaseUIOnlyModeCount()
{
	--mUIOnlyModeCount;
	if (mUIOnlyModeCount == 0)
	{
		SetGameOnlyMode();
	}
}

void AMainPlayerController::IncreaseUIOnlyModeCount(UWidget* FocusWidget)
{
	if (mUIOnlyModeCount == 0)
	{
		SetUIOnlyMode(FocusWidget);
	}
	else
	{
		FEventReply Reply;
		UWidgetBlueprintLibrary::SetUserFocus(Reply, FocusWidget);
	}
	++mUIOnlyModeCount;
}

void AMainPlayerController::ClearUIOnlyModeCount()
{
	mUIOnlyModeCount = 0;
	SetGameOnlyMode();
}

void AMainPlayerController::SetUIOnlyMode(UWidget* FocusWidget)
{
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, FocusWidget);
	auto* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(InputSubsystem) == true)
	{
		const auto* InputDataCDO = GetDefault<UPlayerInputData>();
		if (IsValid(InputDataCDO) == true)
		{
			InputSubsystem->RemoveMappingContext(InputDataCDO->GetInputMappingContext());
		}
	}
	SetShowMouseCursor(true);
}

void AMainPlayerController::SetGameOnlyMode()
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	auto* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(InputSubsystem) == true)
	{
		const auto* InputDataCDO = GetDefault<UPlayerInputData>();
		if (IsValid(InputDataCDO) == true && InputSubsystem->HasMappingContext(InputDataCDO->GetInputMappingContext()) == false)
		{
			InputSubsystem->AddMappingContext(InputDataCDO->GetInputMappingContext(), 1);
		}
	}
	SetShowMouseCursor(false);
}
