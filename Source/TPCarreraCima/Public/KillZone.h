#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillZone.generated.h"

UCLASS()
class TPCARRERACIMA_API AKillZone : public AActor
{
	GENERATED_BODY()

public:
	AKillZone();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerBox;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};