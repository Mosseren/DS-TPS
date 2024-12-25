#include "PauseWidget.h"
#include "Components/Button.h"
#include "MyPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include "GameFramework/PlayerController.h"

void UPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (RestartButton != nullptr)
	{
		RestartButton->OnClicked.AddDynamic(this, &UPauseWidget::OnRestartClicked);
	}
	if (ResumeButton != nullptr)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::OnResumeClicked);
	}
	if (ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddDynamic(this, &UPauseWidget::OnExitClicked);
	}
}

void UPauseWidget::OnRestartClicked()
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HidePauseWidget();
		UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
	}
}

void UPauseWidget::OnResumeClicked()
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HidePauseWidget();
	}
}

void UPauseWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
