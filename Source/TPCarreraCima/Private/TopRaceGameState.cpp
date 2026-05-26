#include "Public/TopRaceGameState.h"
#include "Net/UnrealNetwork.h"

ATopRaceGameState::ATopRaceGameState()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATopRaceGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATopRaceGameState, MatchPhase);
    DOREPLIFETIME(ATopRaceGameState, TimeRemaining);
    DOREPLIFETIME(ATopRaceGameState, TotalCheckpoints);
    DOREPLIFETIME(ATopRaceGameState, PlayerRankings);
}

// ---------------------------------------------------------------
// Setters — solo el servidor los llama
// ---------------------------------------------------------------

void ATopRaceGameState::SetMatchPhase(EMatchPhase NewPhase)
{
    MatchPhase = NewPhase;
    OnRep_MatchPhase(); // Ejecutamos localmente en servidor tambien
}

void ATopRaceGameState::SetTimeRemaining(float NewTime)
{
    TimeRemaining = NewTime;
}

void ATopRaceGameState::SetTotalCheckpoints(int32 Total)
{
    TotalCheckpoints = Total;
}

void ATopRaceGameState::UpdatePlayerRankings(const TArray<ATopRacePlayerState*>& SortedPlayers)
{
    PlayerRankings = SortedPlayers;
}

// ---------------------------------------------------------------
// RepNotify — se ejecuta en clientes cuando MatchPhase cambia
// ---------------------------------------------------------------

void ATopRaceGameState::OnRep_MatchPhase()
{
    // Aqui podemos notificar a la UI o disparar efectos segun la fase
    switch (MatchPhase)
    {
    case EMatchPhase::InProgress:
        UE_LOG(LogTemp, Log, TEXT("GameState [Cliente]: Carrera en progreso"));
        // Ejemplo: BroadcastEvent para que el HUD muestre "GO!"
        break;

    case EMatchPhase::Finished:
        UE_LOG(LogTemp, Log, TEXT("GameState [Cliente]: Carrera finalizada"));
        // Ejemplo: BroadcastEvent para pantalla de resultados
        break;

    default:
        break;
    }
}
