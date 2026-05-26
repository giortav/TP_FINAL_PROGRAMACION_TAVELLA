#include "Public/TopRaceCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Public/TopRaceGameMode.h"
#include "Public/TopRacePlayerState.h"

ATopRaceCharacter::ATopRaceCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // El movimiento se replica automaticamente via CharacterMovementComponent
    bReplicates = true;

    // Configuracion de movimiento tipico de plataformas
    GetCharacterMovement()->JumpZVelocity         = 600.f;
    GetCharacterMovement()->AirControl            = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed          = 600.f;
    GetCharacterMovement()->GravityScale          = 1.5f;
    GetCharacterMovement()->SetIsReplicated(true);
}

void ATopRaceCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ATopRaceCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Solo el servidor verifica la caida (tiene autoridad sobre el estado)
    if (HasAuthority() && !bIsDead)
    {
        CheckFallDeath();
    }
}

void ATopRaceCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATopRaceCharacter, Health);
    DOREPLIFETIME(ATopRaceCharacter, bIsDead);
}

// ---------------------------------------------------------------
// Server RPC — ServerInteractCheckpoint
// El cliente llama esto cuando detecta overlap con un checkpoint
// ---------------------------------------------------------------

bool ATopRaceCharacter::ServerInteractCheckpoint_Validate(int32 CheckpointIndex)
{
    // Validacion anti-cheat basica: el indice debe ser positivo
    return CheckpointIndex >= 0 && CheckpointIndex < 100;
}

void ATopRaceCharacter::ServerInteractCheckpoint_Implementation(int32 CheckpointIndex)
{
    // Esta funcion se ejecuta en el SERVIDOR

    // Cooldown para evitar activaciones repetidas rapidas
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastCheckpointInteractTime < CheckpointCooldown) return;
    LastCheckpointInteractTime = CurrentTime;

    // Avisamos al GameMode (que tiene la logica de negocio)
    ATopRaceGameMode* GM = GetWorld()->GetAuthGameMode<ATopRaceGameMode>();
    ATopRacePlayerState* PS = GetPlayerState<ATopRacePlayerState>();

    if (GM && PS)
    {
        GM->OnPlayerReachedCheckpoint(PS, CheckpointIndex);
        MulticastPlayCheckpointFX(); // Todos ven el efecto
    }
}

// ---------------------------------------------------------------
// NetMulticast RPCs — ejecutan en todos los clientes y servidor
// ---------------------------------------------------------------

void ATopRaceCharacter::MulticastPlayRespawnFX_Implementation()
{
    // Reproducir particula o sonido de respawn en todos los clientes
    // Ejemplo: UGameplayStatics::SpawnEmitterAtLocation(...)
    UE_LOG(LogTemp, Log, TEXT("Character [Multicast]: Efecto de respawn para %s"),
        *GetName());
}

void ATopRaceCharacter::MulticastPlayCheckpointFX_Implementation()
{
    // Reproducir efecto de checkpoint (solo estetico, no cambia estado)
    UE_LOG(LogTemp, Log, TEXT("Character [Multicast]: Efecto de checkpoint"));
}

// ---------------------------------------------------------------
// Caida y muerte
// ---------------------------------------------------------------

void ATopRaceCharacter::CheckFallDeath()
{
    if (GetActorLocation().Z < FallDeathZ)
    {
        OnFellOffMap();
    }
}

void ATopRaceCharacter::OnFellOffMap()
{
    if (bFallDeathPending) return;
    bFallDeathPending = true;

    ATopRacePlayerState* PS = GetPlayerState<ATopRacePlayerState>();
    if (PS)
    {
        PS->SetRaceStatus(EPlayerRaceStatus::Respawning);
    }

    UE_LOG(LogTemp, Log, TEXT("Character: %s cayo al vacio, respawneando..."), *GetName());

    // Respawn con delay desde el GameMode
    AController* OldController = GetController();
    if (OldController)
    {
        FTimerHandle RespawnTimer;
        GetWorldTimerManager().SetTimer(RespawnTimer, [this, OldController]()
        {
            ATopRaceGameMode* GM = GetWorld()->GetAuthGameMode<ATopRaceGameMode>();
            if (GM)
            {
                GM->RespawnPlayer(Cast<APlayerController>(OldController));
            }
            Destroy(); // Destruimos el pawn viejo
        }, 3.f, false);
    }
}

void ATopRaceCharacter::Die()
{
    if (!HasAuthority() || bIsDead) return;

    bIsDead = true;
    OnFellOffMap(); // Reutilizamos la logica de respawn
}

// ---------------------------------------------------------------
// RepNotify — se ejecuta en clientes cuando Health cambia
// ---------------------------------------------------------------

void ATopRaceCharacter::OnRep_Health()
{
    // Actualizar barra de vida en HUD del jugador local
    UE_LOG(LogTemp, Log, TEXT("Character [RepNotify]: Salud actualizada a %.1f"), Health);

    // Ejemplo de notificacion al HUD:
    // if (IsLocallyControlled())
    // {
    //     if (ATopRacePlayerController* PC = Cast<ATopRacePlayerController>(GetController()))
    //         PC->UpdateHealthHUD(Health, MaxHealth);
    // }
}
