#pragma once

#include "CoreMinimal.h"
#include "TPCarreraCimaPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "TopRacePlayerController.generated.h"

class UTopRaceHUD;

UCLASS()
class TPCARRERACIMA_API ATopRacePlayerController : public ATPCarreraCimaPlayerController
{
    GENERATED_BODY()

public:
    ATopRacePlayerController();

    virtual void BeginPlay() override;

    // -------------------------------------------------------
    // Client RPCs — servidor llama, ejecutan en el cliente
    // -------------------------------------------------------

    // Muestra la pantalla de fin de carrera con la posicion final
    UFUNCTION(Client, Reliable)
    void ClientOnMatchEnd(int32 FinalPosition);

    // Muestra un mensaje en pantalla (notificaciones)
    UFUNCTION(Client, Unreliable)
    void ClientShowNotification(const FString& Message);

    // -------------------------------------------------------
    // UI helpers — usados por widgets
    // -------------------------------------------------------
    void ShowHUD();
    void HideHUD();
    void ShowVictoryScreen(int32 Position);

protected:
    // Clase del widget HUD principal
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> HUDWidgetClass;

    // Clase del widget de victoria/derrota
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> EndScreenWidgetClass;

private:
    // Referencia al HUD instanciado
    UPROPERTY()
    UUserWidget* HUDWidget = nullptr;

    // Referencia a la pantalla de fin
    UPROPERTY()
    UUserWidget* EndScreenWidget = nullptr;
    
};
