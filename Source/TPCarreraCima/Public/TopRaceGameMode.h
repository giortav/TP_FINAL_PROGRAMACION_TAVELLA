#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopRaceGameMode.generated.h"

class ATopRacePlayerState;
class ATopRaceGameState;
class ATopRaceCharacter;

UCLASS()
class TPCARRERACIMA_API ATopRaceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATopRaceGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void OnPlayerReachedCheckpoint(ATopRacePlayerState* PlayerState, int32 CheckpointIndex);
	void OnPlayerFinished(ATopRacePlayerState* PlayerState);
	void RespawnPlayer(APlayerController* PlayerController);
	void UpdateRankings();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Race Config")
	float MatchDuration = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Race Config")
	int32 TotalCheckpoints = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Race Config")
	float RespawnDelay = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Race Config")
	TSubclassOf<ATopRaceCharacter> CharacterClass;

private:
	void StartMatch();
	void EndMatch();
	void TickMatchTimer(float DeltaTime);

	bool bMatchStarted = false;
	bool bMatchEnded = false;
	float ElapsedTime = 0.f;

	TArray<ATopRacePlayerState*> FinishedPlayers;
};