// Fill out your copyright notice in the Description page of Project Settings.


#include "TreasureHolderComponent.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "PiratePursuitCharacter.h"
#include "Engine.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UTreasureHolderComponent::UTreasureHolderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTreasureHolderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	currentCooldown = cooldownTime;
	UCharacterMovementComponent* movement = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	if (movement && movement->MaxWalkSpeed)
	{
		originalMaxSpeed = movement->MaxWalkSpeed;
	}
}


// Called every frame
void UTreasureHolderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (!canGrabTreasure())
	{
		currentCooldown += DeltaTime;
	}
	if (!hasTreasure())
	{
		UCharacterMovementComponent* movement = this->GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
		if (movement && movement->MaxWalkSpeed) {
			movement->MaxWalkSpeed = originalMaxSpeed;
		}
	}
}


void UTreasureHolderComponent::GetTreasure(AActor* treasure)
{
	if (hasTreasure()) return;
	this->treasure = treasure;

	AActor* parent = treasure->GetAttachParentActor();
	if (parent)
	{
		UTreasureHolderComponent* parentTreasureHolder = static_cast<UTreasureHolderComponent*>(parent->GetComponentByClass(GetClass()));
		if (parentTreasureHolder) { parentTreasureHolder->LoseTreasure(); }
	}
	UCharacterMovementComponent* movement = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	if (movement)
	{
		movement->MaxWalkSpeed = originalMaxSpeed * speedMultiplier;
	}
	treasure->AttachToActor(GetOwner(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false));
}


void UTreasureHolderComponent::LoseTreasure()
{
	if (!hasTreasure()) return;
	currentCooldown = 0.f;
	treasure->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, false));
	treasure = nullptr;

	UCharacterMovementComponent* movement = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	if (movement)
	{
		movement->MaxWalkSpeed = originalMaxSpeed;
	}
}
