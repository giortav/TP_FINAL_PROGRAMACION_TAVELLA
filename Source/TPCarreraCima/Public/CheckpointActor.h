#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckpointActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class ATopRaceCharacter;

UCLASS()
class TPCARRERACIMA_API ACheckpointActor : public AActor
{
    GENERATED_BODY()

public:
    ACheckpointActor();

    virtual void BeginPlay() override;

    // Indice de este checkpoint en la secuencia (0-based)
    UPROPERTY(EditInstanceOnly, Category = "Checkpoint")
    int32 CheckpointIndex = 0;

    // Posicion de spawn al respawnear en este checkpoint
    UPROPERTY(EditInstanceOnly, Category = "Checkpoint")
    FVector RespawnOffset = FVector(0.f, 0.f, 100.f);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* TriggerBox;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* CheckpointMesh;

private:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
                        AActor* OtherActor,
                        UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex,
                        bool bFromSweep,
                        const FHitResult& SweepResult);
};
