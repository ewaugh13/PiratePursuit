// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Water.h"

#include "FloatWater.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

	bool GetActorOnTop() const { return this->ActorOnTop; }
	void SetActorOnTop(bool i_ActorOnTop) { this->ActorOnTop = i_ActorOnTop; }

	void SetQuickRise(bool value) { this->QuickRising = value; }

	// sinks the object
	void SinkObject(float SinkRate);
	// over time object floats back up
	void FloatBackUp(float TimeToRise);

private:
	UPROPERTY(EditAnywhere, Category = "Water")
		float RiseSinkRate = 15.0f;
	UPROPERTY(EditAnywhere, Category = "Water")
		float TimeToRise = 3.0f;
	UPROPERTY(EditAnywhere, Category = "Water")
		AWater * WaterInstance;
	float StartDistanceBetweenWater;
	bool ActorOnTop = false;
	bool QuickRising = false;
};
