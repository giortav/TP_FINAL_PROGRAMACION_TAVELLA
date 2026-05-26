#include "Public/TopRaceGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "TimerManager.h"
#include "TopRaceCharacter.h"
#include "TopRaceGameState.h"
#include "TopRacePlayerState.h"
#include "Public/TopRacePlayerController.h"

class ATopRaceCharacter;

ATopRaceGameMode::ATopRaceGameMode()
{
    PrimaryActorTick.bCanEverTick = true;

    GameStateClass        = ATopRaceGameState::StaticClass();
    PlayerStateClass      = ATopRacePlayerState::StaticClass();
    PlayerControllerClass = ATopRacePlayerController::StaticClass();
    DefaultPawnClass      = ATopRaceCharacter::StaticClass();
}

void ATopRaceGameMode::BeginPlay()
{
    Super::BeginPlay();
    StartMatch();
}

void ATopRaceGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bMatchStarted && !bMatchEnded)
    {
        TickMatchTimer(DeltaTime);
    }
}

void ATopRaceGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    UE_LOG(LogTemp, Log, TEXT("GameMode: Jugador conectado. Total: %d"), GetNumPlayers());
}

void ATopRaceGameMode::StartMatch()
{
    bMatchStarted = true;
    ElapsedTime   = 0.f;

    ATopRaceGameState* GS = GetGameState<ATopRaceGameState>();
    if (GS)
    {
        GS->SetMatchPhase(EMatchPhase::InProgress);
        GS->SetTimeRemaining(MatchDuration);
        GS->SetTotalCheckpoints(TotalCheckpoints);
    }

    UE_LOG(LogTemp, Log, TEXT("GameMode: Carrera iniciada."));
}

void ATopRaceGameMode::EndMatch()
{
    if (bMatchEnded) return;
    bMatchEnded = true;

    ATopRaceGameState* GS = GetGameState<ATopRaceGameState>();
    if (GS)
    {
        GS->SetMatchPhase(EMatchPhase::Finished);
    }

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        ATopRacePlayerController* PC = Cast<ATopRacePlayerController>(It->Get());
        if (PC)
        {
            int32 FinalPos = FinishedPlayers.IndexOfByKey(PC->GetPlayerState<ATopRacePlayerState>()) + 1;
            PC->ClientOnMatchEnd(FinalPos);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("GameMode: Carrera finalizada."));
}

void ATopRaceGameMode::TickMatchTimer(float DeltaTime)
{
    if (MatchDuration <= 0.f) return;

    ElapsedTime += DeltaTime;
    float Remaining = FMath::Max(0.f, MatchDuration - ElapsedTime);

    ATopRaceGameState* GS = GetGameState<ATopRaceGameState>();
    if (GS)
    {
        GS->SetTimeRemaining(Remaining);
    }

    if (Remaining <= 0.f)
    {
        EndMatch();
    }
}

void ATopRaceGameMode::OnPlayerReachedCheckpoint(ATopRacePlayerState* PlayerState, int32 CheckpointIndex)
{
    if (!PlayerState || bMatchEnded) return;

    if (CheckpointIndex == PlayerState->GetCurrentCheckpoint() + 1)
    {
        PlayerState->SetCurrentCheckpoint(CheckpointIndex);
        UpdateRankings();

        UE_LOG(LogTemp, Log, TEXT("GameMode: %s alcanzo checkpoint %d"),
            *PlayerState->GetPlayerName(), CheckpointIndex);
    }
}

void ATopRaceGameMode::OnPlayerFinished(ATopRacePlayerState* PlayerState)
{
    if (!PlayerState || bMatchEnded) return;
    if (FinishedPlayers.Contains(PlayerState)) return;

    if (PlayerState->GetCurrentCheckpoint() < TotalCheckpoints - 1) return;

    int32 Position = FinishedPlayers.Num() + 1;
    FinishedPlayers.Add(PlayerState);
    PlayerState->SetRacePosition(Position);
    PlayerState->SetFinished(true);

    UE_LOG(LogTemp, Log, TEXT("GameMode: %s termino en posicion %d"),
        *PlayerState->GetPlayerName(), Position);

    if (FinishedPlayers.Num() >= GetNumPlayers())
    {
        EndMatch();
    }
}

void ATopRaceGameMode::UpdateRankings()
{
    ATopRaceGameState* GS = GetGameState<ATopRaceGameState>();
    if (!GS) return;

    TArray<ATopRacePlayerState*> AllPS;
    for (APlayerState* PS : GS->PlayerArray)
    {
        if (ATopRacePlayerState* TRPS = Cast<ATopRacePlayerState>(PS))
        {
            AllPS.Add(TRPS);
        }
    }

    AllPS.Sort([](const ATopRacePlayerState& A, const ATopRacePlayerState& B)
    {
        return A.GetCurrentCheckpoint() > B.GetCurrentCheckpoint();
    });

    for (int32 i = 0; i < AllPS.Num(); i++)
    {
        AllPS[i]->SetRacePosition(i + 1);
    }

    GS->UpdatePlayerRankings(AllPS);
}

void ATopRaceGameMode::RespawnPlayer(APlayerController* PlayerController)
{
    if (!PlayerController) return;

    ATopRacePlayerState* PS = PlayerController->GetPlayerState<ATopRacePlayerState>();
    if (!PS) return;

    FVector SpawnLocation = FVector::ZeroVector;
    FRotator SpawnRotation = FRotator::ZeroRotator;

    FString CheckpointTag = FString::Printf(TEXT("Checkpoint_%d"), PS->GetCurrentCheckpoint());
    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        if (It->PlayerStartTag.ToString() == CheckpointTag)
        {
            SpawnLocation = It->GetActorLocation();
            SpawnRotation = It->GetActorRotation();
            break;
        }
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    ATopRaceCharacter* NewChar = GetWorld()->SpawnActor<ATopRaceCharacter>(
        CharacterClass, SpawnLocation, SpawnRotation, SpawnParams);

    if (NewChar)
    {
        PlayerController->Possess(NewChar);
        NewChar->MulticastPlayRespawnFX();
    }
}