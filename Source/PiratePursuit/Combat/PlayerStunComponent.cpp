#include "PlayerStunComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UPlayerStunComponent::UPlayerStunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USoundWave> PunchSoundObject(TEXT("SoundWave'/Game/PiratePursuit/Audio/punch.punch'"));
	m_PunchSound = Cast<USoundBase>(PunchSoundObject.Object);

	BindDelegates();
}

void UPlayerStunComponent::BindDelegates()
{
	_StunDelegate.BindUFunction(this, "Stun");
	_UnstunDelegate.BindUFunction(this, "Unstun");
	_StartGracePeriodDelegate.BindUFunction(this, "StartGracePeriod");
	_EndGracePeriodDelegate.BindUFunction(this, "EndGracePeriod");
}

void UPlayerStunComponent::Stun()
{
	UGameplayStatics::PlaySound2D(this, m_PunchSound);

	if (!m_IsStunned && !m_IsInGracePeriod)
	{
		UCharacterMovementComponent * ownerMovementComp = Cast<UCharacterMovementComponent>(GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
		_OriginalMaxSpeed = ownerMovementComp->MaxWalkSpeed;
		ownerMovementComp->MaxWalkSpeed = 0;
		m_IsStunned = true;

		UKismetSystemLibrary::K2_SetTimerDelegate(_UnstunDelegate, m_StunTime, false);
	}
}

void UPlayerStunComponent::Unstun()
{
	UCharacterMovementComponent * ownerMovementComp = Cast<UCharacterMovementComponent>(GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	ownerMovementComp->MaxWalkSpeed = _OriginalMaxSpeed;
	m_IsStunned = false;

	StartGracePeriod();
}

void UPlayerStunComponent::StartGracePeriod()
{
	m_IsInGracePeriod = true;
	UKismetSystemLibrary::K2_SetTimerDelegate(_EndGracePeriodDelegate, m_GracePeriod, false);
}

void UPlayerStunComponent::EndGracePeriod()
{
	m_IsInGracePeriod = false;
}
