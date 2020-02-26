// Fill out your copyright notice in the Description page of Project Settings.


#include "OctopusAIController.h"

AOctopusAIController::AOctopusAIController()
{
	//Initialize the behavior tree and blackboard components
	this->pBehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	this->pBlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
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
			this->pBlackboardComp->InitializeBlackboard(*(Octopus->GetBehaviorTree()->BlackboardAsset));
			this->pBehaviorComp->StartTree(*Octopus->GetBehaviorTree());
		}

		if (Octopus->GetTargetPoints() == nullptr)
		{
			Octopus->InitalizeTargetPointsQueue();
		}
	}
}

void AOctopusAIController::SetSeenTarget(APawn * InPawn)
{
	////Registers the Pawn that the AI has seen in the blackboard
	//if (this->pBlackboardComp)
	//{
	//	this->pBlackboardComp->SetValueAsObject(this->LocationBlackboardKey, InPawn);
	//}
}

void AOctopusAIController::EvaulateNextTargetPoint()
{
	AOctopus * Octopus = Cast<AOctopus>(this->GetPawn());
	if (this->pBlackboardComp != nullptr && Octopus->GetTargetPoints()->Count() > 0)
	{
		AOctopusTargetPoint * targetPoint;
		// remove next target point
		Octopus->GetTargetPoints()->Dequeue(targetPoint);
		this->pBlackboardComp->SetValueAsObject(this->LocationBlackboardKey, targetPoint);
		// add target point to end of queue to continue circular link
		Octopus->GetTargetPoints()->Enqueue(targetPoint);
	}
}
