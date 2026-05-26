#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinishLineActor.generated.h"

class UBoxComponent;
class ATopRaceCharacter;

UCLASS()
class TPCARRERACIMA_API AFinishLineActor : public AActor
{
    GENERATED_BODY()

public:
    AFinishLineActor();
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* TriggerBox;

private:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
                        AActor* OtherActor,
                        UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex,
                        bool bFromSweep,
                        const FHitResult& SweepResult);
};
