#include "Public/FinishLineActor.h"
#include "Components/BoxComponent.h"
#include "Public/TopRaceCharacter.h"
#include "Public/TopRaceGameMode.h"
#include "Public/TopRacePlayerController.h"
#include "Public/TopRacePlayerState.h"

AFinishLineActor::AFinishLineActor()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetBoxExtent(FVector(200.f, 400.f, 200.f));
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = TriggerBox;
}

void AFinishLineActor::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFinishLineActor::OnOverlapBegin);
    }
}

void AFinishLineActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
                                       AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp,
                                       int32 OtherBodyIndex,
                                       bool bFromSweep,
                                       const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;

    ATopRaceCharacter* RacingChar = Cast<ATopRaceCharacter>(OtherActor);
    if (!RacingChar || RacingChar->IsDead()) return;

    ATopRacePlayerState* PS = RacingChar->GetPlayerState<ATopRacePlayerState>();
    if (!PS || PS->IsFinished()) return;

    if (ATopRaceGameMode* GM = GetWorld()->GetAuthGameMode<ATopRaceGameMode>())
    {
        GM->OnPlayerFinished(PS);
    }

    // ANTES: se casteaba dos veces a ATopRacePlayerController en bloques separados.
    // AHORA: un único cast reutilizado para ambas operaciones.
    if (ATopRacePlayerController* PC = Cast<ATopRacePlayerController>(RacingChar->GetController()))
    {
        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);
        PC->ClientShowNotification(TEXT("Llegaste a la meta!"));
    }
}
