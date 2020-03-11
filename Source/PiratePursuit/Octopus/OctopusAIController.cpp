#include "OctopusAIController.h"

#include "Octopus.h"
#include "OctopusTargetPoint.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

AOctopusAIController::AOctopusAIController()
{
	//Initialize the behavior tree and blackboard components
	_BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	_BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AOctopusAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	// Get the possessed Pawn. If it's the AI Character we created
	// initialize it's blackboard and start it's corresponding behavior tree
	AOctopus * Octopus = Cast<AOctopus>(InPawn);
	if (Octopus != nullptr)
	{
		// the AAICharacter class does not exclude any member "BehaviorTree"
		if (Octopus->GetBehaviorTree()->BlackboardAsset)
		{
			_BlackboardComp->InitializeBlackboard(*(Octopus->GetBehaviorTree()->BlackboardAsset));
			_BehaviorComp->StartTree(*Octopus->GetBehaviorTree());
		}

		if (Octopus->GetTargetPoints() == nullptr)
		{
			Octopus->InitalizeTargetPointsQueue();
		}
	}
}

void AOctopusAIController::EvaulateNextTargetPoint()
{
	AOctopus * Octopus = Cast<AOctopus>(GetPawn());
	if (_BlackboardComp != nullptr && Octopus->GetTargetPoints()->Count() > 0)
	{
		AOctopusTargetPoint * targetPoint;
		// remove next target point
		Octopus->GetTargetPoints()->Dequeue(targetPoint);

		_BlackboardComp->SetValueAsObject(_LocationBlackboardKey, targetPoint);
		// add target point to end of queue to continue circular link
		Octopus->GetTargetPoints()->Enqueue(targetPoint);
	}
}
