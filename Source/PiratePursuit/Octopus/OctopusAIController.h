#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Containers/CircularQueue.h"
#include "OctopusAIController.generated.h"

class AOctopus;
class AOctopusTargetPoint;

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class PIRATEPURSUIT_API AOctopusAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Constructor
	AOctopusAIController();

	// executes when the controller posses a pawn
	virtual void OnPossess(APawn * InPawn) override;

	// Set the target point in the blackboard data
	void EvaulateNextTargetPoint();

#pragma region Getters and Setters
	FORCEINLINE FName GetLocationBlackboardKey() const { return _LocationBlackboardKey; }

	FORCEINLINE UBehaviorTreeComponent * GetBehaviorTreeComp() const { return _BehaviorComp; }

	FORCEINLINE UBlackboardComponent * GetBlackboardComp() { return _BlackboardComp; }
#pragma endregion

private:

	// Blackboard key for location to go to
	UPROPERTY(EditDefaultsOnly, Category = "Custom AI")
		FName _LocationBlackboardKey = "LocationToGo";

	// Behavior Tree component reference
	class UBehaviorTreeComponent * _BehaviorComp;

	// Blackboard component reference
	class UBlackboardComponent * _BlackboardComp;
};
