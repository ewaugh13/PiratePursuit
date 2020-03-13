#include "TreasureCheckpoint.h"

#include "Treasure/Treasure.h"
#include "Treasure/TreasureRespawnComponent.h"
#include "Water.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATreasureCheckpoint::ATreasureCheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CheckpointZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = m_CheckpointZone;

	OnActorBeginOverlap.AddDynamic(this, &ATreasureCheckpoint::ActorOverlap);
}

void ATreasureCheckpoint::ActorOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	// if we hit water
	if (Cast<AWater>(OtherActor))
	{
		TArray<AActor*> treasures;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATreasure::StaticClass(), treasures);

		for (int32 i = 0; i < treasures.Num(); i++)
		{
			UTreasureRespawnComponent * treasureRespawn = Cast<UTreasureRespawnComponent>(treasures[i]->GetComponentByClass(UTreasureRespawnComponent::StaticClass()));
			if (IsValid(treasureRespawn))
			{
				treasureRespawn->m_RespawnPoint = GetActorLocation();
			}
		}
	}
}
