// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopRaceResultWidget.generated.h"
 
UCLASS()
class TPCARRERACIMA_API UTopRaceResultWidget : public UUserWidget
{
	GENERATED_BODY()
 
public:
	virtual void NativeConstruct() override;
 
	// Llamado desde el PlayerController via ClientOnMatchEnd
	UFUNCTION(BlueprintCallable, Category = "Result")
	void SetupResult(int32 FinalPosition, int32 TotalPlayers);
 
protected:
	// Imagen de victoria (posicion 1)
	UPROPERTY(meta = (BindWidget))
	class UImage* ImageVictory;
 
	// Imagen de derrota (posicion 2 en adelante)
	UPROPERTY(meta = (BindWidget))
	class UImage* ImageDefeat;
};
 