#include "FloatWater.h"

#include "Water.h"

#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UFloatWater::UFloatWater()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UFloatWater::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() != nullptr && m_WaterInstance != nullptr)
	{
		_StartDistanceBetweenWater = GetOwner()->GetActorLocation().Z - m_WaterInstance->GetActorLocation().Z;
	}
}

// Called every frame
void UFloatWater::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner() != nullptr && m_WaterInstance != nullptr && m_WaterInstance->m_IsActive)
	{
		// sink state
		if (_ActorOnTop)
		{
			FVector barrelLoc = GetOwner()->GetActorLocation();
			GetOwner()->SetActorLocation(FVector(barrelLoc.X, barrelLoc.Y, barrelLoc.Z - m_SinkRate * DeltaTime));
		}
		// rise back up state
		else if (_QuickRising)
		{
			FVector waterLoc = m_WaterInstance->GetActorLocation();
			FVector barrelLoc = GetOwner()->GetActorLocation();
			if (abs(waterLoc.Z + _StartDistanceBetweenWater - barrelLoc.Z) > 1.0f)
			{
				// rise up at a fraction of the sink rate
				GetOwner()->SetActorLocation(FVector(barrelLoc.X, barrelLoc.Y,
					barrelLoc.Z + ((m_SinkRate * m_RiseRate + m_WaterInstance->m_RisingSpeed) * DeltaTime)));
			}
			else
			{
				_QuickRising = false;
			}
		}
		// rising with water state
		else
		{
			FVector waterLoc = m_WaterInstance->GetActorLocation();
			FVector barrelLoc = GetOwner()->GetActorLocation();
			GetOwner()->SetActorLocation(FVector(barrelLoc.X, barrelLoc.Y, waterLoc.Z + _StartDistanceBetweenWater));
		}
	}
}