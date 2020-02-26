// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatWater.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UFloatWater::UFloatWater()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFloatWater::BeginPlay()
{
	Super::BeginPlay();
	if (this->GetOwner() != nullptr && this->WaterInstance != nullptr)
	{
		this->StartDistanceBetweenWater = this->GetOwner()->GetActorLocation().Z - this->WaterInstance->GetActorLocation().Z;
	}
	// ...

}


// Called every frame
void UFloatWater::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->GetOwner() != nullptr && this->WaterInstance != nullptr && this->WaterInstance->IsActive)
	{
		// sink
		if (this->ActorOnTop)
		{
			FVector barrelLoc = this->GetOwner()->GetActorLocation();
			this->GetOwner()->SetActorLocation(FVector(barrelLoc.X, barrelLoc.Y, barrelLoc.Z - this->RiseSinkRate * DeltaTime));
		}
		// rise back up
		else if (this->QuickRising)
		{
			FVector waterLoc = this->WaterInstance->GetActorLocation();
			FVector barrelLoc = this->GetOwner()->GetActorLocation();
			if (abs(waterLoc.Z + this->StartDistanceBetweenWater - barrelLoc.Z) > 1.0f)
			{
				this->GetOwner()->SetActorLocation(FVector(barrelLoc.X, barrelLoc.Y,
					barrelLoc.Z + ((this->RiseSinkRate / 4 + this->WaterInstance->RisingSpeed) * DeltaTime)));
			}
			else
			{
				this->QuickRising = false;
			}
		}
		// rising with water
		else
		{
			FVector waterLoc = this->WaterInstance->GetActorLocation();
			FVector barrelLoc = this->GetOwner()->GetActorLocation();
			this->GetOwner()->SetActorLocation(FVector(barrelLoc.X, barrelLoc.Y, waterLoc.Z + this->StartDistanceBetweenWater));
		}
	}
	// ...
}

void UFloatWater::SinkObject(float SinkRate)
{
	FVector barrelLoc = this->GetOwner()->GetActorLocation();
	this->GetOwner()->SetActorLocation(FVector(barrelLoc.X, barrelLoc.Y, barrelLoc.Z + SinkRate));
}

void UFloatWater::FloatBackUp(float TimeToRise)
{

}