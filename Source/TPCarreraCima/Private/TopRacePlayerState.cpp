#include "Public/TopRacePlayerState.h"

#include "Net/UnrealNetwork.h"

ATopRacePlayerState::ATopRacePlayerState()
{
}

void ATopRacePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATopRacePlayerState, CurrentCheckpoint);
    DOREPLIFETIME(ATopRacePlayerState, RacePosition);
    DOREPLIFETIME(ATopRacePlayerState, RaceStatus);
    DOREPLIFETIME(ATopRacePlayerState, bFinished);
    DOREPLIFETIME(ATopRacePlayerState, TeamIndex);
}

// ---------------------------------------------------------------
// Setters
// ---------------------------------------------------------------

void ATopRacePlayerState::SetCurrentCheckpoint(int32 NewCheckpoint)
{
    CurrentCheckpoint = NewCheckpoint;
    OnRep_CurrentCheckpoint(); // Ejecutar localmente en servidor
}

void ATopRacePlayerState::SetRacePosition(int32 NewPosition)
{
    RacePosition = NewPosition;
}

void ATopRacePlayerState::SetRaceStatus(EPlayerRaceStatus NewStatus)
{
    RaceStatus = NewStatus;
    OnRep_RaceStatus();
}

void ATopRacePlayerState::SetFinished(bool bNewFinished)
{
    bFinished = bNewFinished;
    if (bFinished)
    {
        SetRaceStatus(EPlayerRaceStatus::Finished);
    }
}

void ATopRacePlayerState::SetTeam(int32 NewTeam)
{
    TeamIndex = NewTeam;
}

// ---------------------------------------------------------------
// RepNotifies
// ---------------------------------------------------------------

void ATopRacePlayerState::OnRep_CurrentCheckpoint()
{
    // El cliente local actualiza su HUD con el nuevo checkpoint
    // En el cliente, llamamos al PlayerController para que actualice la UI
    UE_LOG(LogTemp, Log, TEXT("PlayerState [RepNotify]: Checkpoint actualizado a %d"), CurrentCheckpoint);

    // Ejemplo de como notificar al HUD (implementar segun el widget):
    // if (APlayerController* PC = GetPlayerController())
    // {
    //     if (ATopRacePlayerController* TRPC = Cast<ATopRacePlayerController>(PC))
    //         TRPC->UpdateCheckpointHUD(CurrentCheckpoint);
    // }
}

void ATopRacePlayerState::OnRep_RaceStatus()
{
    UE_LOG(LogTemp, Log, TEXT("PlayerState [RepNotify]: Estado cambiado a %d"), (int32)RaceStatus);

    // Aqui podriamos disparar efectos visuales o UI segun el estado:
    // Racing   -> mostrar HUD normal
    // Respawning -> mostrar pantalla de respawn countdown
    // Finished -> mostrar pantalla de resultado personal
}
