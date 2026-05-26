#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TopRacePlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerRaceStatus : uint8
{
    Racing      UMETA(DisplayName = "Racing"),
    Respawning  UMETA(DisplayName = "Respawning"),
    Finished    UMETA(DisplayName = "Finished"),
    Spectating  UMETA(DisplayName = "Spectating")
};

UCLASS()
class TPCARRERACIMA_API ATopRacePlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    ATopRacePlayerState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // --- Setters (servidor) ---
    void SetCurrentCheckpoint(int32 NewCheckpoint);
    void SetRacePosition(int32 NewPosition);
    void SetRaceStatus(EPlayerRaceStatus NewStatus);
    void SetFinished(bool bNewFinished);
    void SetTeam(int32 NewTeam);

    // --- Getters (todos) ---
    UFUNCTION(BlueprintCallable, Category = "Race")
    int32 GetCurrentCheckpoint() const { return CurrentCheckpoint; }

    UFUNCTION(BlueprintCallable, Category = "Race")
    int32 GetRacePosition() const { return RacePosition; }

    UFUNCTION(BlueprintCallable, Category = "Race")
    EPlayerRaceStatus GetRaceStatus() const { return RaceStatus; }

    UFUNCTION(BlueprintCallable, Category = "Race")
    bool IsFinished() const { return bFinished; }

    UFUNCTION(BlueprintCallable, Category = "Race")
    int32 GetTeam() const { return TeamIndex; }

private:
    // Ultimo checkpoint alcanzado (0 = inicio)
    UPROPERTY(ReplicatedUsing = OnRep_CurrentCheckpoint)
    int32 CurrentCheckpoint = 0;

    // Posicion en la carrera (1 = primero)
    UPROPERTY(Replicated)
    int32 RacePosition = 1;

    // Estado actual del jugador en la carrera
    UPROPERTY(ReplicatedUsing = OnRep_RaceStatus)
    EPlayerRaceStatus RaceStatus = EPlayerRaceStatus::Racing;

    // Si el jugador ya termino la carrera
    UPROPERTY(Replicated)
    bool bFinished = false;

    // Equipo del jugador (para futura logica de equipos)
    UPROPERTY(Replicated)
    int32 TeamIndex = 0;

    // RepNotify: actualiza HUD del duenio cuando cambia su checkpoint
    UFUNCTION()
    void OnRep_CurrentCheckpoint();

    // RepNotify: actualiza animaciones/UI segun estado
    UFUNCTION()
    void OnRep_RaceStatus();
};
