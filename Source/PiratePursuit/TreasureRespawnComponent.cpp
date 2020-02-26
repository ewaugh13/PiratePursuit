// Fill out your copyright notice in the Description page of Project Settings.

#include "TreasureRespawnComponent.h"
#include "Engine.h"
#include "Engine/Classes/GameFramework/Actor.h"

// Sets default values for this component's properties
UTreasureRespawnComponent::UTreasureRespawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTreasureRespawnComponent::BeginPlay()
{
	Super::BeginPlay();
	//RespawnPoint = GetOwner()->GetActorLocation();
	// ...

}


// Called every frame
void UTreasureRespawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTreasureRespawnComponent::Respawn()
{
	GetOwner()->SetActorLocation(this->RespawnPoint);
}

