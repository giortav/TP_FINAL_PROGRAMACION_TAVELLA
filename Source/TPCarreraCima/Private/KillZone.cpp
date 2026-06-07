#include "KillZone.h"
#include "Components/BoxComponent.h"
#include "TopRaceCharacter.h"
#include "TopRaceGameMode.h"
#include "GameFramework/PlayerController.h"

AKillZone::AKillZone()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(500.f, 500.f, 50.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = TriggerBox;
}

void AKillZone::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(
			this, &AKillZone::OnOverlapBegin);
	}
}

void AKillZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	ATopRaceCharacter* Char = Cast<ATopRaceCharacter>(OtherActor);
	if (!Char || Char->IsDead()) return;

	Char->OnFellOffMap();
}