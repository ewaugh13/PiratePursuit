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
	m_CheckpointZone->SetBoxExtent(FVector(32.0f, 32.0f, 8.0f));

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
			ATreasure * treasure = Cast<ATreasure>(treasures[i]);
			// remove top element that would be the one getting hit by water
			treasure->m_TreasureCheckpoints.pop();

			UTreasureRespawnComponent * treasureRespawn = Cast<UTreasureRespawnComponent>(treasures[i]->GetComponentByClass(UTreasureRespawnComponent::StaticClass()));
			if (IsValid(treasureRespawn) && treasure->m_TreasureCheckpoints.size() > 0)
			{
				// get next respawn point from stack and add 10 so its above the checkpoint box
				treasureRespawn->m_RespawnPoint = FVector(0.0f, 0.0f, 50.0f) +
					treasure->m_TreasureCheckpoints.top()->GetActorLocation();
			}
		}
	}
}
