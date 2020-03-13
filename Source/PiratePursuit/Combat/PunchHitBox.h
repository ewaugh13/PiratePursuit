#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PunchHitBox.generated.h"

class UBoxComponent;

UCLASS()
class PIRATEPURSUIT_API APunchHitBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APunchHitBox();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UBoxComponent * m_HitBox;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox")
		float m_KnockbackPower = 100000000.0f;
};
