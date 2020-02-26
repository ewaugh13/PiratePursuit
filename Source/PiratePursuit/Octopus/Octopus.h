// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CircularQueue.h"
//#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "OctopusTargetPoint.h"
#include "Octopus.generated.h"

UCLASS()
class PIRATEPURSUIT_API AOctopus : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOctopus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Initalize Circular Queue for target points
	void InitalizeTargetPointsQueue();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Getters and Setters
	UBehaviorTree * GetBehaviorTree() { return this->pBehaviorTree; }

	TCircularQueue<AOctopusTargetPoint *> * GetTargetPoints() { return this->OctopusTargetPointsQueue; }
#pragma endregion


private:
	// behavior tree for the octopus
	UPROPERTY(EditAnywhere, Category = "BehaviorTree")
		UBehaviorTree * pBehaviorTree;
	// add the elements to the set in the order we will visit them  
	UPROPERTY(EditAnyWhere, Category = "Custom AI")
		TSet<AOctopusTargetPoint *> OctopusTargetPointsSet;
	TCircularQueue<AOctopusTargetPoint *> * OctopusTargetPointsQueue;
};
