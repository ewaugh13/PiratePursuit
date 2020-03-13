#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Treasure.generated.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UTreasureRespawnComponent * m_TreasureRespawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent * m_TreasureMesh;

	UFUNCTION()
		void ActorOverlap(AActor * OverlappedActor, AActor * OtherActor);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Treasure")
		// Sound to play when stunning a player
		USoundBase * m_TreasurePickupSound;
};
