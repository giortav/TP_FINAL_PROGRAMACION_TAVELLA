#include "Public/TopRacePlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Public/TopRaceCharacter.h"

ATopRacePlayerController::ATopRacePlayerController()
{
}

void ATopRacePlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Solo el cliente local crea su HUD
    if (IsLocalController())
    {
        ShowHUD();
    }
}


// ---------------------------------------------------------------
// Client RPCs
// ---------------------------------------------------------------

void ATopRacePlayerController::ClientOnMatchEnd_Implementation(int32 FinalPosition)
{
    // Se ejecuta en el cliente cuando el servidor termina la partida
    HideHUD();
    ShowVictoryScreen(FinalPosition);

    UE_LOG(LogTemp, Log, TEXT("PlayerController [ClientRPC]: Fin de partida, posicion %d"), FinalPosition);
}

void ATopRacePlayerController::ClientShowNotification_Implementation(const FString& Message)
{
    // Muestra un mensaje breve en pantalla (ej: "Checkpoint!")
    // Se puede conectar a un widget UMG con un texto animado
    UE_LOG(LogTemp, Log, TEXT("Notificacion: %s"), *Message);
}

// ---------------------------------------------------------------
// UI
// ---------------------------------------------------------------

void ATopRacePlayerController::ShowHUD()
{
    if (!HUDWidgetClass) return;

    HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
    if (HUDWidget)
    {
        HUDWidget->AddToViewport();
    }
}

void ATopRacePlayerController::HideHUD()
{
    if (HUDWidget)
    {
        HUDWidget->RemoveFromParent();
        HUDWidget = nullptr;
    }
}

void ATopRacePlayerController::ShowVictoryScreen(int32 Position)
{
    if (!EndScreenWidgetClass) return;

    EndScreenWidget = CreateWidget<UUserWidget>(this, EndScreenWidgetClass);
    if (EndScreenWidget)
    {
        EndScreenWidget->AddToViewport(10); // ZOrder alto para tapar el HUD
        // Pasar la posicion al widget via Binding o funcion
    }
}

// ---------------------------------------------------------------
// Input — delega al Character poseido
// ---------------------------------------------------------------

