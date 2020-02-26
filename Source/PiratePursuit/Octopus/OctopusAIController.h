// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Octopus.h"
#include "OctopusTargetPoint.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Containers/CircularQueue.h"
#include "Containers/List.h"
#include "OctopusAIController.generated.h"

/**
 *
 */
UCLASS()
class PIRATEPURSUIT_API AOctopusAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Constructor
	AOctopusAIController();

	// executes when the controller posses a pawn
	virtual void OnPossess(APawn * InPawn) override;

	// Set the sensed target in the blackboard
	void SetSeenTarget(APawn * Pawn);

	// Set the target point in the blackboard data
	void EvaulateNextTargetPoint();

#pragma region Getters and Setters
	FORCEINLINE FName GetLocationBlackboardKey() const { return this->LocationBlackboardKey; }

	FORCEINLINE UBehaviorTreeComponent * GetBehaviorTreeComp() const { return this->pBehaviorComp; }

	FORCEINLINE UBlackboardComponent * GetBlackboardComp() { return this->pBlackboardComp; }
#pragma endregion


private:
	// Blackboard key for location to go to
	UPROPERTY(EditDefaultsOnly, Category = "Custom AI")
		FName LocationBlackboardKey = "LocationToGo";
	// Behavior Tree component reference
	class UBehaviorTreeComponent * pBehaviorComp;
	// Blackboard component reference
	class UBlackboardComponent * pBlackboardComp;
};
