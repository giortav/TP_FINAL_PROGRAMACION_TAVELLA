#include "TopRaceResultWidget.h"
#include "Components/Image.h"
 
void UTopRaceResultWidget::NativeConstruct()
{
	Super::NativeConstruct();
 
	// Ocultamos ambas imagenes al inicio
}
 
void UTopRaceResultWidget::SetupResult(int32 FinalPosition, int32 TotalPlayers)
{
	UE_LOG(LogTemp, Log, TEXT("SetupResult llamado, posicion: %d"), FinalPosition);
	UE_LOG(LogTemp, Log, TEXT("ImageVictory es valido: %s"), ImageVictory ? TEXT("SI") : TEXT("NO"));
	UE_LOG(LogTemp, Log, TEXT("ImageDefeat es valido: %s"),  ImageDefeat  ? TEXT("SI") : TEXT("NO"));

	if (FinalPosition == 1)
	{
		if (ImageVictory)
		{
			ImageVictory->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Log, TEXT("ImageVictory -> Visible"));
		}
		if (ImageDefeat)
		{
			ImageDefeat->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Log, TEXT("ImageDefeat -> Hidden"));
		}
	}
	else
	{
		if (ImageVictory)
		{
			ImageVictory->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Log, TEXT("ImageVictory -> Hidden"));
		}
		if (ImageDefeat)
		{
			ImageDefeat->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Log, TEXT("ImageDefeat -> Visible"));
		}
	}
}