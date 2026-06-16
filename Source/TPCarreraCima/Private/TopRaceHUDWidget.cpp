#include "TopRaceHUDWidget.h"
#include "TopRaceGameState.h"
#include "TopRacePlayerState.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"

void UTopRaceHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();
    CacheReferences();
}

void UTopRaceHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    RefreshHUD();
}

// ---------------------------------------------------------------
// Cache — busca las referencias una sola vez
// ---------------------------------------------------------------

void UTopRaceHUDWidget::CacheReferences()
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    CachedPlayerState = PC->GetPlayerState<ATopRacePlayerState>();
    CachedGameState   = GetWorld()->GetGameState<ATopRaceGameState>();
}

// ---------------------------------------------------------------
// RefreshHUD — se llama cada tick, actualiza los Text Blocks
// ---------------------------------------------------------------

void UTopRaceHUDWidget::RefreshHUD()
{
    if (!CachedPlayerState || !CachedGameState)
    {
        CacheReferences();
        return;
    }

    if (TextPosition)
    {
        TextPosition->SetText(FormatPosition(CachedPlayerState->GetRacePosition()));
    }

    if (TextCheckpoint)
    {
        FText CheckpointText = FText::FromString(
            FString::Printf(TEXT("%d / %d"),
                CachedPlayerState->GetCurrentCheckpoint(),
                CachedGameState->GetTotalCheckpoints()));

        TextCheckpoint->SetText(CheckpointText);
    }

    if (TextTime)
    {
        TextTime->SetText(FormatTime(CachedGameState->GetTimeRemaining()));
    }

    if (TextStatus)
    {
        FString StatusStr;
        switch (CachedPlayerState->GetRaceStatus())
        {
            case EPlayerRaceStatus::Racing:     StatusStr = TEXT("Corriendo");    break;
            case EPlayerRaceStatus::Respawning: StatusStr = TEXT("Respawneando"); break;
            case EPlayerRaceStatus::Finished:   StatusStr = TEXT("Finalizado!");  break;
            case EPlayerRaceStatus::Spectating: StatusStr = TEXT("Espectador");   break;
        }
        TextStatus->SetText(FText::FromString(StatusStr));
    }
}

// ---------------------------------------------------------------
// Helpers de formato
// ---------------------------------------------------------------

FText UTopRaceHUDWidget::FormatTime(float Seconds) const
{
    int32 Minutes = FMath::FloorToInt(Seconds / 60.f);
    int32 Secs    = FMath::FloorToInt(FMath::Fmod(Seconds, 60.f));
    return FText::FromString(FString::Printf(TEXT("%d:%02d"), Minutes, Secs));
}

FText UTopRaceHUDWidget::FormatPosition(int32 Position) const
{
    // ANTES: cases 1-4 hardcodeados que producían exactamente el mismo
    // resultado que el default ("%d°"), código muerto sin valor.
    // AHORA: un único path para todos los valores.
    return FText::FromString(FString::Printf(TEXT("%d°"), Position));
}
