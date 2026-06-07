#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpeedBoostItem.generated.h"

UCLASS()
class TPCARRERACIMA_API ASpeedBoostItem : public AActor
{
    GENERATED_BODY()

public:
    ASpeedBoostItem();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    class USphereComponent* OverlapSphere;

    // Cuánto multiplica la velocidad
    UPROPERTY(EditDefaultsOnly, Category = "SpeedBoost")
    float SpeedMultiplier = 2.f;

    // Duración del boost en segundos
    UPROPERTY(EditDefaultsOnly, Category = "SpeedBoost")
    float BoostDuration = 2.f;

    // Tiempo antes de reaparecer (0 = no reaparece)
    UPROPERTY(EditDefaultsOnly, Category = "SpeedBoost")
    float RespawnTime = 5.f;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

private:
    void Respawn();
    bool bIsActive = true;
};