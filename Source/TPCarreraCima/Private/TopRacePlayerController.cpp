#include "Public/TopRacePlayerController.h"
#include "TopRaceResultWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameStateBase.h"
#include "Public/TopRaceCharacter.h"

// ANTES: forward declaration de UTopRaceResultWidget redundante (ya cubierta por el #include).
// ANTES: #include de GameFramework/GameStateBase.h innecesario en este .cpp.

ATopRacePlayerController::ATopRacePlayerController()
{
}

void ATopRacePlayerController::BeginPlay()
{
    Super::BeginPlay();

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
    HideHUD();
    ShowVictoryScreen(FinalPosition);
}

void ATopRacePlayerController::ClientShowNotification_Implementation(const FString& Message)
{
    // Conectar a un widget UMG con texto animado cuando esté disponible.
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

    // ANTES: EndScreenWidgetClass era TSubclassOf<UUserWidget>, lo que obligaba
    // a un Cast<UTopRaceResultWidget> posterior que podía fallar silenciosamente.
    // AHORA: asumimos que el .h declara EndScreenWidgetClass como
    // TSubclassOf<UTopRaceResultWidget>, lo que elimina el cast por completo
    // y garantiza en tiempo de edición que solo se asigne la clase correcta.
    UTopRaceResultWidget* ResultWidget = CreateWidget<UTopRaceResultWidget>(this, EndScreenWidgetClass);
    if (!ResultWidget) return;

    const int32 TotalPlayers = GetWorld()->GetGameState()->PlayerArray.Num();
    ResultWidget->SetupResult(Position, TotalPlayers);
    ResultWidget->AddToViewport(10);

    EndScreenWidget = ResultWidget;
}
