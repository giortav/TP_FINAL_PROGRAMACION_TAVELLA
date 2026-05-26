#include "Public/CheckpointActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Public/TopRaceCharacter.h"

ACheckpointActor::ACheckpointActor()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true; // El actor existe en todos los clientes

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetBoxExtent(FVector(100.f, 200.f, 150.f));
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = TriggerBox;

    CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheckpointMesh"));
    CheckpointMesh->SetupAttachment(RootComponent);
    CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACheckpointActor::BeginPlay()
{
    Super::BeginPlay();

    // Solo el servidor registra el overlap para evitar llamadas duplicadas
    // Los clientes ven el actor pero no procesan la logica
    if (HasAuthority())
    {
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpointActor::OnOverlapBegin);
    }
}

void ACheckpointActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
                                       AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp,
                                       int32 OtherBodyIndex,
                                       bool bFromSweep,
                                       const FHitResult& SweepResult)
{
    // HasAuthority() redundante pero explicito para claridad didactica
    if (!HasAuthority()) return;

    ATopRaceCharacter* RacingChar = Cast<ATopRaceCharacter>(OtherActor);
    if (!RacingChar) return;

    // Delegamos al Server RPC del Character (ya estamos en servidor)
    // Esto dispara la cadena: Character -> GameMode -> PlayerState
    RacingChar->ServerInteractCheckpoint(CheckpointIndex);

    UE_LOG(LogTemp, Log, TEXT("Checkpoint %d: %s entro al trigger"),
        CheckpointIndex, *OtherActor->GetName());
}
