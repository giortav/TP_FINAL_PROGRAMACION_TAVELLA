#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopRaceHUDWidget.generated.h"

class ATopRacePlayerState;
class ATopRaceGameState;

UCLASS()
class TOPRACE_API UTopRaceHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // Actualiza todos los elementos del HUD
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void RefreshHUD();

protected:
    // --- Text Blocks a bindear en el Widget Blueprint ---

    // Muestra la posicion en carrera: "1°", "2°", etc.
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TextPosition;

    // Muestra el checkpoint actual: "Checkpoint: 2/5"
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TextCheckpoint;

    // Muestra el tiempo restante: "2:45"
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TextTime;

    // Muestra el estado del jugador: "Corriendo", "Respawneando"
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TextStatus;

private:
    // Referencias cacheadas para no buscarlas cada tick
    UPROPERTY()
    ATopRacePlayerState* CachedPlayerState = nullptr;

    UPROPERTY()
    ATopRaceGameState* CachedGameState = nullptr;

    void CacheReferences();
    FText FormatTime(float Seconds) const;
    FText FormatPosition(int32 Position) const;
};