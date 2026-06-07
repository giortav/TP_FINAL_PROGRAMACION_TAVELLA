#include "Public/TopRacePlayerController.h"

#include "TopRaceResultWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameStateBase.h"
#include "Public/TopRaceCharacter.h"

class UTopRaceResultWidget;

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
    UE_LOG(LogTemp, Log, TEXT("ShowVictoryScreen llamado, posicion: %d"), Position);

    if (!EndScreenWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("EndScreenWidgetClass es NULL!"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("EndScreenWidgetClass OK, creando widget..."));

    EndScreenWidget = CreateWidget<UUserWidget>(this, EndScreenWidgetClass);
    if (!EndScreenWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("CreateWidget fallo, EndScreenWidget es NULL!"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Widget creado OK, haciendo Cast..."));

    UTopRaceResultWidget* ResultWidget = Cast<UTopRaceResultWidget>(EndScreenWidget);
    if (!ResultWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cast a TopRaceResultWidget fallo!"));
    }
    else
    {
        int32 TotalPlayers = GetWorld()->GetGameState()->PlayerArray.Num();
        UE_LOG(LogTemp, Log, TEXT("SetupResult con posicion %d, total jugadores %d"), Position, TotalPlayers);
        ResultWidget->SetupResult(Position, TotalPlayers);
    }

    EndScreenWidget->AddToViewport(10);
    UE_LOG(LogTemp, Log, TEXT("Widget agregado al viewport!"));
}
// ---------------------------------------------------------------
// Input — delega al Character poseido
// ---------------------------------------------------------------

