#include "SpeedBoostItem.h"
#include "TopRaceCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASpeedBoostItem::ASpeedBoostItem()
{
    bReplicates = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
    OverlapSphere->SetupAttachment(RootComponent);
    OverlapSphere->SetSphereRadius(60.f);
    OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    OverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ASpeedBoostItem::BeginPlay()
{
    Super::BeginPlay();

    // Solo el servidor maneja la lógica
    if (HasAuthority())
    {
        OverlapSphere->OnComponentBeginOverlap.AddDynamic(
            this, &ASpeedBoostItem::OnOverlapBegin);
    }
}

void ASpeedBoostItem::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!HasAuthority() || !bIsActive) return;

    ATopRaceCharacter* Char = Cast<ATopRaceCharacter>(OtherActor);
    if (!Char) return;

    bIsActive = false;
    SetActorHiddenInGame(true);
    OverlapSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Aplicamos el boost
    float BaseSpeed = Char->GetCharacterMovement()->MaxWalkSpeed;
    Char->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed * SpeedMultiplier;

    // Timer para revertir el boost
    FTimerHandle BoostHandle;
    GetWorldTimerManager().SetTimer(BoostHandle, [Char, BaseSpeed]()
    {
        if (IsValid(Char))
            Char->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
    }, BoostDuration, false);

    // Timer para reaparecer el item
    if (RespawnTime > 0.f)
    {
        FTimerHandle RespawnHandle;
        GetWorldTimerManager().SetTimer(RespawnHandle,
            this, &ASpeedBoostItem::Respawn,
            RespawnTime, false);
    }
}

void ASpeedBoostItem::Respawn()
{
    bIsActive = true;
    SetActorHiddenInGame(false);
    OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}