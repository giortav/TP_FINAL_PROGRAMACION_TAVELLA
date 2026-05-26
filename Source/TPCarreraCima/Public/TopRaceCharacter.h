#pragma once

#include "CoreMinimal.h"
#include "TPCarreraCimaCharacter.h"
#include "GameFramework/Character.h"
#include "TopRaceCharacter.generated.h"

class ATopRacePlayerState;
class ATopRaceGameMode;

UCLASS()
class TPCARRERACIMA_API ATopRaceCharacter : public ATPCarreraCimaCharacter
{
    GENERATED_BODY()

public:
    ATopRaceCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // -------------------------------------------------------
    // Server RPCs — cliente llama, ejecutan en servidor
    // -------------------------------------------------------

    // El cliente le avisa al servidor que interactuo con un checkpoint
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerInteractCheckpoint(int32 CheckpointIndex);

    // -------------------------------------------------------
    // NetMulticast RPCs — servidor llama, ejecutan en TODOS
    // -------------------------------------------------------

    // Todos ven los efectos de respawn
    UFUNCTION(NetMulticast, Reliable)
    void MulticastPlayRespawnFX();

    // Todos ven efectos de checkpoint
    UFUNCTION(NetMulticast, Unreliable)
    void MulticastPlayCheckpointFX();

    // -------------------------------------------------------
    // Caida / Muerte / Respawn
    // -------------------------------------------------------
    void OnFellOffMap();

    // Llamado cuando la salud llega a 0
    UFUNCTION(BlueprintCallable, Category = "Race")
    void Die();

    // -------------------------------------------------------
    // Estado replicado
    // -------------------------------------------------------

    UFUNCTION(BlueprintCallable, Category = "Race")
    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable, Category = "Race")
    bool IsDead() const { return bIsDead; }

protected:
    // Limite de altura para detectar caida al vacio
    UPROPERTY(EditDefaultsOnly, Category = "Race")
    float FallDeathZ = -500.f;

    // Tiempo de cooldown entre interacciones con checkpoints (segundos)
    UPROPERTY(EditDefaultsOnly, Category = "Race")
    float CheckpointCooldown = 1.f;

    // Salud del jugador — replicada con RepNotify para actualizar barra en HUD
    UPROPERTY(ReplicatedUsing = OnRep_Health)
    float Health = 100.f;

    UPROPERTY(EditDefaultsOnly, Category = "Race")
    float MaxHealth = 100.f;

    UPROPERTY(Replicated)
    bool bIsDead = false;

private:
    UFUNCTION()
    void OnRep_Health();

    void CheckFallDeath();

    float LastCheckpointInteractTime = 0.f;
    bool bFallDeathPending = false;
};
