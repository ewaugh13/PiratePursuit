#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Barrel.generated.h"

class UFloatWater;

UCLASS()
class PIRATEPURSUIT_API ABarrel : public AStaticMeshActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

private:

		class UFloatWater * _FloatComponent;
};
