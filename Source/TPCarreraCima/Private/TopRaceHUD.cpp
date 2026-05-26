#include "Public/TopRaceHUD.h"
#include "Engine/Canvas.h"
#include "Public/TopRaceGameState.h"
#include "Public/TopRacePlayerState.h"

class ATopRaceGameState;
class ATopRacePlayerState;

ATopRaceHUD::ATopRaceHUD()
{
}

void ATopRaceHUD::BeginPlay()
{
    Super::BeginPlay();
}

void ATopRaceHUD::DrawHUD()
{
    Super::DrawHUD();

    if (bShowDebugInfoHud)
    {
        DrawDebugRaceInfo();
    }
}

void ATopRaceHUD::DrawDebugRaceInfo()
{
    ATopRacePlayerState* PS = GetOwningPlayerController()
        ? GetOwningPlayerController()->GetPlayerState<ATopRacePlayerState>()
        : nullptr;

    ATopRaceGameState* GS = GetWorld()
        ? GetWorld()->GetGameState<ATopRaceGameState>()
        : nullptr;

    float Y = 50.f;
    const float LineH = 22.f;
    const FLinearColor White(1,1,1,1);
    const FLinearColor Yellow(1,1,0,1);

    DrawText(TEXT("=== TOP RACE DEBUG ==="), Yellow, 20.f, Y); Y += LineH;

    if (GS)
    {
        DrawText(FString::Printf(TEXT("Tiempo restante: %.1f s"), GS->GetTimeRemaining()),
            White, 20.f, Y); Y += LineH;

        FString Phase;
        switch (GS->GetMatchPhase())
        {
            case EMatchPhase::WaitingToStart: Phase = TEXT("Esperando inicio"); break;
            case EMatchPhase::InProgress:     Phase = TEXT("En carrera");       break;
            case EMatchPhase::Finished:       Phase = TEXT("Finalizada");       break;
        }
        DrawText(FString::Printf(TEXT("Estado: %s"), *Phase), White, 20.f, Y); Y += LineH;
    }

    if (PS)
    {
        DrawText(FString::Printf(TEXT("Checkpoint: %d / %d"),
            PS->GetCurrentCheckpoint(),
            GS ? GS->GetTotalCheckpoints() : 0),
            White, 20.f, Y); Y += LineH;

        DrawText(FString::Printf(TEXT("Posicion: %d"), PS->GetRacePosition()),
            Yellow, 20.f, Y); Y += LineH;

        DrawText(FString::Printf(TEXT("Estado: %s"),
            PS->IsFinished() ? TEXT("FINALIZADO") : TEXT("Corriendo")),
            White, 20.f, Y);
    }
}
