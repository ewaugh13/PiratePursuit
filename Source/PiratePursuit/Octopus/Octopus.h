#pragma once

#include "CoreMinimal.h"
#include "CircularQueue.h"
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
	UBehaviorTree * GetBehaviorTree() { return _BehaviorTree; }

	TCircularQueue<AOctopusTargetPoint *> * GetTargetPoints() { return _OctopusTargetPointsQueue; }
#pragma endregion


private:

	UPROPERTY(EditAnywhere, Category = "BehaviorTree")
		// behavior tree for the octopus
		UBehaviorTree * _BehaviorTree;

	UPROPERTY(EditAnyWhere, Category = "Custom AI")
		// add the elements to the set in the order we will visit them  
		TSet<AOctopusTargetPoint *> _OctopusTargetPointsSet;

	TCircularQueue<AOctopusTargetPoint *> * _OctopusTargetPointsQueue;
};
