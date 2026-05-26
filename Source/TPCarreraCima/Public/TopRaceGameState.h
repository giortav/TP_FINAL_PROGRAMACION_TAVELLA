#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TopRaceGameState.generated.h"

class ATopRacePlayerState;

UENUM(BlueprintType)
enum class EMatchPhase : uint8
{
    WaitingToStart  UMETA(DisplayName = "Waiting to Start"),
    InProgress      UMETA(DisplayName = "In Progress"),
    Finished        UMETA(DisplayName = "Finished")
};

UCLASS()
class TPCARRERACIMA_API ATopRaceGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ATopRaceGameState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // --- Setters (llamados solo en servidor) ---
    void SetMatchPhase(EMatchPhase NewPhase);
    void SetTimeRemaining(float NewTime);
    void SetTotalCheckpoints(int32 Total);
    void UpdatePlayerRankings(const TArray<ATopRacePlayerState*>& SortedPlayers);

    // --- Getters (usados por UI en clientes) ---
    UFUNCTION(BlueprintCallable, Category = "Race")
    EMatchPhase GetMatchPhase() const { return MatchPhase; }

    UFUNCTION(BlueprintCallable, Category = "Race")
    float GetTimeRemaining() const { return TimeRemaining; }

    UFUNCTION(BlueprintCallable, Category = "Race")
    int32 GetTotalCheckpoints() const { return TotalCheckpoints; }

    UFUNCTION(BlueprintCallable, Category = "Race")
    const TArray<ATopRacePlayerState*>& GetPlayerRankings() const { return PlayerRankings; }

private:
    // Estado de la partida — replicado con RepNotify para actualizar HUD
    UPROPERTY(ReplicatedUsing = OnRep_MatchPhase)
    EMatchPhase MatchPhase = EMatchPhase::WaitingToStart;

    // Tiempo restante en segundos
    UPROPERTY(Replicated)
    float TimeRemaining = 0.f;

    // Total de checkpoints en el mapa
    UPROPERTY(Replicated)
    int32 TotalCheckpoints = 0;

    // Lista ordenada de jugadores por posicion
    UPROPERTY(Replicated)
    TArray<ATopRacePlayerState*> PlayerRankings;

    // RepNotify: se ejecuta en todos los clientes cuando MatchPhase cambia
    UFUNCTION()
    void OnRep_MatchPhase();
};
