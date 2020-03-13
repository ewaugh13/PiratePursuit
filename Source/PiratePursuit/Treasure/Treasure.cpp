#include "Treasure.h"

#include "TreasureRespawnComponent.h"
#include "Water.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

#include "TreasureHolderComponent.h"

// Sets default values
ATreasure::ATreasure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_TreasureRespawn = CreateDefaultSubobject<UTreasureRespawnComponent>(TEXT("TreasureRespawn"));

	m_TreasureMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreasureMesh"));
	RootComponent = m_TreasureMesh;
}

void ATreasure::ActorOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	UTreasureHolderComponent * treasureHolder = Cast<UTreasureHolderComponent>(OtherActor->GetComponentByClass(UTreasureHolderComponent::StaticClass()));

	// if the other actor has a treasure holder component
	if (treasureHolder)
	{
		if (IsValid(treasureHolder) && !treasureHolder->InCooldownState())
		{
			// pickup treasure
			treasureHolder->PickupTreasure(this);
			UGameplayStatics::PlaySound2D(this, m_TreasurePickupSound);

			TArray<AActor*> waterInstances;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWater::StaticClass(), waterInstances);

			// activate water once treasure if picked up
			for (int32 i = 0; i < waterInstances.Num(); i++)
			{
				Cast<AWater>(waterInstances[i])->m_IsActive = true;
			}
		}
		else
		{
			// check for water collision
			if (Cast<AWater>(OtherActor))
			{
				m_TreasureRespawn->Respawn();
			}
		}
	}
}
