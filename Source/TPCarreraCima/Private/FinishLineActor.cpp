#include "Public/FinishLineActor.h"
#include "Components/BoxComponent.h"
#include "Public/TopRaceCharacter.h"
#include "Public/TopRaceGameMode.h"
#include "Public/TopRacePlayerController.h"
#include "Public/TopRacePlayerState.h"

class ATopRacePlayerController;
class ATopRaceGameMode;
class ATopRacePlayerState;

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

    // Notificamos al GameMode que este jugador termino
    ATopRaceGameMode* GM = GetWorld()->GetAuthGameMode<ATopRaceGameMode>();
    if (GM)
    {
        GM->OnPlayerFinished(PS);
    }

    // Notificamos al cliente especifico via Client RPC en su PlayerController
    if (ATopRacePlayerController* PC = Cast<ATopRacePlayerController>(RacingChar->GetController()))
    {
        PC->ClientShowNotification(TEXT("Llegaste a la meta!"));
    }

    UE_LOG(LogTemp, Log, TEXT("FinishLine: %s llego a la meta!"), *OtherActor->GetName());
}
