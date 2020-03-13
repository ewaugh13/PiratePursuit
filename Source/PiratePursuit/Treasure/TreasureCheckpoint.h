#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TreasureCheckpoint.generated.h"

class UBoxComponent;

UCLASS()
class PIRATEPURSUIT_API ATreasureCheckpoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATreasureCheckpoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UBoxComponent * m_CheckpointZone;

	UFUNCTION()
		void ActorOverlap(AActor * OverlappedActor, AActor * OtherActor);
};
