#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <stack> 
#include "Treasure.generated.h"

class ATreasureCheckpoint;
class UTreasureRespawnComponent;

class USoundBase;
class UStaticMeshComponent;

UCLASS()
class PIRATEPURSUIT_API ATreasure : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATreasure();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UTreasureRespawnComponent * m_TreasureRespawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent * m_TreasureMesh;

	UFUNCTION()
		void ActorOverlap(AActor * OverlappedActor, AActor * OtherActor);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Treasure")
		// Sound to play when stunning a player
		USoundBase * m_TreasurePickupSound;

	std::stack<ATreasureCheckpoint*> m_TreasureCheckpoints;

private:

	// Predicate used to sort array of respawn platforms with lower ones (Z axis) first
	inline static bool LowestPosition(const AActor& i_Actor1, const AActor& i_Actor2)
	{
		return (i_Actor1.GetActorLocation().Z > i_Actor2.GetActorLocation().Z);
	}
};
