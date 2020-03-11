#include "OctopusSelectTargetTask.h"

#include "OctopusAIController.h"
#include "OctopusTargetPoint.h"

#include "Engine.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UOctopusSelectTargetTask::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AOctopusAIController * octopusAI = Cast<AOctopusAIController>(OwnerComp.GetAIOwner());

	if (octopusAI != nullptr)
	{
		UBlackboardComponent * blackBoardComp = octopusAI->GetBlackboardComp();

		AOctopusTargetPoint * targetPoint = Cast<AOctopusTargetPoint>(blackBoardComp->
			GetValueAsObject(octopusAI->GetLocationBlackboardKey()));

		octopusAI->EvaulateNextTargetPoint();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
