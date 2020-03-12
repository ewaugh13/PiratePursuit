#include "PlayerStunComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundBase.h"

// Sets default values for this component's properties
UPlayerStunComponent::UPlayerStunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerStunComponent::BindDelegates()
{
	StunDelegate.BindUFunction(this, "Stun");
	UnstunDelegate.BindUFunction(this, "Unstun");
	StartGracePeriodDelegate.BindUFunction(this, "StartGracePeriod");
	EndGracePeriodDelegate.BindUFunction(this, "EndGracePeriod");
}

void UPlayerStunComponent::Stun()
{
	UGameplayStatics::PlaySound2D(this, m_PunchSound);

	if (!_IsStunned && !_IsInGracePeriod)
	{
		UCharacterMovementComponent * ownerMovementComp = Cast<UCharacterMovementComponent>(GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
		_OriginalMaxSpeed = ownerMovementComp->MaxWalkSpeed;
		ownerMovementComp->MaxWalkSpeed = 0;
		_IsStunned = true;

		UKismetSystemLibrary::K2_SetTimerDelegate(UnstunDelegate, m_StunTime, false);
	}
}

void UPlayerStunComponent::Unstun()
{
	UCharacterMovementComponent * ownerMovementComp = Cast<UCharacterMovementComponent>(GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	ownerMovementComp->MaxWalkSpeed = _OriginalMaxSpeed;
	_IsStunned = false;

	StartGracePeriod();
}

void UPlayerStunComponent::StartGracePeriod()
{
	_IsInGracePeriod = true;
	UKismetSystemLibrary::K2_SetTimerDelegate(EndGracePeriodDelegate, m_GracePeriod, false);
}

void UPlayerStunComponent::EndGracePeriod()
{
	_IsInGracePeriod = false;
}
