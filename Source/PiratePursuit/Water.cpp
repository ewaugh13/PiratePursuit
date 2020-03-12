#include "Water.h"
#include "PiratePursuit.h"
#include "Treasure/Treasure.h"
#include "Treasure/TreasureHolderComponent.h"
#include "Treasure/TreasureRespawnComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWater::AWater()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWater::BeginPlay()
{
	Super::BeginPlay();

	this->GetStaticMeshComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &AWater::BeginOverlap);
}

// Called every frame
void AWater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_IsActive)
	{
		FVector newLocation = GetActorLocation();
		newLocation.Z += DeltaTime * m_RisingSpeed;
		SetActorLocation(newLocation, true, nullptr, ETeleportType::None);
	}
}

void AWater::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != nullptr)
	{
		UTreasureHolderComponent * treasureHolder = OtherActor->FindComponentByClass<UTreasureHolderComponent>();
		// if other actor is holding treasure and water is active
		if (treasureHolder != nullptr && m_IsActive && treasureHolder->GetHeldTreasure() != nullptr)
		{
			treasureHolder->LoseTreasure();

			TArray<AActor *> treasureActors;
			UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ATreasure::StaticClass(), treasureActors);

			// respawn all treasure
			for (AActor * treasureActor : treasureActors)
			{
				ATreasure * treasure = Cast<ATreasure>(treasureActor);
				UTreasureRespawnComponent * treasureRespawn = treasure->FindComponentByClass<UTreasureRespawnComponent>();
				treasureRespawn->Respawn();
			}
		}
	}
}

