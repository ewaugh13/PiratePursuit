#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Water.generated.h"

UCLASS()
class PIRATEPURSUIT_API AWater : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWater();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Rising")
		// Speed water rises at
		float m_RisingSpeed = 33.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rising")
		// Whether the water is rising or not
		bool m_IsActive = false;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};
