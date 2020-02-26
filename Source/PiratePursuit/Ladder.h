// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PiratePursuitCharacter.h"
#include "GameFramework/Actor.h"
#include "Ladder.generated.h"

UCLASS()
class PIRATEPURSUIT_API ALadder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALadder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	APiratePursuitCharacter * PiratePursuitCharacter;

	APiratePursuitCharacter * PiratePursuitCharacter1;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent * CollisionBox;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
