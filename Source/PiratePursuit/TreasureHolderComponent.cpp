#include "TreasureHolderComponent.h"

#include "PiratePursuitCharacter.h"
#include "Treasure.h"

#include "Engine.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UTreasureHolderComponent::UTreasureHolderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTreasureHolderComponent::BeginPlay()
{
	Super::BeginPlay();

	_OwnerMovement = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();

	if (_OwnerMovement != nullptr)
	{
		_OriginalMaxSpeed = _OwnerMovement->MaxWalkSpeed;
	}
}

// Called every frame
void UTreasureHolderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// update cooldown if we are in cooldown state
	if (InCooldownState())
	{
		_CurrentCooldown -= DeltaTime;
	}
}

void UTreasureHolderComponent::PickupTreasure(ATreasure * NewTreasure)
{
	if (_HeldTreasure == nullptr)
	{
		_HeldTreasure = NewTreasure;

		// make the other player loose the treasure
		AActor* parent = NewTreasure->GetAttachParentActor();
		if (parent)
		{
			UTreasureHolderComponent * parentTreasureHolder = Cast<UTreasureHolderComponent>(parent->GetComponentByClass(GetClass()));
			if (parentTreasureHolder)
			{ 
				parentTreasureHolder->LoseTreasure(); 
			}
		}

		if (_OwnerMovement != nullptr)
		{
			_OwnerMovement->MaxWalkSpeed = _OriginalMaxSpeed * m_SpeedMultiplier;
		}
		NewTreasure->AttachToActor(GetOwner(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false));
	}
}

void UTreasureHolderComponent::LoseTreasure()
{
	if (_HeldTreasure != nullptr)
	{
		_CurrentCooldown = m_CooldownTime;
		_HeldTreasure->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, false));
		_HeldTreasure = nullptr;

		if (_OwnerMovement != nullptr)
		{
			_OwnerMovement->MaxWalkSpeed = _OriginalMaxSpeed;
		}
	}
}
