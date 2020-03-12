#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FloatWater.generated.h"

class AWater;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PIRATEPURSUIT_API UFloatWater : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFloatWater();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetActorOnTop() const { return _ActorOnTop; }
	void SetActorOnTop(bool i_ActorOnTop) { _ActorOnTop = i_ActorOnTop; }

	void SetQuickRise(bool i_QuickRise) { _QuickRising = i_QuickRise; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water")
		// Amount the barrel sinks
		float m_SinkRate = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Water")
		// Rate at which we rise compared to sinking
		float m_RiseRate = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water")
		// Total time it will take to rise
		float m_TimeToRise = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water")
		// Water object is floating in
		AWater * m_WaterInstance;

private:

	UPROPERTY(VisibleAnywhere, Category = "Water")
		// On begin play will set start distance apart
		float _StartDistanceBetweenWater = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Water")
		// Is there an actor on top
		bool _ActorOnTop = false;

	UPROPERTY(VisibleAnywhere, Category = "Water")
		// Are we quick rising
		bool _QuickRising = false;
};
