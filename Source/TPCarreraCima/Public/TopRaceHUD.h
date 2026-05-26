#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopRaceHUD.generated.h"

/**
 * HUD base — la UI principal se implementa en UMG (Widgets Blueprint).
 * Esta clase sirve como puente entre el PlayerController y los widgets.
 *
 * Widgets a crear en Blueprint/UMG:
 *   - WBP_RaceHUD        : tiempo, posicion, checkpoints
 *   - WBP_VictoryScreen  : resultado final con posicion
 *   - WBP_Countdown      : cuenta regresiva al inicio
 *
 * Cada widget bindea sus textos directamente al PlayerState y GameState
 * usando las funciones GetCurrentCheckpoint(), GetTimeRemaining(), etc.
 */
UCLASS()
class TPCARRERACIMA_API ATopRaceHUD : public AHUD
{
    GENERATED_BODY()

public:
    ATopRaceHUD();

    virtual void BeginPlay() override;

    // Dibuja debug info en pantalla si está habilitado
    virtual void DrawHUD() override;

protected:
    // Mostrar informacion de debug en desarrollo
    UPROPERTY(EditDefaultsOnly, Category = "Debug")
    bool bShowDebugInfoHud = true;

private:
    void DrawDebugRaceInfo();
};
