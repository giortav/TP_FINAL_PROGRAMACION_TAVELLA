#include "TopRaceResultWidget.h"
#include "Components/Image.h"

void UTopRaceResultWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UTopRaceResultWidget::SetupResult(int32 FinalPosition, int32 TotalPlayers)
{
    // ANTES: en cada rama del if/else se tocaban ambas imágenes por separado,
    // duplicando la misma lógica espejada. 
    // AHORA: se calcula la visibilidad de victoria una vez y se deriva la derrota.
    const bool bWon = (FinalPosition == 1);
    const ESlateVisibility WinVisibility  = bWon ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
    const ESlateVisibility LoseVisibility = bWon ? ESlateVisibility::Hidden  : ESlateVisibility::Visible;

    if (ImageVictory) { ImageVictory->SetVisibility(WinVisibility);  }
    if (ImageDefeat)  { ImageDefeat->SetVisibility(LoseVisibility);  }
}
