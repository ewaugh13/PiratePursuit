// Fill out your copyright notice in the Description page of Project Settings.


#include "Octopus.h"

// Sets default values
AOctopus::AOctopus()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_OctopusTargetPointsQueue = nullptr;
}

// Called when the game starts or when spawned
void AOctopus::BeginPlay()
{
	Super::BeginPlay();

	if (_OctopusTargetPointsQueue == nullptr)
	{
		this->InitalizeTargetPointsQueue();
	}
}

// Called every frame
void AOctopus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOctopus::InitalizeTargetPointsQueue()
{
	int32 numTargets = _OctopusTargetPointsSet.Num();
	_OctopusTargetPointsQueue = new TCircularQueue<AOctopusTargetPoint *>(numTargets + 1);
	for (AOctopusTargetPoint * target : _OctopusTargetPointsSet)
	{
		_OctopusTargetPointsQueue->Enqueue(target);
	}
	_OctopusTargetPointsSet.Empty();
}

// Called to bind functionality to input
void AOctopus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

